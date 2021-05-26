import numpy as np
import time
import random
from sympy import *
from tqdm import tqdm

import intspeed

def create_random_params():
	ret = np.zeros((2,7))
	for i in range(2):
		for j in range(1,7):
			ret[i][j] = random.uniform(-1,1)
	return ret

def f_x(params,x,y):
	assert params.shape == (7,), 'Check number of parameters'
	return -params[1]*np.sin(x) + params[2]*np.cos(x) + params[5]*np.cos(x)*(np.cos(y)**2) - params[6]*np.sin(y)*np.sin(2*x)

def f_y(params,x,y):
	assert params.shape == (7,), 'Check number of parameters'
	return -params[3]*np.sin(y) + params[4]*np.cos(y) - params[5]*np.sin(x)*np.sin(2*y) + params[6]*np.cos(y)*(np.cos(x)**2)

# def norm_grad_2(params,x,y):
#     assert params.shape == (2,7), 'Check number of parameters'
#     params_a, params_b = params[0], params[1]
    
#     return f_x(params_a,x,y)**2 + f_y(params_a,x,y)**2 + f_x(params_b,x,y)**2 + f_y(params_b,x,y)**2 

# def det_grad(params,x,y):
#     assert params.shape == (2,7), 'Check number of parameters'
#     params_a, params_b = params[0], params[1]
    
#     return f_x(params_a,x,y)*f_y(params_b,x,y) - f_x(params_b,x,y)*f_y(params_a,x,y)

# def norm_grad_4(params,x,y):
#     return norm_grad_2(params,x,y)**2
def integrate_sympy_new(params):
    assert params.shape == (2,7), 'Check number of parameters'
    params_a, params_b = params[0],params[1]
    x, y = symbols('x y', real=True)
    dax = -params_a[1]*sin(x) + params_a[2]*cos(x) + params_a[5]*cos(x)*(cos(y)**2) - 2*params_a[6]*sin(y)*sin(x)*cos(x)
    day = -params_a[3]*sin(y) + params_a[4]*cos(y) - 2* params_a[5]*sin(x)*sin(y)*cos(y) + params_a[6]*cos(y)*(cos(x)**2)


    dbx = -params_b[1]*sin(x) + params_b[2]*cos(x) + params_b[5]*cos(x)*(cos(y)**2) - 2*params_b[6]*sin(y)*sin(x)*cos(x)
    dby = -params_b[3]*sin(y) + params_b[4]*cos(y) - 2*params_b[5]*sin(x)*sin(y)*cos(y) + params_b[6]*cos(y)*(cos(x)**2)
    
    ng2 = dax**2 + day**2 + dbx**2 + dby**2
    ng4 = ng2**2
    dg = dax*dby - day*dbx
    
    rhs = N(integrate(ng2*dg, (x, -pi, pi), (y, -pi, pi)))
    lhs = N(integrate(ng4, (x, -pi, pi), (y, -pi, pi)))
    if rhs == 0:
        return 0
    else:
        return lhs/rhs

def integrate_sympy_old(params):
    assert params.shape == (2,7), 'Check number of parameters'
    params_a, params_b = params[0],params[1]
    x, y = symbols('x y', real=True)
    
    a = params_a[1] * cos(x) + params_a[2] * sin(x) + params_a[3] * cos(y) + params_a[4] * sin(y) + params_a[5]*sin(x)*(cos(y)**2) + \
        params_a[6]*sin(y)*(cos(x)**2)
    b = params_b[1] * cos(x) + params_b[2] * sin(x) + params_b[3] * cos(y) + params_b[4] * sin(y) + params_b[5]*sin(x)*(cos(y)**2) + \
        params_b[6]*sin(y)*(cos(x)**2)

    # Calculate interior of right-hand integral
    det_del_u = diff(a, x)*diff(b, y) - diff(a, y)*diff(b, x)
    norm_del_u_squared = diff(a, x)*diff(a, x) + diff(a, y)*diff(a, y) + diff(b, x)*diff(b, x) \
                         + diff(b, y)*diff(b, y)

    # Calculate interior of left-hand integral
    norm_del_u_fourth = norm_del_u_squared * norm_del_u_squared

    # Compute left and right hand integrals
    rhs = N(integrate(det_del_u * norm_del_u_squared, (x, -pi, pi), (y, -pi, pi)))
    lhs = N(integrate(norm_del_u_fourth, (x, -pi, pi), (y, -pi, pi)))
    
    if rhs == 0:
        return 0
    else:
        return lhs/rhs  

def integrate_fast(params, intervals = 500):
	assert params.shape == (2,7), 'Check number of parameters'
	
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
	
	lhs = 0
	rhs = 0
	
	for x in xvals:
		for y in yvals:
			dax = f_x(params_a,x,y)
			day = f_y(params_a,x,y)
			dbx = f_x(params_b,x,y)
			dby = f_y(params_b,x,y)
			
			ng2 = dax**2 + day**2 + dbx**2 + dby**2
			ng4 = ng2**2
			dg = dax*dby - day*dbx
			
			lhs += ng4
			rhs += ng2*dg
	if rhs == 0:
		return 0
	return lhs/rhs

if __name__ == '__main__':
	#fcts = [integrate_sympy_new, integrate_fast]

	arr = []

	for k in tqdm(range(10)):
		rparams = create_random_params()
		start = time.perf_counter()
		c1 = intspeed.integrate_cython_fast(rparams)
		end = time.perf_counter()
		print('cython', end - start)


		start = time.perf_counter()
		c2 = integrate_fast(rparams)
		end = time.perf_counter()
		print('fast',end - start)

		print(c1, c2)
        