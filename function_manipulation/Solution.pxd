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
	cdef cppclass Solution:
		Solution()
		Solution(int check)
		double evaluate(double** params)