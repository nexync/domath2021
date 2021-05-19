import math
import random
from sympy import *


if __name__ == '__main__':
    """Call run_posterior(), do not modify"""
    center = 0
    width = 1
    iterations = 30
    max_c = 999999999

    max_a_1 = center
    max_b_1 = center
    max_c_1 = center
    max_d_1 = center
    max_e_1 = center
    max_f_1 = center

    max_a_2 = center
    max_b_2 = center
    max_c_2 = center
    max_d_2 = center
    max_e_2 = center
    max_f_1 = center

    # Set our parameter variables
    # a = a_1 * cos(x) + b_1 * sin(x) + c_1 * cos(y) + d_1 * sin(y) + e_1*sin(x)*(cos(y)**2) + f_1*sin(y)*(cos(x)**2)
    # b = a_2 * cos(x) + b_2 * sin(x) + c_2 * cos(y) + d_2 * sin(y) + e_2*sin(x)*(cos(y)**2) + f_2*sin(y)*(cos(x)**2)
    for i in range(iterations):
        a_1 = random.uniform(center - width, center + width)
        b_1 = random.uniform(center - width, center + width)
        c_1 = random.uniform(center - width, center + width)
        d_1 = random.uniform(center - width, center + width)
        e_1 = random.uniform(center - width, center + width)
        f_1 = random.uniform(center - width, center + width)

        a_2 = random.uniform(center - width, center + width)
        b_2 = random.uniform(center - width, center + width)
        c_2 = random.uniform(center - width, center + width)
        d_2 = random.uniform(center - width, center + width)
        e_2 = random.uniform(center - width, center + width)
        f_2 = random.uniform(center - width, center + width)

        # Define our input function u = [a b]
        # IMPORTANT PART *********!!!!!!!!!!!!!!!!!!!!!!!!
        x, y = symbols('x y', real=True)
        a = a_1 * cos(x) + b_1 * sin(x) + c_1 * cos(y) + d_1 * sin(y) + e_1*sin(x)*(cos(y)**2) + f_1*sin(y)*(cos(x)**2)
        b = a_2 * cos(x) + b_2 * sin(x) + c_2 * cos(y) + d_2 * sin(y) + e_2*sin(x)*(cos(y)**2) + f_2*sin(y)*(cos(x)**2)

        # Calculate interior of right-hand integral
        det_del_u = diff(a, x)*diff(b, y) - diff(a, y)*diff(b, x)
        norm_del_u_squared = diff(a, x)*diff(a, x) + diff(a, y)*diff(a, y) + diff(b, x)*diff(b, x) + diff(b, y)*diff(b, y)

        # Calculate interior of left-hand integral
        norm_del_u_fourth = norm_del_u_squared * norm_del_u_squared

        # Compute integral
        right_hand_integral = N(integrate(det_del_u * norm_del_u_squared, (x, -pi, pi), (y, -pi, pi)))
        left_hand_integral = N(integrate(norm_del_u_fourth, (x, -pi, pi), (y, -pi, pi)))

        if right_hand_integral == 0:
            continue
        else:
            temp_c = left_hand_integral/right_hand_integral
        if temp_c < max_c and temp_c > 0:
            max_c = temp_c
            max_a_1 = a_1
            max_b_1 = b_1
            max_c_1 = c_1
            max_d_1 = d_1
            max_e_1 = e_1
            max_f_1 = f_1

            max_a_2 = a_2
            max_b_2 = b_2
            max_c_2 = c_2
            max_d_2 = d_2
            max_e_2 = e_2
            max_f_2 = f_2

    # Print output
    print('Parameters achieving optimal c: ')
    print('a_1 = ', end='')
    print(max_a_1, end=', ')
    print('b_1 = ', end='')
    print(max_b_1, end=', ')
    print('c_1 = ', end='')
    print(max_c_1, end=', ')
    print('d_1 = ', end='')
    print(max_d_1, end=', ')
    print('e_1 = ', end='')
    print(max_e_1, end=', ')
    print('f_1 = ', end='')
    print(max_f_1)

    print('a_2 = ', end='')
    print(max_a_2, end=', ')
    print('b_2 = ', end='')
    print(max_b_2, end=', ')
    print('c_2 = ', end='')
    print(max_c_2, end=', ')
    print('d_2 = ', end='')
    print(max_d_2, end=', ')
    print('e_2 = ', end='')
    print(max_e_2, end=', ')
    print('f_2 = ', end='')
    print(max_f_2)
    print('')

    print('Functions:')
    print('a = ', end='')
    print(a)
    print('b = ', end='')
    print(b)
    print('')

    print('Right-hand integral value: ', end='')
    print(right_hand_integral)
    print('Left-hand integral value: ', end='')
    print(left_hand_integral)
    print('')

    print('Largest possible value of c under the given parameters: ', end='')
    if max_c > 999999:
        print('N/A  (right-hand integral is always 0, any value of c will do)')
    else:
        print(max_c)
