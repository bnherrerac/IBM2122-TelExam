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
data_sd, data_amount_sd = project.get_data('ValidacionCaminataLarga.txt')
data_cd, data_amount_cd = project.get_data('ValidacionCaminataConDolor.txt')

# project.plot_data(data_cd, data_amount_cd)

data_sd = data_sd[1170:1880,:]
data_cd = data_cd[725:1646-211,:]


print("shape sd: ", np.shape(data_sd))
print("shape cd: ", np.shape(data_cd))

rms_sd_vm = project.rms(data_sd[:,3])
rms_sd_gc = project.rms(data_sd[:,4])
rms_cd_vm = project.rms(data_cd[:,3])
rms_cd_gc = project.rms(data_cd[:,4])

# project.plot_data(rms_sd_vm, 1)

x = np.arange(710)/18

fig, ax = plt.subplots()
fig.set_size_inches(13,5)
ax.plot(x, rms_sd_vm, color="green", label="Sin dolor")
ax.plot(x, rms_cd_vm, color="orange", label="Con dolor")
ax.set_xlabel("Segundos [s]")
ax.set_ylabel("Potencial de actividad muscular [mV]")
ax.set_title("Activación del vasto medial durante una caminata con y sin dolor.")
ax.legend(loc="lower center", bbox_to_anchor=(0.94,0.76))
plt.show()


fig, ax = plt.subplots()
fig.set_size_inches(13,5)
ax.plot(x[5:-5], rms_sd_gc[5:-5], color="green", label="Sin dolor")
ax.plot(x, rms_cd_gc, color="orange", label="Con dolor")
ax.set_xlabel("Segundos [s]")
ax.set_ylabel("Potencial de actividad muscular [mV]")
ax.set_title("Activación del gastrocnemio durante una caminata con y sin dolor.")
ax.legend(loc="lower center", bbox_to_anchor=(0.94,0.76))

plt.show()


# rms_value = project.mvc_calibration(data_mvc[915:1119,:]) # selected interval for the same ang_emg file

# Get angle and EMG data for analysis
# datacd, data_amount = project.get_data('con_rodillera.txt')
# print("shape cd: ", np.shape(datacd))
# project.plot_data(datacd, data_amount) # 0 theta_x, 1 theta_y, 2 theta_z, 3 EMG1, 4 EMG2
# project.plot_data(datasd[:,:3], 3)
# emg_data = data[:,3]
# freq, y = project.generate_fft_and_plot(datacd)
# project.eda_process(data[:,4], sampling_rate=37)

# data, dataam = project.read_from_realtime('realtime.txt')
# project.plot_data(data, dataam)
