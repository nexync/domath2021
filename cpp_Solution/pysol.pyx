# distutils: language = c++

from Solution cimport Solution

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class PySolution:
	cdef Solution sol  # Hold a C++ instance which we're wrapping

	def __cinit__(self):
		self.sol = Solution()

	cpdef create_random_params(self):
		return self.sol.create_random_params()

	cpdef float f_x(self, params, float x, float y):
		return self.sol.f_x(params, x, y)

	cpdef float f_y(self, params, float x, float y):
		return self.sol.f_y(params, x, y)

	cpdef float integrate(self, params, int intervals):
		self.sol.integrate(params, intervals)