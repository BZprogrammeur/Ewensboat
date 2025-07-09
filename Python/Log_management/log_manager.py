import os
import sys
import numpy as np
import matplotlib.pyplot as plt
%matplotlib qt


def read_log(log_number : int) -> np.ndarray:
    logfilename = 'Log_files/navlog' + str(log_number) + '.txt'
    logfile = open(logfilename, 'r')
    line = 'Starting...'
    logs = []
    line = logfile.readline()
    while line:
        values = line.split()
        logs.append([float(val) for val in values])
        line = logfile.readline()
    logs = np.array(logs).T
    print(logs)
    return logs
    
def get_latest_log_number() -> int:
    file_list = os.listdir('Log_files')
    max_number = 0
    for filename in file_list:
        if filename[0:6] == 'navlog' and int(filename[6:-4]) > max_number and filename[-4:] == '.txt':
            max_number = int(filename[6:-4])
    return max_number

def draw_trajectory(logs : np.ndarray) -> None:
    lat = logs[1] * np.pi / 180
    long = logs[2] * np.pi / 180
    R = 6371e3
    x = R * (long - long[0]) * np.cos(lat[0])
    y = R * (lat - lat[0])
    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_title("Boat's trajectory")
    return

def map2(x : float, in_min : float, in_max : float, out_min : float, out_max : float) -> float:
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

def conv_rud_com_to_pos(com: np.ndarray) -> np.ndarray:
    return np.clip(map2(com, 180, 430, -85, 85), -85, 85)

def conv_sail_com_to_pos(com: np.ndarray) -> np.ndarray:
    return np.clip(map2(com, 180, 280, 0, 90), 0, 90)
    
def generate_animation(logs : np.ndarray) -> None:
    lat = logs[1] * np.pi / 180
    long = logs[2] * np.pi / 180
    head = logs[3] * np.pi / 180
    rud_angles = conv_rud_com_to_pos(logs[4]) * np.pi / 180
    sail_angles = conv_sail_com_to_pos(logs[5]) * np.pi / 180
    R = 6371e3
    x = R * (long - long[0]) * np.cos(lat[0])
    y = R * (lat - lat[0])
    print(f'x = {x}')
    print(f'y = {y}')
    time = logs[0]
    fig, ax = plt.subplots()
    ax.xmin=-50
    ax.xmax=50
    ax.ymin=-50
    ax.ymax=50
    size_factor = 3
    dir_wind = (logs[4] - logs[3]) * np.pi / 180
    boat_shape = size_factor * np.array([[-0.25, 0, 0.25, 0.25, -0.25, -0.25],
                           [0., 1, 0., -2., -2., 0.]])
    sail_shape = size_factor * np.array([[0., 0.],
                           [0., -0.9]])
    rud_shape = size_factor * np.array([[0., 0.],
                          [-0., -0.3]])
    arrow_pos = (0.85, 0.85)
    for i in range(len(logs[0])):
        ax.clear()
        ax.set_xlim(ax.xmin,ax.xmax)
        ax.set_ylim(ax.ymin,ax.ymax)
        print(f'i = {i}')
        R_boat = np.array([[np.cos(head[i]), -np.sin(head[i])],
                           [np.sin(head[i]), np.cos(head[i])]]) 
        R_rud = np.array([[np.cos(rud_angles[i]), -np.sin(rud_angles[i])],
                          [np.sin(rud_angles[i]), np.cos(rud_angles[i])]])
        R_sail = np.array([[np.cos(sail_angles[i]), -np.sin(sail_angles[i])],
                           [np.sin(sail_angles[i]), np.cos(sail_angles[i])]])
        R_wind = np.array([[np.cos(dir_wind[i]), - np.sin(dir_wind[i])],
                           [np.sin(dir_wind[i]), np.cos(dir_wind[i])]])
        arrow_tip = R_wind @ np.array([[0.], [0.1]])
        arrow_tip_pos = (arrow_pos[0] + arrow_tip[0, 0], arrow_pos[1] + arrow_tip[1, 0])
        print((R_boat @ boat_shape)[0] + x[i])
        ax.plot((R_boat @ boat_shape)[0] + x[i], (R_boat @ boat_shape)[1] + y[i])
        ax.plot((R_sail @ R_boat @ sail_shape)[0] + x[i], (R_sail @ R_boat @ sail_shape)[1] + y[i])
        ax.plot((R_rud @ R_boat @ rud_shape)[0] + x[i], (R_rud @ R_boat @ rud_shape)[1] + y[i] - 2 * size_factor)
        ax.annotate(
            '', 
            xy=arrow_tip_pos, xycoords='axes fraction',
            xytext=arrow_pos, textcoords='axes fraction',
            arrowprops=dict(facecolor='red', shrink=0.05)
            )
        ax.text(0.7, 0.8, f'Apparent wind speed : {logs[5][i]}',
                transform=ax.transAxes,
                fontsize=8)

        plt.pause(1)
    return

if __name__ == '__main__' :
    print(generate_animation(read_log(get_latest_log_number())))
     
    