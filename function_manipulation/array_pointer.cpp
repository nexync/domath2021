#include "array_pointer.h"

double* arr_to_ptr(int m) {
	double* ret = new double[m];
	return ret;
}

double** arr_to_ptr_ptr(int m) {
	double** ret = new double*[m];
	return ret;
}