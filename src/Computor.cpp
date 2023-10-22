#include "../inc/Computor.hpp"

Computor::Computor() {}

Computor::~Computor() {}

int isVariable(std::string &variable)
{
    if (variable[0] != 'X' || variable[1] != '^')
        return NOT_VARIABLE;
    char *endPtr;
    int number = strtod(&variable.c_str()[2], &endPtr);
    if (strlen(endPtr))
        return NOT_VARIABLE;
    return number;
}

std::string Computor::equationFormatting(std::string & equation){
    std::stringstream result;
    for (int i = 0; equation[i]; i++) {
        if (strchr(SYMBOL, equation[i]))
            result << " " << equation[i] << " ";
        else if (i > 0 && equation[i] == 'X' && equation[i - 1] != ' ')
            result << " " << equation[i];
        else
            result << equation[i];
    }
    return result.str();
}

void Computor::formatSplit() {
    for (ulong i = 0; i < _equation.size(); i++) {
        if (_equation[i] == "X")
            _equation[i] = "X^1";
        if (isVariable(_equation[i]) >= 0 && (i == 0 || _equation[i - 1] != "*")) {
            _equation.insert(_equation.begin() + i, "*");
            if (!isFloat(_equation[i - 1]))
                _equation.insert(_equation.begin() + i, "1");
        }
    }
}

void Computor::parsEquation(std::string &equation)
{
    int degree;
    equation = equationFormatting(equation);
    _equation = mysplit(equation, DELIMITER);
    formatSplit();
    for (ulong i = 0; i < _equation.size(); i++)
    {
        if (isFloat(_equation[i])) continue;
        else if (_equation[i] == "*") continue;
        else if (_equation[i] == "=") continue;
        else if (_equation[i] == "-" || _equation[i] == "+") {
            if (i > 0 && isFloat(_equation[i - 1])) {
                _equation.insert(_equation.begin() + i, "X^0");
                _equation.insert(_equation.begin() + i, "*");
            }
        }
        else if ((degree = isVariable(_equation[i])) >= 0)
            _degree = degree > _degree ? degree : _degree;
        else
            throw(std::logic_error("Bad input: " + _equation[i]));
    }
    if (isFloat(_equation[_equation.size() - 1])) {
        _equation.push_back("*");
        _equation.push_back("X^0");
    }
    if (std::count(_equation.begin(), _equation.end(), "=") != 1)
        throw(std::logic_error("Bad input: an equation must contain only one equality!"));
}

void Computor::getReduceForm()
{
    bool equal;
    float reduceNumber;
    float reduceTab[_degree];
    int eqSize = _equation.size();
    for (int eqCount = 0; eqCount <= _degree; eqCount++)
    {
        reduceNumber = 0;
        equal = false;
        for (int head = 0; head < eqSize; head++)
        {
            if (_equation[head] == "=")
                equal = true;
            else if (isVariable(_equation[head]) >= 0 && _equation[head] == "X^" + std::to_string(eqCount))
                reduceNumber = getReduceNumber(head - 2, reduceNumber, equal);
        }
        reduceTab[eqCount] = reduceNumber;
    }
    printReduceForm(reduceTab);
}


float Computor::getReduceNumber(int head, float result, bool equal)
{
    float number = stof(_equation[head]);
    if (head < 1)
        return number;
    std::string symbol = _equation[head - 1];
    if (symbol == "-")
        result = equal ? result + number : result - number;
    else if (symbol == "+" || symbol == "=")
        result = equal ? result - number : result + number;
    else
        throw(std::logic_error("Bad input!"));
    return result;
}

void Computor::printReduceForm(float *reduceTab) {
    float reduceNumber;
    std::stringstream reduceForm;
    for (int degree = 0; degree <= _degree; degree++) {
        reduceNumber = reduceTab[degree];
        if (degree == 0)
            reduceForm << reduceNumber;
        else if (reduceNumber > 0)
            reduceForm << "+ " << reduceNumber;
        else
            reduceForm << "- " << ABS(reduceNumber);
        reduceForm << " * X^" << degree << " ";
    }
    reduceForm << "= 0";
    _reduceForm = reduceForm.str();
    _equation = mysplit(_reduceForm, DELIMITER);
    std::cout << "Reduced form: " << _reduceForm << "\n";
    specialCases();
}

void Computor::specialCases() {

    if (_degree == 2 && _equation[VAR_3] == "0")
        _degree = 1;
    if (_degree == 1 && _equation[VAR_2] == "0")
        _degree = 0;
    std::cout << "Polynomial degree: " << _degree << "\n";
    if (_degree == 0 && _equation[VAR_1] != "0")
        throw (std::logic_error("This equation is impossible!"));
}

void Computor::getSolution() {
    switch (_degree)
    {
    case 0:
        std::cout << "Each real number is a solution.\n";
        break;
    case 1:
        resolveFirstDegree();
        break;
    case 2:
        resolveSecondDegree();
        break;
    default:
        std::cout << "The polynomial degree is strictly greater than 2, I can't solve.\n";
        break;
    }
}

void Computor::resolveFirstDegree() {
    float result;
    float b = stof(_equation[VAR_1]);
    float a = stof(_equation[VAR_2]);
    if (a == 0 && b == 0) {
        std::cout << "Each real number is a solution.\n";
        return ;
    }
    if (_equation[3] == "-")
        a = INVERSE(a);
    result = (0 - b) / a;
    result = result == -0 ? result = 0 : result;
    std::cout << "The solution is:\n" << result << "\n";
}

void Computor::resolveSecondDegree() {
    float c = stof(_equation[VAR_1]);
    float b = stof(_equation[VAR_2]);
    float a = stof(_equation[VAR_3]);
    if (_equation[3] == "-")
        b = INVERSE(b);
    if (_equation[7] == "-")
        a = INVERSE(a);
    _discriminant = pow(b, 2) - 4 * a * c;
    if (_discriminant > 0) {
        std::cout << "Discriminant is strictly positive, the two solutions are:\n";
        std::cout << (-b - sqrt(_discriminant)) / (2 * a) << "\n";
        std::cout << (-b + sqrt(_discriminant)) / (2 * a) << "\n";
    }
    else if (_discriminant == 0) {
        std::cout << "Discriminant is equal to 0, the only solution is:\n";
        std::cout << -b / (2 * a);
    }
    else {
        std::cout << "Discriminant is strictly negative, the two complex solutions are:\n";
        std::cout << -b / (2 * a) << " - i" << sqrt(ABS(_discriminant)) / (2 * a) << "\n";
        std::cout << -b / (2 * a) << " + i" << sqrt(ABS(_discriminant)) / (2 * a) << "\n";
    }
}
