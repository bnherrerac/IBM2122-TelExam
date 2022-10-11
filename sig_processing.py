import matplotlib.pyplot as plt
import numpy as np
import os
from source import TelExam

# Initialisation of a TelExam object
project = TelExam()
print("project.folderpath = ", project.folder_path)

# Get maximum voluntary contraction (MVC) signal for calibration
data_mvc, data_amount_mvc = project.get_data('ang_emg.txt')
rms_value = project.mvc_calibration(data_mvc[915:1119,:]) # selected interval for the same ang_emg file

# Get angle and EMG data for analysis
data, data_amount = project.get_data('ang_emg.txt')
project.plot_data(data, data_amount, data_to_show=np.arange(1,4)) # 0 theta_x, 1 theta_y, 2 theta_z, 3 EMG
emg_data = data[:,3]
freq, y = project.generate_fft_and_plot(emg_data)
t, filtered_data = project.low_pass(freq, y)
data_rms = project.rms(emg_data)
filtered_data_rms = project.rms(filtered_data)
project.eda_process(emg_data, sampling_rate=37)