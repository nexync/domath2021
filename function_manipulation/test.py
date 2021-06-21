import numpy as np
import random
import time
import pysol

sol = pysol.PySolution(4,9)

file1 = open("c_vals", "rb")
cv = np.load(file1)

file2 = open("params", "rb")
par = np.load(file2)


points = []
cvals = []
for i,p in enumerate(par):
	if cv[i] > 2:
		arr = []
		temp = [0]
		for j in range(8):
			temp.append(p[0][j])
		arr.append(temp)
		arr.append([0,1.,0,0,0,0,0,0,0])
		temp = [0]
		for j in range(8):
			temp.append(p[1][j])
		arr.append(temp)
		arr.append([0,0,1.,0,0,0,0,0,0])

		point, cval = sol.steepest_descent(arr, 0.00000001, 0.000001, 1000)
		print(cv[i])

		print("New Point: ", point)
		print("c-value", 1/cval)

		points.append(point)
		cvals.append(cval)

