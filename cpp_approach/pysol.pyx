# distutils: language = c++

import numpy as np
cimport numpy as np

np.import_array()

from Solution cimport Solution
from array_pointer cimport *

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.

cdef class PySolution:
	cdef Solution sol  # Hold a C++ instance which we're wrapping

	def __cinit__(self):
		self.sol = Solution()

	cpdef create_random_params(self):
		cdef float** arr = self.sol.create_random_params()
		cdef np.ndarray[float, ndim = 2] ret
		ret = np.zeros((2,6)).astype(np.single)

		cdef int i,j
		for i in range(2):
			for j in range(6):
				ret[i][j] = arr[i][j]	
		return ret
	
	# cpdef float f_x(self, params, float x, float y):
	# 	cdef float* ret = arr_to_ptr(6)
	# 	cdef int i
	# 	for i in range(6):
	# 		ret[i] = params[i]
	# 	return self.sol.f_x(ret, x, y)

	# cpdef float f_y(self, params, float x, float y):
	# 	cdef float* ret = arr_to_ptr(6)
	# 	cdef int i
	# 	for i in range(6):
	# 		ret[i] = params[i]
	# 	return self.sol.f_y(ret, x, y)
	
	cpdef float i_fast(self, params, int intervals):
		cdef float** ret = arr_to_ptr_ptr(2)

		cdef float* p_a = arr_to_ptr(6)
		cdef float* p_b = arr_to_ptr(6)
		cdef int i,j
		for i in range(6):
			p_a[i] = params[0][i]
		for j in range(6):
			p_b[j] = params[1][j]
		ret[0] = p_a
		ret[1] = p_b
		
		return self.sol.i_fast(ret, intervals)