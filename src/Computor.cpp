#include "../inc/Computor.hpp"

Computor::Computor()
{
    _degree = 0;
}

Computor::~Computor() {}

void parsStruct(std::vector<int> equationStruct)
{
    int i = 0;
    const int structSize = equationStruct.size();
    const int format[3] = {NUMBER, MULTIPLICATION, VARIABLE};
    while (i < structSize)
    {
        for (int j = 0; j != 3; j++)
        {
            if (i >= structSize || equationStruct[i] != format[j])
                throw(std::logic_error("Bad format!"));
            i++;
        }
        if (i < structSize && equationStruct[i] != EQUAL && equationStruct[i] != OPERATOR)
            throw(std::logic_error("Bad format!"));
        i++;
    }
}

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

void Computor::parsEquation(std::string &equation)
{
    char *endPtr;
    int degree;
    _equation = mysplit(equation, DELIMITER);
    const int size = _equation.size();
    std::vector<int> equationStruct;

    for (int i = 0; i < size; i++)
    {
        strtof(_equation[i].c_str(), &endPtr);
        if (!strlen(endPtr))
            equationStruct.push_back(NUMBER);
        else if (_equation[i] == "*")
            equationStruct.push_back(MULTIPLICATION);
        else if (_equation[i] == "=")
            equationStruct.push_back(EQUAL);
        else if (_equation[i] == "-" || _equation[i] == "+")
            equationStruct.push_back(OPERATOR);
        else if ((degree = isVariable(_equation[i])) >= 0)
        {
            if (degree > _degree)
                _degree = degree;
            equationStruct.push_back(VARIABLE);
        }
        else
            throw(std::logic_error("Bad input: " + _equation[i]));
    }
    if (std::count(equationStruct.begin(), equationStruct.end(), EQUAL) != 1)
        throw(std::logic_error("Bad input: an equation must be an equality!"));
    parsStruct(equationStruct);
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
    specialCases();
    std::cout << "Reduced form: " << _reduceForm << "\n";
    std::cout << "Polynomial degree: " << _degree << "\n";
}

void Computor::specialCases() {
    if (_degree == 0 && _equation[0] != "0")
        throw (std::logic_error("This equation is impossible to solve!"));
    if (_degree == 2 && _equation[8] == "0")
        throw (std::logic_error("Not a polynomial second degree equation because by definition :\n\
    ax2 + bx + c = 0 where 'a', 'b' and 'c' are real numbers with 'a' ≠ 0.\n\
    Or 'a' equal to 0 !"));
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
    float b = stof(_equation[0]);
    float a = stof(_equation[4]);
    if (_equation[3] == "-")
        a = INVERSE(a);
    result = (0 - b) / a;
    std::cout << "The solution is:\n" << result << "\n";
}

void Computor::resolveSecondDegree() {
    float c = stof(_equation[0]);
    float b = stof(_equation[4]);
    float a = stof(_equation[8]);
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
