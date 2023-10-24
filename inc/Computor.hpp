#ifndef COMPUTOR_HPP
#define COMPUTOR_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "utils.hpp"

#define DELIMITER "\n\r\t "
#define SYMBOL "+-*="
#define NOT_VARIABLE -1
#define ABS(X) (X * ((X > 0) - (X < 0)))
#define INVERSE(X) (X * -1)
#define VAR_1 0
#define VAR_2 4
#define VAR_3 8

class Computor
{
private:
    uint16_t _degree = 0;
    float _discriminant;
    std::vector<std::string> _equation;
    std::string _reduceForm;

    void resolveFirstDegree();
    void resolveSecondDegree();
    void specialCases();
    float getReduceNumber(int head, float result, bool equal);
    void printReduceForm(float *reduceTab);
    std::string equationFormatting(std::string &equation);
    void formatSplit();
    void checkStructure();
    void printFraction(float numerator, float quotient);
    void discriminantCase(float a, float b);
    
public:
    Computor();
    ~Computor();
    void parsEquation(std::string &equation);
    void getReduceForm();
    void getSolution();
};



#endif