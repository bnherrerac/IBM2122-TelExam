import matplotlib.pyplot as plt
import numpy as np
import os
from scipy.fft import fft, ifft, fftshift, ifftshift, fftfreq
import neurokit2 as nk

class TelExam():
    def __init__(self):
        '''Instance the model.
        folder_path: path to current. (string)
        '''
        self.folder_path = r"C:\Users\bherr\Downloads\IBM2122-TelExam"

    def get_data(self, filename):
        loc = os.path.join(self.folder_path, filename)
        # print("location =", loc)
        data = open(loc, 'r')
        contents = data.read()
        measurements = np.array(contents.split('\n'))
        data_amount = len(measurements[0].split(','))
        # print("data_amount = ", data_amount)

        measure_num = np.shape(measurements)[0]
        separated = np.zeros((measure_num,data_amount))
        # print("shape final = ", np.shape(separated))

        for i in range(measure_num):
            line = measurements[i].split(',')
            if line != ['']:
                # print("valid line =", line)
                for j in range(data_amount):
                    separated[i,j] = line[j]
        # print(separated)

        separated = separated[:measure_num-1,:]
        
        data.close()

        return separated, data_amount

    def rms(self, y):
        # print("y shape =", np.shape(y))
        # print("y type = ", type(y))
        hop_length = 1
        frame_length = 10
        pad_y = np.concatenate((np.zeros(frame_length//2), y, np.zeros(frame_length//2)))
        rms = np.sqrt(np.array([sum(abs(pad_y[i-frame_length//2:i+frame_length//2]**2))/frame_length for i in range(frame_length//2, np.shape(y)[0]+frame_length//2, hop_length)]))
        # print("rms shape =", np.shape(rms))
        
        fig, ax = plt.subplots()
        t = np.arange(np.shape(rms)[0])
        ax.plot(t, y, color='0.8', label='original signal')
        ax.plot(t, rms, color='0', label='rms')
        ax.legend(loc = 'upper left', bbox_to_anchor = (1.02, 1))
        plt.show()

        return rms    

    def mvc_calibration(self, data):
        mvc_emg = data[:,-1]
        mvc_rms = self.rms(mvc_emg)
        rms_value = np.sqrt(np.sum(mvc_emg**2)/np.shape(mvc_emg)[0])

        fig, ax = plt.subplots()
        x_axis = np.arange(np.shape(mvc_emg)[0])
        ax.plot(x_axis, mvc_emg, label='mvc_emg')
        ax.plot(x_axis, mvc_rms, label='mvc_rms')
        ax.axhline(y=rms_value, label='rms_value')
        ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        plt.show()

        return rms_value

    def plot_data(self, data, data_amount, data_to_show=[]):
        x_axis = np.arange(np.shape(data)[0])
        # print("x_axis shape= ", np.shape(x_axis))
        # print("separated[:,0].shape =", np.shape(data[:,0]))
        labels = ['theta_x', 'theta_y', 'theta_z', 'EMG']

        fig, ax = plt.subplots(figsize=(18,10))
        if data_to_show == []:
            for j in range(data_amount):
                ax.plot(x_axis, data[:,j], label=labels[j])
                if j == data_amount - 1:
                    ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        else:
            for j in data_to_show:
                ax.plot(x_axis, data[:,j], label=labels[j])
                if j == data_amount - 1:
                    ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))                
        plt.show()
    
    def generate_fft_and_plot(self, x):
        print("Shape of fft input: ", np.shape(x))
        t = np.arange(np.shape(x)[0])
        y = fft(x)
        shifted_y = fftshift(y)
        freq = fftshift(fftfreq(t.shape[-1]))
        fig, ax = plt.subplots()
        ax.plot(freq, shifted_y, label='with fftshift')
        ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        plt.show()

        return freq, shifted_y

    def low_pass(self, freq, y):
        mask = np.abs(freq)<0.05
        print(mask)
        f = y*mask
        
        fig, ax = plt.subplots()
        ax.plot(freq, y, label='before filter')
        ax.plot(freq, f, label='after filter')
        ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        plt.show()

        unshifted_y = ifftshift(y)
        unshifted_f = ifftshift(f)

        x = ifft(unshifted_y)
        filtered_x = ifft(unshifted_f)

        t = np.arange(np.shape(x)[0])
        print("t = ", t)

        fig, ax = plt.subplots()
        ax.plot(t, x, label='before filter')
        ax.plot(t, filtered_x, label='after filter')
        ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        plt.show()

        return t, filtered_x

    def eda_process(self, x, sampling_rate):
        signals, info = nk.eda_process(x, sampling_rate=sampling_rate) # experimental, fueron 4666 mediciones en 128 segundos, 4666/128 ~ 36.45
        nk.eda_plot(signals, sampling_rate=sampling_rate)
        plt.show()

    



