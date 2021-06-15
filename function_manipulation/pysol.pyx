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

	def __cinit__(self):
		self.sol = Solution()

	cpdef evaluate(self, params, intervals):
		return self.sol.evaluate(params, intervals)