import numpy as np
import time
import random

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
    return -params[3]*np.sin(y) + params[4]*np.cos(y) - params[5]*np.sin(x)*np.sin(2*y) + params[6]*np.cos(y)*(np.sin(x)**2)

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

def integrate(params):
    assert params.shape == (2,7), 'Check number of parameters'
    
    params_a, params_b = params[0], params[1]
    #functions are
    #a = a1 cos(x) + a2 sin(x) + a3 cos(y) + a4 sin(y) + a5 sin(x) cos^2(y) + a6 sin(y) cos^2(x)
    #b = b1 cos(x) + b2 sin(x) + b3 cos(y) + b4 sin(y) + b5 sin(x) cos^2(y) + b6 sin(y) cos^2(x)
    
    #derivatives are
    #a_x = -a1 sin(x) + a2 cos(x) + a5 cos(x) cos^2(y) - a6 sin(y) sin(2x)
    #a_y = -a3 sin(y) + a4 cos(y) - a5 sin(x) sin(2y) + a6 cos(y) sin^2(x)
    #b_x = -b1 sin(x) + b2 cos(x) + b5 cos(x) cos^2(y) - b6 sin(y) sin(2x)
    #b_y = -b3 sin(y) + b4 cos(y) - b5 sin(x) sin(2y) + b6 cos(y) sin^2(x)
    xvals = np.linspace(-np.pi,np.pi,1000)
    yvals = np.linspace(-np.pi,np.pi,1000)
    
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
    return lhs/rhs

if __name__ == '__main__':
	start = time.perf_counter()
	rparams = create_random_params()
	c = integrate(rparams)
	end = time.perf_counter()
	print(end - start)
	print(c)