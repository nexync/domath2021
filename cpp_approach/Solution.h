#ifndef SOLUTION_H
#define SOLUTION_H

class Solution {
	public:
		Solution();
		float** create_random_params();
		float f_x(float* params, float x, float y);
		float f_y(float* params, float x, float y);
		float i_fast(float** params, int intervals);
};

#endif