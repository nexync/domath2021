# distutils: language = c++

import numpy as np
cimport numpy as np

np.import_array()

from Solution cimport Solution
from Solution cimport opVal
from Solution cimport retStruct
from array_pointer cimport *

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.

cdef class PySolution:
	cdef Solution sol  # Hold a C++ instance which we're wrapping

	def __cinit__(self):
		self.sol = Solution()

	cpdef create_random_params(self, centers, double width):
		cdef double** inputCenters = arr_to_ptr_ptr(2)

		cdef double* centers_a = arr_to_ptr(10)
		cdef double* centers_b = arr_to_ptr(10)
		cdef int k
		for k in range(10):
			centers_a[k] = centers[0][k]
			centers_b[k] = centers[1][k]
		inputCenters[0] = centers_a
		inputCenters[1] = centers_b

		cdef double** arr = self.sol.create_random_params(inputCenters, width)
		cdef np.ndarray[double, ndim = 2] ret
		ret = np.zeros((2,10)).astype(np.double)

		cdef int i,j
		for i in range(2):
			for j in range(10):
				ret[i][j] = arr[i][j]	
		return ret
	
	cpdef double i_fast(self, params, int intervals):
		cdef double** ret = arr_to_ptr_ptr(2)

		cdef double* p_a = arr_to_ptr(10)
		cdef double* p_b = arr_to_ptr(10)
		cdef int i,j
		for i in range(10):
			p_a[i] = params[0][i]
			p_b[i] = params[1][i]
		ret[0] = p_a
		ret[1] = p_b
		
		return self.sol.i_fast(ret, intervals)
	
	cpdef double check_integral(self, params, int intervals):
		cdef double** ret = arr_to_ptr_ptr(2)

		cdef double* p_a = arr_to_ptr(10)
		cdef double* p_b = arr_to_ptr(10)
		cdef int i
		for i in range(10):
			p_a[i] = params[0][i]
			p_b[i] = params[1][i]
		ret[0] = p_a
		ret[1] = p_b

		return self.sol.check_integral(ret, intervals)


	cpdef optimize(self, int iterations, int num_starts, double target):
		cdef retStruct output = self.sol.optimize(iterations, num_starts, target)
		cdef opVal* ret = output.values
		cdef int k = output.size

		cdef np.ndarray[double, ndim = 1] c_values = np.zeros((k,)).astype(np.double)
		cdef np.ndarray[double, ndim = 3] params = np.zeros((k,2,10)).astype(np.double)

		cdef int a,i,j
		for a in range(k):
			#cdef np.ndarray[double, ndim = 2] temp_params
			#temp_params = np.zeros((2,10)).astype(np.double)
			for i in range(2):
				for j in range(10):
					params[a][i][j] = ret[a].params[i][j]	
			c_values[a] = ret[a].c
		return c_values, params

	cpdef better_optimize(params, int max_iters, double threshold, double initial_width):
		cdef double** inp = arr.arr_to_ptr_ptr(2)

		cdef double* p_a = arr_to_ptr(10)
		cdef double* p_b = arr_to_ptr(10)

		cdef int i
		for i in range(10):
			p_a[i] = params[0][i]
			p_b[i] = params[1][i]
		inp[0] = p_a
		int[1] = p_b

		cdef opVal ret = self.sol.better_optimize(inp, max_iters, threshold, initial_width)
		cdef np.ndarray[double, ndim = 2] params = np.zeros((2,10)).astype(np.double)

		for i in range(10):
			params[0][i] = ret.params[0][i]
			params[1][i] = ret.params[0][i]
		return ret.c, params