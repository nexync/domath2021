import numpy as np
import time
import random
from tqdm import tqdm

import pysol

sol_obj = pysol.PySolution()

c, params = sol_obj.optimize(1000, 100, 100)

print("Best c Value ", c)
print("Parameters ", params)

