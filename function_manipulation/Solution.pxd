cdef extern from "Solution.cpp":
	pass

# Declare the class with cdef
cdef extern from "Solution.h":
	cdef struct dummy_var:
		int subscript
		char letter
	cdef struct coeff:
		int factor
		dummy_var* variables
	cdef struct poly:
		coeff* scales
		double factor

	cdef struct param_dict:
		double alpha
		double** v0
		double mu

	cdef struct ret_triple:
		double** point
		double res
		param_dict state

	cdef cppclass Solution:
		Solution()
		Solution(int, int)
		double evaluate(double**)
		ret_triple steepest_descent(double**, double, double, int)