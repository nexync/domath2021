import numpy as np

file = open("c_vals", "rb")
arr1 = np.load(file)

print(arr1)