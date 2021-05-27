import numpy as np
cimport numpy as np
import random

cdef float f_x(np.ndarray[np.float64_t, ndim = 1] params, float x, float y):
	assert params.shape[0] == 6, 'Check number of parameters'
	return -params[0]*np.sin(x) + params[1]*np.cos(x) + params[4]*np.cos(x)*(np.cos(y)**2) - params[5]*np.sin(y)*np.sin(2*x)

cdef float f_y(np.ndarray[np.float64_t, ndim = 1] params, float x, float y):
	assert params.shape[0] == 6, 'Check number of parameters'
	return -params[2]*np.sin(y) + params[3]*np.cos(y) - params[4]*np.sin(x)*np.sin(2*y) + params[5]*np.cos(y)*(np.cos(x)**2)


cpdef float integrate_cython_fast(np.ndarray[np.float64_t, ndim = 2] params, int intervals = 500):
	assert params.shape[0] == 2, 'Check number of parameters'
	assert params.shape[1] == 6, 'Check number of parameters'
	
	params_a, params_b = params[0], params[1]
	
	#functions are
	#a = a1 cos(x) + a2 sin(x) + a3 cos(y) + a4 sin(y) + a5 sin(x) cos^2(y) + a6 sin(y) cos^2(x)
	#b = b1 cos(x) + b2 sin(x) + b3 cos(y) + b4 sin(y) + b5 sin(x) cos^2(y) + b6 sin(y) cos^2(x)
	
	#derivatives are
	#a_x = -a1 sin(x) + a2 cos(x) + a5 cos(x) cos^2(y) - a6 sin(y) sin(2x)
	#a_y = -a3 sin(y) + a4 cos(y) - a5 sin(x) sin(2y) + a6 cos(y) cos^2(x)
	#b_x = -b1 sin(x) + b2 cos(x) + b5 cos(x) cos^2(y) - b6 sin(y) sin(2x)
	#b_y = -b3 sin(y) + b4 cos(y) - b5 sin(x) sin(2y) + b6 cos(y) cos^2(x)
	xvals = np.linspace(-np.pi,np.pi,intervals)
	yvals = np.linspace(-np.pi,np.pi,intervals)
	
	cdef float lhs = 0
	cdef float rhs = 0
	cdef Py_ssize_t xind, yind

	for xind in range(intervals):
		for yind in range(intervals):
			dax = f_x(params_a,xvals[xind],yvals[yind])
			day = f_y(params_a,xvals[xind],yvals[yind])
			dbx = f_x(params_b,xvals[xind],yvals[yind])
			dby = f_y(params_b,xvals[xind],yvals[yind])
			
			ng2 = dax**2 + day**2 + dbx**2 + dby**2
			ng4 = ng2**2
			dg = dax*dby - day*dbx
			
			lhs += ng4
			rhs += ng2*dg
	if rhs == 0:
			return 0
	return lhs/rhs
