import math
import random
from sympy import *
from tqdm import tqdm


if __name__ == '__main__':
    # For reference, we have vector-valued input function u = [a b]

    # Choose number of starting points and iterations per starting point
    iterations = 200
    starting_points = 10

    max_c = 999999999

    max_a_1 = 0
    max_b_1 = 0
    max_c_1 = 0
    max_d_1 = 0
    max_e_1 = 0
    max_f_1 = 0

    max_a_2 = 0
    max_b_2 = 0
    max_c_2 = 0
    max_d_2 = 0
    max_e_2 = 0
    max_f_2 = 0

    max_a = 0
    max_b = 0
    max_right_hand_integral = 0
    max_left_hand_integral = 0

    # Functions:
    # a = a_1 * cos(x) + b_1 * sin(x) + c_1 * cos(y) + d_1 * sin(y) + e_1*sin(x)*(cos(y)**2) + f_1*sin(y)*(cos(x)**2)
    # b = a_2 * cos(x) + b_2 * sin(x) + c_2 * cos(y) + d_2 * sin(y) + e_2*sin(x)*(cos(y)**2) + f_2*sin(y)*(cos(x)**2)

    for j in tqdm(range(starting_points), desc='Starting points optimized: ', position=0,):
        # Select center and radius of interval
        center = 0
        # Reset width for each new starting point
        width = 1
        # Choose random starting parameter values from the given interval
        a_1_start = random.uniform(center - width, center + width)
        b_1_start = random.uniform(center - width, center + width)
        c_1_start = random.uniform(center - width, center + width)
        d_1_start = random.uniform(center - width, center + width)
        e_1_start = random.uniform(center - width, center + width)
        f_1_start = random.uniform(center - width, center + width)

        a_2_start = random.uniform(center - width, center + width)
        b_2_start = random.uniform(center - width, center + width)
        c_2_start = random.uniform(center - width, center + width)
        d_2_start = random.uniform(center - width, center + width)
        e_2_start = random.uniform(center - width, center + width)
        f_2_start = random.uniform(center - width, center + width)

        # Reset temporary optimal parameters for each new starting point
        temp_max_c = 10
        temp_max_a = 0
        temp_max_b = 0
        temp_max_right_hand_integral = 0
        temp_max_left_hand_integral = 0

        temp_max_a_1 = a_1_start
        temp_max_b_1 = b_1_start
        temp_max_c_1 = c_1_start
        temp_max_d_1 = d_1_start
        temp_max_e_1 = e_1_start
        temp_max_f_1 = f_1_start

        temp_max_a_2 = a_2_start
        temp_max_b_2 = b_2_start
        temp_max_c_2 = c_2_start
        temp_max_d_2 = d_2_start
        temp_max_e_2 = e_2_start
        temp_max_f_2 = f_2_start

        for i in range(iterations):
            # Set new random parameter values for each iteration using the current (updating) width and center
            a_1 = random.uniform(a_1_start - width, a_1_start + width)
            b_1 = random.uniform(b_1_start - width, b_1_start + width)
            c_1 = random.uniform(c_1_start - width, c_1_start + width)
            d_1 = random.uniform(d_1_start - width, d_1_start + width)
            e_1 = random.uniform(e_1_start - width, e_1_start + width)
            f_1 = random.uniform(f_1_start - width, f_1_start + width)

            a_2 = random.uniform(a_2_start - width, a_2_start + width)
            b_2 = random.uniform(b_2_start - width, b_2_start + width)
            c_2 = random.uniform(c_2_start - width, c_2_start + width)
            d_2 = random.uniform(d_2_start - width, d_2_start + width)
            e_2 = random.uniform(e_2_start - width, e_2_start + width)
            f_2 = random.uniform(f_2_start - width, f_2_start + width)

            # Define our input function u = [a b]
            # IMPORTANT PART *********!!!!!!!!!!!!!!!!!!!!!!!!
            x, y = symbols('x y', real=True)
            a = a_1 * cos(x) + b_1 * sin(x) + c_1 * cos(y) + d_1 * sin(y) + e_1*sin(x)*(cos(y)**2) + \
                f_1*sin(y)*(cos(x)**2)
            b = a_2 * cos(x) + b_2 * sin(x) + c_2 * cos(y) + d_2 * sin(y) + e_2*sin(x)*(cos(y)**2) + \
                f_2*sin(y)*(cos(x)**2)

            # Calculate interior of right-hand integral
            det_del_u = diff(a, x)*diff(b, y) - diff(a, y)*diff(b, x)
            norm_del_u_squared = diff(a, x)*diff(a, x) + diff(a, y)*diff(a, y) + diff(b, x)*diff(b, x) \
                                 + diff(b, y)*diff(b, y)

            # Calculate interior of left-hand integral
            norm_del_u_fourth = norm_del_u_squared * norm_del_u_squared

            # Compute left and right hand integrals
            right_hand_integral = N(integrate(det_del_u * norm_del_u_squared, (x, -pi, pi), (y, -pi, pi)))
            left_hand_integral = N(integrate(norm_del_u_fourth, (x, -pi, pi), (y, -pi, pi)))

            if right_hand_integral == 0:
                continue
            else:
                temp_c = left_hand_integral/right_hand_integral
                if temp_c < temp_max_c and temp_c > 0:
                    temp_max_c = temp_c
                    temp_max_a_1 = a_1
                    temp_max_b_1 = b_1
                    temp_max_c_1 = c_1
                    temp_max_d_1 = d_1
                    temp_max_e_1 = e_1
                    temp_max_f_1 = f_1

                    temp_max_a_2 = a_2
                    temp_max_b_2 = b_2
                    temp_max_c_2 = c_2
                    temp_max_d_2 = d_2
                    temp_max_e_2 = e_2
                    temp_max_f_2 = f_2

                    temp_max_right_hand_integral = right_hand_integral
                    temp_max_left_hand_integral = left_hand_integral
                    temp_max_a = a
                    temp_max_b = b

                    # Update the center of each parameter to be the current best optimal values for the current
                    # starting point
                    a_1_start = a_1
                    b_1_start = b_1
                    c_1_start = c_1
                    d_1_start = d_1
                    e_1_start = e_1
                    f_1_start = f_1

                    a_2_start = a_2
                    b_2_start = b_2
                    c_2_start = c_2
                    d_2_start = d_2
                    e_2_start = e_2
                    f_2_start = f_2

                    # Update width to be smaller based on the maximum of the differences between the new optimal
                    # parameters and the center of each (this is necessarily smaller because the parameters are
                    # at a distance of at most 'width' from their centers)
                    width = width + max(abs(a_1 - a_1_start), abs(b_1 - b_1_start), abs(c_1 - c_1_start),
                                         abs(d_1 - d_1_start) , abs(e_1 - e_1_start), abs(f_1 - f_1_start),
                                         abs(a_2 - a_2_start), abs(b_2 - b_2_start), abs(c_2 - c_2_start),
                                         abs(d_2 - d_2_start), abs(e_2 - e_2_start), abs(f_2 - f_2_start))
        # After optimizing for our current starting point, we see if we have found a new optimal value of c across
        # all starting points
        if temp_max_c < max_c:
            max_c = temp_max_c

            max_a_1 = temp_max_a_1
            max_b_1 = temp_max_b_1
            max_c_1 = temp_max_c_1
            max_d_1 = temp_max_d_1
            max_e_1 = temp_max_e_1
            max_f_1 = temp_max_f_1

            max_a_2 = temp_max_a_2
            max_b_2 = temp_max_b_2
            max_c_2 = temp_max_c_2
            max_d_2 = temp_max_d_2
            max_e_2 = temp_max_e_2
            max_f_2 = temp_max_f_2

            max_a = temp_max_a
            max_b = temp_max_b
            max_right_hand_integral = temp_max_right_hand_integral
            max_left_hand_integral = temp_max_left_hand_integral

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
    print(max_a)
    print('b = ', end='')
    print(max_b)
    print('')

    print('Right-hand integral value: ', end='')
    print(max_right_hand_integral)
    print('Left-hand integral value: ', end='')
    print(max_left_hand_integral)
    print('')

    print('Largest possible value of c under the given parameters: ', end='')
    if max_c > 999999:
        print('N/A  (right-hand integral is always 0, any value of c will do)')
    else:
        print(max_c)
