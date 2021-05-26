cdef extern from "Solution.cpp":
	pass

# Declare the class with cdef
cdef extern from "Solution.h":
	cdef cppclass Solution:
		Solution() except +
		float[:,;] create_random_params()
		float f_x(float[:] params, float x, float y)
		float f_y(float[:] params, float x, float y)
		float integrate(float[:,:] params, int intervals)