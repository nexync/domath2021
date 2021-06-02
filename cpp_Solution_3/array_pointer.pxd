cdef extern from "array_pointer.cpp":
	pass

#needed functions for pysol.pyx file
cdef extern from "array_pointer.h":
	double* arr_to_ptr(int m)
	double** arr_to_ptr_ptr(int m)