import numpy as np
import random
import time
import pysol

sol = pysol.PySolution(4,11)

params = np.zeros((4,11))
for i in range(4):
	for j in range(11):
		params[i][j] = random.uniform(-1,1)

start = time.perf_counter()
d = sol.evaluate(params)
end = time.perf_counter()
print(end-start)
print(d)