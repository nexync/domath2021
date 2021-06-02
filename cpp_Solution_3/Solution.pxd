cdef extern from "Solution.cpp":
	pass

# Declare the class with cdef
cdef extern from "Solution.h":
	cdef struct opVal:
		double c
		double** params
	cdef struct retStruct:
		int size
		opVal* values
	cdef cppclass Solution:
		Solution() except +
		double** create_random_params(double** centers, double width)
		double i_fast(double** params, int intervals)
		double check_integral(double** params, int intervals)
		retStruct optimize(int iterations, int num_starts, double target)