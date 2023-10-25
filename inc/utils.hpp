#ifndef UTILS_HPP
# define UTILS_HPP
#include "Computor.hpp"

std::vector<std::string> mysplit(std::string &, std::string delimiter);
bool isFloat(std::string &str);
int GCD(int numerator, int quotient);
int countDecimal(float number);
int isVariable(std::string &variable);
bool isOperator(std::string & str);

#endif