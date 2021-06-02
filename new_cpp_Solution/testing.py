import numpy as np
import time
import random
from tqdm import tqdm

import pysol

sol_obj = pysol.PySolution()

c, params = sol_obj.optimize(5000, 1000, 25)

#params = [ 0.69710904 -0.2324088  -0.27294162 -0.20181291 -0.10823596  0.22151731 0.1156492  -0.02838143 -0.02024559 -0.04363271]

print("Best c Value ", c)
print("Parameters ", params)

