# distutils: language = c++

import numpy as np
cimport numpy as np

np.import_array()

from Solution cimport Solution
from Solution cimport dummy_var
from Solution cimport coeff
from Solution cimport poly
from array_pointer cimport *

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.

cdef class PySolution:
	cdef Solution sol  # Hold a C++ instance which we're wrapping
	cdef int nf, np

	def __cinit__(self, num_f, num_p):
		self.sol = Solution(num_f,num_p)
		self.nf = num_f
		self.np = num_p

	cpdef double evaluate(self, params):
		cdef double** arr = arr_to_ptr_ptr(self.nf)
		cdef double* temp
		cdef int i,j
		for i in range(self.nf):
			temp = arr_to_ptr(self.np)
			for j in range(self.np):
				temp[j] = params[i][j]
			arr[i] = temp

		return self.sol.evaluate(arr)