#ifndef MAIN_H
#define MAIN_H

class Approximator {
	public:
		Approximator();
		double Approximator::parabola_eval(double, double, double);
		std::vector<double> Approximator::parabola_grad(double, double, double);
		std::vector<double> Approximator::oscillate_eval(double, double, double);
		std::vector<double> Approximator::oscillate_grad(double, double, double);
		std::vector<double> Approximator::func_eval(double, double, double, double);
};

#endif