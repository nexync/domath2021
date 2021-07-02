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

	def __cinit__(self, nf, np):
		self.sol = Solution(nf, np)

	cpdef double evaluate(self, params):
		cdef double** arr = arr_to_ptr_ptr(4)

		cdef double* a = arr_to_ptr(11)
		cdef double* b = arr_to_ptr(11)
		cdef double* c = arr_to_ptr(11)
		cdef double* d = arr_to_ptr(11)

		cdef int i
		for i in range(11):
			a[i] = params[0][i]
			b[i] = params[1][i]
			c[i] = params[2][i]
			d[i] = params[3][i]
		
		arr[0] = a
		arr[1] = b
		arr[2] = c
		arr[3] = d
		
		return self.sol.evaluate(arr)