import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
output_images = "SCENE_FIGS"
output_data = "SCENE_DATA"
# I want to generate a random set of truth objects, set A 
# I want to model Pdet and false alarms, set B
# Generate a bit of random noise on the dets
# I want this to auto generate the correct truth assignment
    # This will take into account the cost of non-association 
def write_frame_to_csv(frame, detections, tracks):
    csv_filename = f'SCENE_DATA/output.csv'
    dataFrameDets = pd.DataFrame(detections,columns=['x','y'])
    dataFrameDets['frame'] = frame
    dataFrameDets['type'] = 'detection'
    dataFrameDets['id'] = dataFrameDets.index
    dataFrameTracks = pd.DataFrame(tracks, columns=['x', 'y'])
    dataFrameTracks['frame'] = frame
    dataFrameTracks['type'] = 'track'
    dataFrameTracks['id'] = dataFrameTracks.index
    dataFrameAll = pd.concat([dataFrameDets,dataFrameTracks], ignore_index=True)
    dataFrameAll = dataFrameAll[['frame','type','id','x','y']]
    # Append or write CSV
    mode = 'a' if frame > 0 else 'w'  # Append after first frame
    header = (frame == 0)
    dataFrameAll.to_csv(csv_filename, mode=mode, header=header, index=False)

def add_noise(dets, std=0.1111):
    dets = np.array(dets)
    noise = np.random.normal(0, std, size=dets.shape)
    noisyDets = dets + noise

    return noisyDets


def generate_dets(rows, cols, tracks, pDet, faRate, std=3):
    dets = []
    truthAss = np.full(len(tracks),-1)
    detIdx = 0
    # Reduce the track list using pDet
    for ii in range(len(tracks)):
        if np.random.rand() < pDet:
            dets.append(tracks[ii])
            # Generate the truth assignment
            truthAss[ii] = detIdx
            detIdx+=1


    # Add the false alarms
    x = np.random.uniform(0, rows, faRate)
    y = np.random.uniform(0, cols, faRate)
    falseAlarms = np.stack((x,y), axis=1)
    dets.extend(falseAlarms)
    # Add white noise to the measurements
    noisyDets = add_noise(dets,std)
    return noisyDets, truthAss


def random_tracks(X, rows, cols, seed=None):
    if seed is not None:
        np.random.seed(seed)

    x = np.random.uniform(0, cols, size=X)
    y = np.random.uniform(0, rows, size=X)
    trackObjects = np.stack((x,y), axis=1)
    return trackObjects

def gen_ass_problem(
    seed = None,
    pDet = 0.75, # probability of sensor detects
    faRate = 20, # false alarms per frame
    rows = 1024,
    cols = 1024,
    nTruth = 1
):
    volume = rows * cols
    if seed is not None:
        np.random.seed(seed)
    # Generate cost of non-association
    cost_non_ass = -np.log(((1-pDet) * volume / faRate))
    # Generate the tracks
    tracks = random_tracks(nTruth,rows,cols,seed)
    # Generate detections based on pDet
    detections, truth_ass = generate_dets(rows, cols, tracks, pDet, faRate)


    return cost_non_ass, tracks, detections, truth_ass

# 70% GO
ROWS = 1024
COLS = 1024
TRACKS = 35
costArray = []
for frame in range(2):
    cost, tracks, detections, truth_ass = gen_ass_problem(nTruth = TRACKS,
                                            pDet = 0.75, 
                                            seed=frame)

    write_frame_to_csv(frame, detections, tracks)

    plt.figure(figsize=(12,12))
    plt.scatter(tracks[:,0], tracks[:,1], c='green', marker='D', label='Tracks')
    for i, (x, y) in enumerate(tracks):
        plt.text(x-10, y-10, str(i), fontsize=8, color='black')

    plt.scatter(detections[:,0], detections[:,1], c='r', marker='.', label='Detections')
    for i, (x, y) in enumerate(detections):
        plt.text(x + 2, y + 2, str(i), fontsize=8, color='black')
    plt.legend(loc='upper right')
    plt.title("Tracks and Detections Scene")
    filename = os.path.join(output_images, f"scene_{frame}.png")
    plt.savefig(filename)
    plt.close()
