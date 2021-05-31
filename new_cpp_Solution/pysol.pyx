# distutils: language = c++

import numpy as np
cimport numpy as np

np.import_array()

from Solution cimport Solution
from Solution cimport opVal
from array_pointer cimport *

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.

cdef class PySolution:
	cdef Solution sol  # Hold a C++ instance which we're wrapping

	def __cinit__(self):
		self.sol = Solution()

	cpdef create_random_params(self, centers, float width):

		cdef float* new_centers = arr_to_ptr(10)
		cdef int k
		for k in range(6):
			new_centers[k] = centers[k]

		cdef float* arr = self.sol.create_random_params(new_centers, width)
		cdef np.ndarray[float, ndim = 1] ret
		ret = np.zeros((10,)).astype(np.single)

		cdef int i
		for i in range(10):
			ret[i] = arr[i]
		return ret
	
	cpdef float i_fast(self, params, int intervals):
		cdef float* ret = arr_to_ptr(10)
		cdef int i
		for i in range(10):
			ret[i] = params[i]
		
		return self.sol.i_fast(ret, intervals)
	
	cpdef optimize(self, int iterations, int num_starts, float target):
		cdef opVal ret = self.sol.optimize(iterations, num_starts, target)
		
		cdef np.ndarray[float, ndim = 1] params
		params = np.zeros((10,)).astype(np.single)

		cdef int i
		for i in range(10):
			params[i] = ret.params[i]

		return ret.c, params