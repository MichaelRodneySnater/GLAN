#include "Tracker.hpp"
#include <Eigen/Dense>

using namespace Eigen;

Tracker::Tracker()
:   m_frame(0),
    m_tracks(),
    m_dets(),
    m_numActiveTracks(0),
    m_activeList(TRACK_MAX, -1),
    m_perfTruth(TRUTH_MAX, PerfTruth(-1, -1, false, -1))
{
}

Tracker::~Tracker()
{
}

void Tracker::predictTrackLocation(double dt)
{
    // STATE TRANSITION MATRIX F
        MatrixXd F = MatrixXd::Zero(4,4);
        F <<    1, 0, dt, 0,
                0, 1, 0, dt,
                0, 0, 1, 0, 
                0, 0, 0, 1;

        double process_noise = PROC_NOISE;
        MatrixXd Q = MatrixXd::Zero(4,4);

        Q(0,0) = process_noise * dt*dt*dt*dt / 4;
        Q(0,2) = process_noise * dt*dt*dt / 2;
        Q(1,1) = process_noise * dt*dt*dt*dt / 4;
        Q(1,3) = process_noise * dt*dt*dt / 2;
        Q(2,0) = process_noise * dt*dt*dt / 2;
        Q(2,2) = process_noise * dt*dt;
        Q(3,1) = process_noise * dt*dt*dt / 2;
        Q(3,3) = process_noise * dt*dt;

    // Loop over the m_tracks and predict the current location with constant velocity model
    for (int trkIdx = 0; trkIdx < m_numActiveTracks; trkIdx++)
    {
        int tf = m_activeList[trkIdx]; 

        // Process Model
        m_tracks.trackFiles[tf].predState = F * m_tracks.trackFiles[tf].estState;

        // Predict the Covariance Matrix
        m_tracks.trackFiles[tf].predCov = F * m_tracks.trackFiles[tf].estCov * F.transpose() + Q;
    }
}

void Tracker::updateTrackEstPosition()
{
    for (int trkIdx = 0; trkIdx < m_numActiveTracks; trkIdx++)
    {
        int tf = m_activeList[trkIdx];
        if (m_tracks.trackFiles[tf].corrDet == NO_CORRELATION)
        {
            m_tracks.trackFiles[tf].estState = m_tracks.trackFiles[tf].predState;
            m_tracks.trackFiles[tf].estCov = m_tracks.trackFiles[tf].predCov;

        }
        else if (m_tracks.trackFiles[tf].corrDet != NO_CORRELATION)
        {
            // Update the track state estimate with the detection information
            
            // Compute the Kalman Gain
            MatrixXd S = MatrixXd::Zero(2,2);
            S = m_tracks.H * m_tracks.trackFiles[tf].predCov * m_tracks.H.transpose() + m_dets.measCov;
            m_tracks.trackFiles[tf].K = m_tracks.trackFiles[tf].predCov * m_tracks.H.transpose() * S.inverse(); 

            // Calculate the residual
            MatrixXd residual(2,1);
            residual(0) = m_dets.detList[m_tracks.trackFiles[tf].corrDet].pos[0] - m_tracks.trackFiles[tf].predState(0);
            residual(1) = m_dets.detList[m_tracks.trackFiles[tf].corrDet].pos[1] - m_tracks.trackFiles[tf].predState(1);

            // Compute the Track Estimated Position
            m_tracks.trackFiles[tf].estState = m_tracks.trackFiles[tf].predState + m_tracks.trackFiles[tf].K * residual;

            // Compute the Estimate Covariance 
            m_tracks.trackFiles[tf].estCov = (MatrixXd::Identity(4,4) - m_tracks.trackFiles[tf].K * m_tracks.H) * m_tracks.trackFiles[tf].predCov;
        }
    }
    
}

void Tracker::checkPersistency()
{
    for (int track = 0; track < TRACK_MAX; track++)
    {

        if (m_tracks.trackFiles[track].state != CLOSED)
        {
            std::vector<double> tempEstState = std::vector<double>(2,0);
            tempEstState[0] = m_tracks.trackFiles[track].estState(0);
            tempEstState[1] = m_tracks.trackFiles[track].estState(1);
            // Update each trackFiles persistancy metric
            if ((m_tracks.trackFiles[track].corrDet != -1) && (m_tracks.trackFiles[track].persistance < 4))
            {
                m_tracks.trackFiles[track].persistance++;

                // Are we persistent enough to be converged 
                if (m_tracks.trackFiles[track].persistance > 2)
                {
                    m_tracks.trackFiles[track].state = CONVERGED;
                }
            }
            else if((m_tracks.trackFiles[track].corrDet == -1) && (m_tracks.trackFiles[track].persistance > 0))
            {
                m_tracks.trackFiles[track].persistance--;

                if (m_tracks.trackFiles[track].persistance == 0)
                {
                    m_tracks.trackFiles[track].state = CLOSED;
                    m_tracks.trackFiles[track].reset();
                    m_tracks.numTracks--;
                }
            }
            else if (true == offFov(tempEstState))
            {
                m_tracks.trackFiles[track].state = CLOSED;
                m_tracks.trackFiles[track].reset();
                m_tracks.numTracks--;
            }
        }
    }
}

bool Tracker::offFov(const std::vector<double>& estState)
{
    int colStart = 0;
    int rowStart = 0;
    int colStop = 1920;
    int rowStop = 1080;

    if ((estState[0] < colStart) || (estState[0] > colStop) || (estState[0] < rowStart) || (estState[1] > rowStop))
    {
        return true;
    }
    else 
    {
        return false;
    }
}

void Tracker::attemptOpenTracks()
{
    // Loop through all valid m_dets
    for (int det = 0; det < m_dets.numDets; det++)
    {
        if (m_dets.detList[det].valid && !m_dets.detList[det].correlated)
        {
            for (int track = 0; track < TRACK_MAX; track++)
            {
                if (m_tracks.trackFiles[track].state == CLOSED)
                {
                    m_tracks.trackFiles[track].state = OPEN;
                    m_tracks.trackFiles[track].corrDet = det;
                    // m_tracks.trackFiles[track].persistance++;

                    m_tracks.trackFiles[track].estState(0)  = m_dets.detList[det].pos[0];
                    m_tracks.trackFiles[track].estState(1)  = m_dets.detList[det].pos[1];
                    m_tracks.trackFiles[track].predState(0) = m_dets.detList[det].pos[0];
                    m_tracks.trackFiles[track].predState(1) = m_dets.detList[det].pos[1];

                    m_tracks.trackFiles[track].predState(2) = 0.0;
                    m_tracks.trackFiles[track].predState(3) = 0.0;
                    m_tracks.trackFiles[track].estState(2)  = 0.0;
                    m_tracks.trackFiles[track].estState(3)  = 0.0;

                    m_tracks.numTracks++;

                    // Assign UniqueId
                    m_tracks.trackFiles[track].uniqueId = track + m_tracks.m_numTimesOpened[track] * TRACK_MAX;
                    m_tracks.m_numTimesOpened[track]++;

                    break;
                }
            }

            m_dets.numUncorrDets++;
        }
    }
}

void Tracker::modifyActiveList()
{   
    m_activeList.assign(m_activeList.size(), -1);
    m_numActiveTracks = 0;

    for (int track = 0; track < TRACK_MAX; track++)
    {
        if(m_tracks.trackFiles[track].state == OPEN)
        {
            m_activeList[m_numActiveTracks] = track;
            m_numActiveTracks++;
        }
        if (m_tracks.trackFiles[track].state == CONVERGED)
        {
            m_activeList[m_numActiveTracks] = track;
            m_numActiveTracks++;
        }
    }
}

void Tracker::fillPerformanceArray()
{
    // Loop over truth IDs and fill in Perf Truth Objects
    for (int det = 0; det < m_dets.numDets; ++det)
    {
        if (m_dets.detList[det].truth_id != -1)
        {
            int perfTruthId = m_dets.detList[det].truth_id;

            m_perfTruth[perfTruthId].valid_ = true;
            m_perfTruth[perfTruthId].truthId_ = perfTruthId; // Redundant since the index of the array is the truth ID. Might be easier to use in the processing script
            m_perfTruth[perfTruthId].frameNum_ = m_frame;
        }
    }

    // Loop over tracks and fill Perf Truth Objects
    for (int track = 0; track < m_numActiveTracks; ++track)
    {
        int activeTrkId = m_activeList[track];

        if(m_tracks.trackFiles[activeTrkId].truth_id != -1)
        {
            int perfTruthId = m_tracks.trackFiles[activeTrkId].truth_id;

            m_perfTruth[perfTruthId].valid_ = true;
            m_perfTruth[perfTruthId].trackUniqueId_ = m_tracks.trackFiles[activeTrkId].uniqueId;
            m_perfTruth[perfTruthId].frameNum_ = m_frame;
        }   
    }

}

void Tracker::updateFrameVariables()
{
    // Clear the associations. The next frame needs the m_tracks to be a clean slate. 
    m_dets.numUncorrDets = 0;
    // Set track gates based on expected estimation error

}

void Tracker::frameCleanUp()
{
    // Clear the associations. The next frame needs the m_tracks to be a clean slate. 
    for (int track = 0; track < TRACK_MAX; track++)
    {
        m_tracks.trackFiles[track].cleanCorrelated();
    }
    // Clear the Detection information
    for (int det = 0; det < DET_MAX; det++)
    {
        m_dets.detList[det].clearDet();
    }
}

void Tracker::updateTrackVariables()
{   
    Tracker::attemptOpenTracks();

    Tracker::checkPersistency();

    Tracker::modifyActiveList();
}

void Tracker::correctTrackState(double dt)
{
}