#ifndef SOLUTION_H
#define SOLUTION_H

struct opVal;

class Solution {
	public:
		Solution();
		float curWidth;
		float** curCenters;
		void setWidth(float width);
		void setCenters(float** centers);
		float** create_random_params(float** centers, float width);
		float f_x(float* params, float x, float y);
		float f_y(float* params, float x, float y);
		float i_fast(float** params, int intervals);
		opVal optimize(int iterations, int num_starts, float target);
};

#endif