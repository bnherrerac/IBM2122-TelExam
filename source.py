import matplotlib.pyplot as plt
import numpy as np
import os

class TelExam():
    def __init__(self):
        '''Instance the model.
        folder_path: path to current. (string)
        '''
        self.folder_path = r"C:\Users\bherr\Downloads\IBM2122-TelExam"

    def get_data(self, filename):
        loc = os.path.join(self.folder_path, filename)
        print("location =", loc)
        data = open(loc, 'r')
        contents = data.read()
        measurements = np.array(contents.split('.'))
        data_amount = len(measurements[0].split(','))
        print("data_amount = ", data_amount)

        measure_num = np.shape(measurements)[0]
        separated = np.zeros((measure_num,data_amount))
        print("shape final = ", np.shape(separated))

        for i in range(measure_num):
            line = measurements[i].split(',')
            print("line =", line)
            if line != ['']:
                for j in range(data_amount):
                    separated[i,j] = line[j]
        print(separated)

        separated = separated[:,:]
        
        data.close()

        return separated, data_amount

    def plot_data(self, data, data_amount):
        x_axis = np.arange(np.shape(data)[0])
        print("x_axis shape= ", np.shape(x_axis))
        print("separated[:,0].shape =", np.shape(data[:,0]))
        labels = ['EMG', 'ax','ay','az','gx','gy','gz']

        fig, ax = plt.subplots(figsize=(18,10))
        for j in range(data_amount):
            ax.plot(x_axis, data[:,j], label=labels[j])
            if j == data_amount - 1:
                ax.legend(loc = 'upper left', bbox_to_anchor=(1.02,1))
        plt.show()