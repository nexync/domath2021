import pysol
import numpy as np

sol_obj = pysol.PySolution()

for i in range(10):
	c, params = sol_obj.optimize(10000, 100, 50)

	print("Best c Value ", c)
	print("Parameters ", params)

	

	file1 = open("params"+str(i), "wb")
	np.save(file1, params)
	file1.close

	file2 = open("c_vals"+str(i), "wb")
	np.save(file2, c)
	file2.close

	

# out = [[-2.10638752e+000,  1.51590930e+001, -6.96432386e-002, -9.47050386e-001,-4.04119999e+000,  4.64790796e+000, -3.29355754e-001, -5.92242195e-001,  -2.44349498e+000, 1.69750053e+000],
# [ 1.95092013e+001,  3.38816492e+000, -2.24799341e-001,  6.40461440e-001,
# 	1.65086825e+000,  7.50999481e-001,  2.09234901e-001,  1.02359081e-001,
# 	-9.64384899e-003,  5.09659108e-001]]


# c = sol_obj.i_fast(out,1000000)
# print(c)

# check = sol_obj.check_integral(out,5000)
# print(check)