import matplotlib.pyplot as plt
import numpy as np
import os
from source import TelExam

project = TelExam()
print("project.folderpath = ", project.folder_path)
data, data_amount = project.get_data('ang_emg.txt')
project.plot_data(data, data_amount, data_to_show=np.arange(1,4)) # 0 theta_x, 1 theta_y, 2 theta_z, 3 EMG
emg_data = data[:,3]
freq, y = project.generate_fft_and_plot(emg_data)
t, filtered_data = project.low_pass(freq, y)
project.rms(freq, emg_data)
project.rms(freq, filtered_data)
project.eda_process(emg_data)