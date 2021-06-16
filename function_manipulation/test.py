import numpy as np
import random
import pysol

sol = pysol.PySolution()

params = np.zeros((4,11))
for i in range(4):
	for j in range(11):
		params[i][j] = random.uniform(-1,1)

d = sol.evaluate(params, 10000)
print(d)