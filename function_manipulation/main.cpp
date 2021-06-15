#include "main.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>    // std::sort
#include <map>					//dictionary
#include <cmath>				//sin/cos
#include <fstream>			// ifstream and ofstream
#include <sstream> 			// std::istringstream

#include <chrono>

struct dummy_var {
	int subscript;
	char letter;

	bool operator<(const dummy_var& rhs) const {
		if (letter != rhs.letter) {
			return letter < rhs.letter;
		}
		else {
			return subscript < rhs.subscript;
		}
		return true;
	}

	bool operator==(const dummy_var& rhs) const {
		if (letter == rhs.letter && subscript == rhs.subscript)	return true;
		else	return false;
	}
};

struct coeff {
	int factor;
	std::vector<dummy_var> variables;
};

struct trig {
	std::string funct;
	int input_coeff;
	char input_var;
	int power;

	bool operator<(const trig& rhs) const {
		if (input_var != rhs.input_var) {
			return input_var < rhs.input_var;
		}
		else {
			if (input_coeff != rhs.input_coeff) {
				return input_coeff < rhs.input_coeff;
			}
			else {
				return funct < rhs.funct;
			}
		}
		return true;
	}

	bool operator==(const trig& rhs) const {
		if (funct == rhs.funct && input_coeff == rhs.input_coeff && input_var == rhs.input_var)	return true;
		else	return false;
	}
};

struct func {
	std::vector<coeff> scales;
	std::vector<trig> functions;
};

struct poly {
	std::vector<coeff> scales;
	double factor;
};

std::ofstream& operator<<(std::ofstream& os, const poly& p) {
	if (p.scales.size() > 0) {
		os << p.factor << " ";
		for (coeff c: p.scales) {
			os << c.factor << " ";
			for (dummy_var v: c.variables) {
				os << v.letter << "_" << v.subscript << "/";
			}
			os << " ";
		}
		os << "\n";
	}
	return os;
}



std::vector<dummy_var> dummyFromString(std::string inp) {
	std::vector<dummy_var> d;
	const std::string delimiter = "/";

	size_t pos = 0;
	std::string token;
	while ((pos = inp.find(delimiter)) != std::string::npos) {
		token = inp.substr(0, pos);

		char c = token[0];
		int subscript = atoi(token.substr(2).c_str());
		d.push_back({subscript, c	});	

		inp.erase(0, pos + delimiter.length());
	}
	return d;
}

poly polyFromString(std::string inp) {
	std::vector<coeff> scales;
	std::istringstream stm(inp);
	std::string s;
	stm >> s;
	double factor = atof(s.c_str());
	while(stm >> s) {
		int sc = atoi(s.c_str());
		stm >> s;
		std::vector<dummy_var> d = dummyFromString(s);
		scales.push_back({sc, d});
	}
	struct poly p {
		scales, factor
	};
	return p;
}

void show_func(func f) {
	if (f.scales.size() > 1) printf("(");
	for (int i = 0; i<f.scales.size(); i++) {
		if(abs(f.scales[i].factor) != 1) {
			printf("%d", f.scales[i].factor);
		}
		else if (f.scales[i].factor == -1) {
			printf("-");
		}
		for (dummy_var d: f.scales[i].variables) {
			printf("%c_%d", d.letter, d.subscript);
		}
		if (i != f.scales.size()-1)	printf(" + ");
	}
	if (f.scales.size() > 1) printf(")");
	

	for (int i = 0; i<f.functions.size(); i++) {
		if (f.functions[i].power > 1)	printf("%s^%d(%d%c)", f.functions[i].funct.c_str(), f.functions[i].power, f.functions[i].input_coeff, f.functions[i].input_var);
		else	printf("%s(%d%c)", f.functions[i].funct.c_str(), f.functions[i].input_coeff, f.functions[i].input_var);
	}
}

void show_func(std::vector<func> f) {
	for (int i = 0; i < f.size(); i++) {
		show_func(f[i]);
		if (i != f.size() - 1) {
			printf(" + ");
		}
	}
	printf("\n");
}

void show_poly(poly p) {
	printf("%f(", p.factor);
	for (int i = 0; i< p.scales.size(); i++) {
		coeff c = p.scales[i];
		printf("%d ", c.factor);
		for (dummy_var v: c.variables) {
			printf("%c_%d", v.letter, v.subscript);
		}
		if (i != p.scales.size() - 1)	printf(" + ");
	}
	printf(")");
}

bool func_comp(trig a, trig b) {
	if (a.input_var != b.input_var) {
		return a.input_var < b.input_var;
	}
	else {
		if (a.input_coeff != b.input_coeff) {
			return a.input_coeff < b.input_coeff;
		}
		else {
			return a.funct < b.funct;
		}
	}
	return true;
}

bool var_comp(dummy_var a, dummy_var b) {
	if (a.letter != b.letter) {
		return a.letter < b.letter;
	}
	else {
		return a.subscript < b.subscript;
	}
	return true;
}

bool equals(trig t, trig r) {
	if (t.funct == r.funct && t.input_coeff == r.input_coeff && t.input_var == r.input_var)	return true;
	else	return false;
}

std::vector<func> simplify(std::vector<func> f) { 
	typedef std::map<std::vector<dummy_var>, int> Dummy2Int;
	typedef std::map<std::vector<trig>, Dummy2Int> Trigs;


	//std::map<std::vector<trig>, std::map<std::vector<dummy_var>, int>> matches;

	Trigs matches;
	std::vector<func> ret;
	for(int i = 0; i<f.size(); i++) {
		std::vector<trig> temp = f[i].functions;

		Trigs::iterator iter = matches.find(temp);
		if (iter != matches.end()) {
			for (coeff scale: f[i].scales) {
				Dummy2Int::iterator iter2 = iter->second.find(scale.variables);
				if (iter2 != iter->second.end()) {
					iter2->second += scale.factor;
				}
				else { //key (of type vector<dummy_var>) not in map
					iter->second.insert(std::pair<std::vector<dummy_var>, int>(scale.variables, scale.factor));
				}
			}
		}
		else { //key (of type vector<trig>) not in map
			Dummy2Int temp_map;
			for (coeff scale: f[i].scales) {
				temp_map.insert(std::pair<std::vector<dummy_var>, int>(scale.variables, scale.factor));
			}
			matches.insert( std::pair<std::vector<trig> , Dummy2Int> (temp, temp_map) );
		}
	}

	for (Trigs::iterator iter = matches.begin(); iter != matches.end(); ++iter)
	{
		std::vector<coeff> new_coeff;
		for(Dummy2Int::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2) {
			new_coeff.push_back(
				{iter2->second, iter2->first}
			);
		}
		ret.push_back(
			{new_coeff, iter->first}
		);
	}

	return ret;
}

func multiply(func f, func g) {
	std::vector<trig> new_funcs;
	std::vector<coeff> new_coeffs;

	for (int i = 0; i<f.functions.size(); i++) {
		new_funcs.push_back(f.functions[i]);
	}
	for (int j = 0; j<g.functions.size(); j++) {
		for (int i = 0; i<f.functions.size(); i++) {
			if(new_funcs[i]==g.functions[j]) {
				new_funcs[i].power += g.functions[j].power;
				break;
			}
			if (i == f.functions.size()-1) {
				new_funcs.push_back(g.functions[j]);
			}
		}
	}

	for (coeff fscale: f.scales) {
		for (coeff gscale: g.scales) {
			int temp_factor = fscale.factor * gscale.factor;
			std::vector<dummy_var> temp_vars;
			for (dummy_var fdummy: fscale.variables) {
				temp_vars.push_back(fdummy);
			}
			for (dummy_var gdummy: gscale.variables) {
				temp_vars.push_back(gdummy);
			}
			std::sort(temp_vars.begin(), temp_vars.end(), var_comp);
			new_coeffs.push_back(
				{temp_factor, temp_vars}
			);
		}
	}

	std::sort(new_funcs.begin(), new_funcs.end(), func_comp);

	struct func fg {
 		new_coeffs, new_funcs
	};

	return fg;
}

std::vector<func> multiply(std::vector<func> f, func g) {
	std::vector<func> ret;
	for (int i = 0; i<f.size(); i++) {
		ret.push_back(multiply(f[i],g));
	}
	return ret;
}

std::vector<func> multiply(std::vector<func> f, std::vector<func> g) {
	std::vector<func> ret;
	for (int i = 0; i<f.size(); i++) {
		for(int j = 0; j<g.size(); j++) {
			ret.push_back(multiply(f[i],g[j]));
		}
	}
	return ret;
}

std::vector<func> square(std::vector<func> f) {
	std::vector<func> ret;
	for (int i = 0; i<f.size(); i++) {
		//std::cout << i << std::endl;
		for (int j = i; j<f.size(); j++) {
			if (i == j) {
				ret.push_back(multiply(f[i],f[j]));
			}
			else {
				func product = multiply(f[i],f[j]);
				for(int i = 0; i<product.scales.size(); i++) {
					product.scales[i].factor *= 2;
				}
				ret.push_back(product);
			}
		}
	}
	return ret;
}

std::vector<func> add(func f, func g) {
	return simplify(std::vector<func>{f, g});
}

std::vector<func> add(std::vector<func> f, func g) {
	f.push_back(g);
	return simplify(f);
}

std::vector<func> add(std::vector<func> f, std::vector<func> g) {
	for (func gf: g) {
		f.push_back(gf);
	}

	return simplify(f);
}

std::vector<func> subtract(func f, func g) {
	struct func alt_g = g;
	for (int i = 0; i<alt_g.scales.size(); i++) {
		alt_g.scales[i].factor *= -1;
	}
	return simplify(std::vector<func>{f, alt_g});
}

std::vector<func> subtract(std::vector<func> f, func g) {
	struct func alt_g = g;
	for (int i = 0; i<alt_g.scales.size(); i++) {
		alt_g.scales[i].factor *= -1;
	}
	f.push_back(alt_g);
	return simplify(f);
}

std::vector<func> subtract(std::vector<func> f, std::vector<func> g) {
	for (func gf : g) {
		struct func alt_g = gf;
		for (int i = 0; i<alt_g.scales.size(); i++) {
			alt_g.scales[i].factor *= -1;
		}
		f.push_back(alt_g);
	}
	return simplify(f);
}

std::vector<func> differentiate(func f) {
	std::vector<func> ret;
	if (f.functions.size() == 1) {
		if (f.functions[0].power == 1) {
			bool sin = f.functions[0].funct == "sin";

			std::vector<coeff> derivate_scales;
			for (coeff scale: f.scales) {
				int new_scale = sin ? scale.factor*f.functions[0].input_coeff : -1 * scale.factor*f.functions[0].input_coeff;
			
				struct coeff temp_coeff {
					new_scale, scale.variables
				};
				derivate_scales.push_back(temp_coeff);
			}

			std::string new_funct = sin ? "cos" : "sin";
			struct trig derivative{
				new_funct, f.functions[0].input_coeff, f.functions[0].input_var, f.functions[0].power
			};
			ret.push_back(
				{derivate_scales, std::vector<trig>{derivative}}
			);
		}
		else { //Defining here would be power rule with sin/cos
			printf("Not defined yet");
		}
	}
	else { //Defining here would be product rule with functions
		printf("Not defined yet");
	}
	return ret;
}

std::vector<func> differentiate(std::vector<func> f) {
	std::vector<func> ret;
	for (func fs: f) {
		std::vector<func> out = differentiate(fs);
		for (func op: out) {
			ret.push_back(op);
		}
	}
	return ret;
}

poly integrate(func f, int intervals) {
	double PI = 3.141592653587973;
	std::vector<double> vals;
	for (int i = 0; i<intervals; i++) {
		vals.push_back(-PI + (2*PI*i)/(intervals-1));
	}

	auto eval = [](std::vector<trig> inp, double pt) {
		if (inp.size() == 0) {
			return 2*3.141592653587973;
		}
		double ret = 1;
		for (trig func: inp) {
			if(func.funct == "sin") {
				ret *= pow(sin(func.input_coeff*pt),func.power);
			}
			else {
				ret *= pow(cos(func.input_coeff*pt),func.power);
			}
		}
		return ret;
	};

	std::vector<trig> x,y;
	for (trig func: f.functions) {
		if(func.input_var == 'x')	x.push_back(func);
		else	y.push_back(func);
	}

	double x_int, y_int = 0;
	for (double pt: vals) {
		x_int += eval(x,pt);
		y_int += eval(y,pt);
	}
	
	if (abs(x_int) <= 0.05 || abs(y_int) <= 0.05) {
		return {std::vector<coeff>{}, 0};
	}
	else {
		return {f.scales, (x_int * y_int)/((double)pow(intervals,2.0))};
	}
}

std::vector<poly> integrate(std::vector<func> f, int intervals) {
	std::vector<poly> ret;
	int i = 0;
	for (func fs: f) {
		if (i % 500 == 0)	printf("%d done out of %d\n", i, f.size());
		poly out = integrate(fs,intervals);
		if (out.factor != 0)	ret.push_back(out);
		i++;
	}
	return ret;
}

int main() {

	std::vector<func> f;
	std::vector<func> g;
	std::vector<func> u;
	std::vector<func> v;


	std::vector<std::string> fs{"cos", "sin"};
	std::vector<char> cs{'x', 'y'};


	//Declaration of f(x) below
	for(int i = 1; i <= 3; i++) {
		for (int j = 0; j<2; j++) {
			struct trig temp_t = {
				fs[j], i, 'x', 1
			};
			struct coeff temp_c = {
				1, std::vector<dummy_var>{{2*i+j-1, 'a'}}
			};
			struct func temp_f = {
				std::vector<coeff>{temp_c}, std::vector<trig>{temp_t}
			};
			f.push_back(temp_f);
		}
	}

	//Declaration of g(y) below
	for(int i = 1; i <= 3; i++) {
		for (int j = 0; j<2; j++) {
			struct trig temp_t = {
				fs[j], i, 'y', 1
			};
			struct coeff temp_c = {
				1, std::vector<dummy_var>{{2*i+j-1, 'b'}}
			};
			struct func temp_f = {
				std::vector<coeff>{temp_c}, std::vector<trig>{temp_t}
			};
			g.push_back(temp_f);
		}
	}

	//Declaration of u(x) below
	for(int i = 1; i <= 3; i++) {
		for (int j = 0; j<2; j++) {
			struct trig temp_t = {
				fs[j], i, 'x', 1
			};
			struct coeff temp_c = {
				1, std::vector<dummy_var>{{2*i+j-1, 'c'}}
			};
			struct func temp_f = {
				std::vector<coeff>{temp_c}, std::vector<trig>{temp_t}
			};
			u.push_back(temp_f);
		}
	}

	//Declaration of v(y) below
	for(int i = 1; i <= 3; i++) {
		for (int j = 0; j<2; j++) {
			struct trig temp_t = {
				fs[j], i, 'y', 1
			};
			struct coeff temp_c = {
				1, std::vector<dummy_var>{{2*i+j-1, 'd'}}
			};
			struct func temp_f = {
				std::vector<coeff>{temp_c}, std::vector<trig>{temp_t}
			};
			v.push_back(temp_f);
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<func> fx = differentiate(f);
	std::vector<func> gy = differentiate(g);
	std::vector<func> ux = differentiate(u);
	std::vector<func> vy = differentiate(v);

	std::vector<func> hx = simplify(multiply(fx, g));
	std::vector<func> hy = simplify(multiply(f, gy));
	std::vector<func> wx = simplify(multiply(ux, v));
	std::vector<func> wy = simplify(multiply(u, vy));

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	printf("done with derivatives and functions, elapsed time: %d ms \n", duration.count());
	printf("\n");

	start = std::chrono::high_resolution_clock::now();
	std::vector<func> n2 = add(add(multiply(hx,hx), multiply(hy,hy)), add(multiply(wx,wx), multiply(wy,wy)));
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	printf("done with n2, elapsed time: %d ms \n", duration.count());
	printf("size of n2: %d\n", n2.size());
	printf("\n");

	start = std::chrono::high_resolution_clock::now();
	std::vector<func> lhs = square(n2);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	printf("done with lhs, elapsed time: %d ms \n", duration.count());
	printf("size of lhs: %d\n", lhs.size());
	printf("\n");

	std::vector<poly> ret_left = integrate(lhs, 10000);
	printf("length of ret: %d\n", ret_left.size());

	std::ofstream outfile_left("lhs.txt");
	for (const poly savedata: ret_left) {
		outfile_left << savedata;
	}
	outfile_left.close();

	start = std::chrono::high_resolution_clock::now();
	std::vector<func> detg = subtract(multiply(hx, wy), multiply(hy,wx));
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	printf("done with detg, elapsed time: %d ms \n", duration.count());
	printf("size of detg: %d\n", detg.size());
	printf("\n");

	start = std::chrono::high_resolution_clock::now();
	std::vector<func> rhs = multiply(n2,detg);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	printf("done with rhs, elapsed time: %d ms \n", duration.count());
	printf("size of rhs: %d\n", rhs.size());
	

	std::vector<poly> ret_right = integrate(rhs, 10000);
	printf("length of ret: %d\n", ret_right.size());

	std::ofstream outfile_right("rhs.txt");
	for (const poly savedata: ret_right) {
		outfile_right << savedata;
	}
	outfile_right.close();


	// std::ifstream infile_test("hi.txt");
	// std::string temp;
	// while(std::getline(infile_test, temp)) {
	// 	poly p = polyFromString(temp);
	// 	show_poly(p);
	// }




	// show_func(hy);
	// printf("\n");
	// show_func(wx);
	// printf("\n");
	// show_func(wy);
	// printf("\n");

	// std::vector<func> fsmall;
	// fsmall.push_back(
	// 	{
	// 		std::vector<coeff>{{1, std::vector<dummy_var>{{1,'a'}}}}, std::vector<trig>{{"cos", 1, 'x', 1}}
	// 	}
	// );
	// fsmall.push_back(
	// 	{
	// 		std::vector<coeff>{{1, std::vector<dummy_var>{{2,'a'}}}}, std::vector<trig>{{"sin", 1, 'x', 2}}
	// 	}
	// );
	// fsmall.push_back(
	// 	{
	// 		std::vector<coeff>{{1, std::vector<dummy_var>{{3,'a'}}}}, std::vector<trig>{{"sin", 2, 'x', 1}}
	// 	}
	// );
	// show_func(fsmall);
	// std::vector<poly> out = integrate(fsmall,10000);

	// for (poly p: out) {
	// 	printf("%f", p.factor);
	// }

	return 0;
}


