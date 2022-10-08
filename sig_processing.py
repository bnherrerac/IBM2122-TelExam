import matplotlib.pyplot as plt
import numpy as np
import os
from source import TelExam

project = TelExam()
print("project.folderpath = ", project.folder_path)
data, data_amount = project.get_data('4.txt')
project.plot_data(data, data_amount-5)

