#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include <string>

struct dummy_var;
struct coeff;
struct poly;
struct param_dict;
struct ret_triple;

class Solution {
	public:
		Solution();
		Solution(int, int);
		std::vector<poly> lhs;
		std::vector<poly> rhs;
		int nf;
		int np;
		std::vector<dummy_var> dummyFromString(std::string);
		poly polyFromString(std::string);
		double evaluate(double**);
		double** gradient(double**);
		ret_triple steepest_descent(double**, double, double, int);
};

#endif