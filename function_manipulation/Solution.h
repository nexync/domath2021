#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include <string>

struct dummy_var;
struct coeff;
struct poly;

class Solution {
	public:
		Solution();
		Solution(int check);
		std::vector<poly> lhs;
		std::vector<poly> rhs;
		double evaluate(double** params);
		std::vector<dummy_var> dummyFromString(std::string);
		poly polyFromString(std::string);
};

#endif