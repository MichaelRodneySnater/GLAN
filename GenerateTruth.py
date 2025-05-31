import numpy as np
import matplotlib.pyplot as plt
import os
import csv
from scipy.optimize import linear_sum_assignment
output_images = "SCENE_FIGS"
output_data = "SCENE_DATA"
# I want to generate a random set of truth objects, set A 
# I want to model Pdet and false alarms, set B
# Generate a bit of random noise on the dets
# I want this to auto generate the correct truth assignment
    # This will take into account the cost of non-association 
def square_cost_matrix(C, C_na):
    """
    Given an (m x n) cost matrix C and a non‐association cost C_na,
    return a (k x k) square matrix where k = max(m, n), by padding
    with C_na rows/columns as needed.
    """
    m, n = C.shape
    k = max(m, n)
    
    # Start with a full k×k of C_na
    C_sq = np.full((k, k), C_na, dtype=C.dtype)
    
    # Copy the original costs into the top‐left m×n block
    C_sq[:m, :n] = C
    
    return C_sq

def write_cost_matrix_to_csv(matrix_number, cost_matrix, numTracks, numDets, output_file_path):
    with open(output_file_path, "a", newline='') as f:
        writer = csv.writer(f)
        
        # Write header for this matrix
        writer.writerow(["matrixNumber", matrix_number, numTracks, numDets])
        
        # Write each row with "row, <row_idx>, <values...>"
        for row_idx in range(cost_matrix.shape[0]):
            row_data = ["row", row_idx] + list(cost_matrix[row_idx])
            writer.writerow(row_data)

def add_noise(dets, stdDets):
    dets = np.array(dets)
    noise = np.random.normal(0, stdDets, size=dets.shape)
    noisyDets = dets + noise

    return noisyDets


def generate_dets(rows, cols, tracks, pDet, faRate, stdDets):
    dets = []
    for ii in range(len(tracks)):
        if np.random.rand() < pDet:
            dets.append(tracks[ii])
    # Add the false alarms
    x = np.random.uniform(0, rows, faRate)
    y = np.random.uniform(0, cols, faRate)
    falseAlarms = np.stack((x,y), axis=1)
    dets.extend(falseAlarms)
    # Add white noise to the measurements
    noisyDets = add_noise(dets,stdDets)
    # return noisyDets, truthAss
    return noisyDets


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
    nTruth = 1,
    stdDets = 0.3333,
    stdTrks = 0.1111
):
    if seed is not None:
        np.random.seed(seed)
    # Generate cost of non-association
    cNA = -np.log(0.3*(1-pDet))
    print(f"Cost of Non-Association: {cNA}")
    # Generate the tracks
    tracks = random_tracks(nTruth,rows,cols,seed)
    # Generate detections based on pDet
    detections = generate_dets(rows, cols, tracks, pDet, faRate, stdDets)
    # Create the Cost matrix
    P = np.array([[stdTrks**2, 0],[0, stdTrks**2]])
    Q = np.array([[stdDets**2, 0],[0, stdDets**2]])
    S = P+Q
    cost_matrix = np.zeros((len(tracks), len(detections)))
    for trk in range(len(tracks)):
        for det in range(len(detections)):
            diff = tracks[trk] - detections[det]
            cost_matrix[trk, det] = np.sqrt(diff.T @ np.linalg.inv(S) @ diff)
    cNA_col = np.full((len(tracks), len(tracks)), cNA)
    cost_matrix = np.column_stack((cost_matrix, cNA_col))
    cost_matrix = square_cost_matrix(cost_matrix,cNA)

    return cNA, tracks, detections, cost_matrix

# 70% GO
ROWS = 1024
COLS = 1024
TRACKS = 35
costArray = []
output_file_path = output_data+"/matricies.csv"

# Generate Scene
for matrix in range(5):
    cost_non_ass, tracks, detections, cost_matrix = gen_ass_problem(nTruth  = TRACKS,
                                                                    pDet    = 0.5,
                                                                    faRate  = 35, 
                                                                    seed    = matrix,
                                                                    stdDets = 0.3333,
                                                                    stdTrks = 0.1111)
    
    write_cost_matrix_to_csv(matrix, cost_matrix, len(tracks), len(detections), output_file_path)
    
    plt.figure(figsize=(6, 6))
    plt.imshow(cost_matrix, cmap='viridis')
    plt.colorbar(label='Cost')
    plt.xlabel('Detections')
    plt.ylabel('Tracks')
    plt.title('Cost Matrix Visualization')
    filename = os.path.join(output_images, f"costMatrix_{matrix}.png")
    plt.savefig(filename)
    plt.close()

    plt.figure(figsize=(12,12))
    plt.scatter(tracks[:,0], tracks[:,1], c='green', marker='D', label='Tracks')
    for i, (x, y) in enumerate(tracks):
        plt.text(x-10, y-2, str(i), fontsize=8, color='black')

    plt.scatter(detections[:,0], detections[:,1], c='r', marker='.', label='Detections')
    for i, (x, y) in enumerate(detections):
        plt.text(x + 2, y + 2, str(i), fontsize=8, color='black')
    plt.legend(loc='upper right')
    plt.title("Tracks and Detections Scene")
    filename = os.path.join(output_images, f"scene_{matrix}.png")
    plt.savefig(filename)
    plt.close()
