import matplotlib.pyplot as plt
import numpy as np
import os
from source import TelExam
import csv
import scipy.signal as sig

# Initialisation of a TelExam object
project = TelExam()
print("project.folderpath = ", project.folder_path)

# Get maximum voluntary contraction (MVC) signal for calibration
# data_mvc, data_amount_mvc = project.get_data('ang_emg.txt')
# rms_value = project.mvc_calibration(data_mvc[915:1119,:]) # selected interval for the same ang_emg file

# Get angle and EMG data for analysis
# datacd, data_amount = project.get_data('con_rodillera.txt')
# print("shape cd: ", np.shape(datacd))
# project.plot_data(datacd, data_amount) # 0 theta_x, 1 theta_y, 2 theta_z, 3 EMG1, 4 EMG2
# project.plot_data(datasd[:,:3], 3)
# emg_data = data[:,3]
# freq, y = project.generate_fft_and_plot(datacd)
# t, filtered_data = project.low_pass(freq, y)
# data_rms = project.rms(emg_data)
# filtered_data_rms = project.rms(filtered_data)
# project.eda_process(data[:,4], sampling_rate=37)

data, dataam = project.read_from_realtime('realtime.txt')
project.plot_data(data, dataam)
