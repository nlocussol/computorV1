#ifndef COMPUTOR_HPP
#define COMPUTOR_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cmath>

#define DELIMITER "\n\r\t "
#define NOT_VARIABLE -1
#define ABS(X) (X * ((X > 0) - (X < 0)))
#define INVERSE(X) (X * -1)

enum e_computor
{
    NUMBER = 1,
    MULTIPLICATION,
    OPERATOR,
    VARIABLE,
    EQUAL
};

class Computor
{
private:
    uint16_t _degree;
    float _discriminant;
    std::vector<std::string> _equation;
    std::string _reduceForm;
    void resolveFirstDegree();
    void resolveSecondDegree();
    void specialCases();
    float getReduceNumber(int head, float result, bool equal);
    void printReduceForm(float *reduceTab);

public:
    Computor();
    ~Computor();
    void parsEquation(std::string &equation);
    void getReduceForm();
    void getSolution();
};

std::vector<std::string> mysplit(std::string &, std::string delimiter);

#endif