#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <string>


struct dummy_var;
struct coeff;
struct trig;
struct func;
struct poly;

void show_func(func);
void show_func(std::vector<func>);
void show_poly(poly);
bool func_comp(trig, trig);
std::vector<dummy_var> dummyFromString(std::string);
poly polyFromString(std::string);
std::vector<func> simplify(std::vector<func>);
func multiply(func, func);
std::vector<func> multiply(std::vector<func>, func);
std::vector<func> multiply(std::vector<func>, std::vector<func>);
std::vector<func> square(std::vector<func>);
std::vector<func> add(func, func);
std::vector<func> add(std::vector<func>, func);
std::vector<func> add(std::vector<func>, std::vector<func>);
std::vector<func> subtract(func, func);
std::vector<func> subtract(std::vector<func>, func);
std::vector<func> subtract(std::vector<func>, std::vector<func>);
std::vector<func> differentiate(func);
std::vector<func> differentiate(std::vector<func>);
poly integrate(func, int);
std::vector<poly> integrate(std::vector<func>, int);

#endif