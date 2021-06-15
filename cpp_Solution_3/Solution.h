#ifndef SOLUTION_H
#define SOLUTION_H


struct opVal;
struct retStruct;

class Solution {
	public:
		Solution();
		double curWidth;
		double** curCenters;
		void setWidth(double width);
		void setCenters(double** centers);
		double** create_random_params(double** centers, double width);
		double a_x(double* params, double x, double y);
		double a_y(double* params, double x, double y);
		double b_x(double* params, double x, double y);
		double b_y(double* params, double x, double y);
		double eval_func(double* params, double x);
		double eval_func_prime(double* params, double x);
		double check_integral(double** params, int intervals);
		double i_fast(double** params, int intervals);
		retStruct optimize(int iterations, int num_starts, double target);
		opVal efficient_optimize(double** params, int max_iters, double threshold, double initial_width);
};

#endif