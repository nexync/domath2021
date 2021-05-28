import numpy as np
import time
import random
from tqdm import tqdm

import pysol

sol_obj = pysol.PySolution()

#First Parameter is number of iterations per starting point
#Second Parameter is number of starting points, if starting point is not under threshold, then point is not opttimized
#Third Parameter is target value that a starting point needs to be less than for optimization to begin

c, params = sol_obj.optimize(1000, 100000, 10)

print("Best c Value ", c)
print("Parameters ", params)

