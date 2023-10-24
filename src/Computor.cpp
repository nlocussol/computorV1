#include "../inc/Computor.hpp"

Computor::Computor() {}

Computor::~Computor() {}

std::string Computor::equationFormatting(std::string & equation){
    std::stringstream result;
    for (int i = 0; equation[i]; i++) {
        if (i > 0 && strchr(SYMBOL, equation[i]))
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
            if (i < 1 || !isFloat(_equation[i - 1]))
                _equation.insert(_equation.begin() + i, "1");
        }
    }
}

void Computor::checkStructure() {
    int eqSize = _equation.size();
    for (int i = 0; i < eqSize; i++) {
        for (int j = 0; j < 3; j++) {
            if (i < eqSize && j == 0 && isFloat(_equation[i])) i++;
            else if (i < eqSize && j == 1 && _equation[i] == "*") i++;
            else if (i < eqSize && j == 2 && isVariable(_equation[i]) >= 0) i++;
            else throw(std::logic_error("Bad input: " + _equation[i]));
        }
        if (i < eqSize - 1 && _equation[i] != "=" && _equation[i] != "+" && _equation[i] != "-")
            throw(std::logic_error("Bad format: " + _equation[i]));
        else if (i == eqSize - 1 && (_equation[i] == "=" || _equation[i] == "+" || _equation[i] == "-"))
            throw(std::logic_error("Bad format: " + _equation[i]));
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
        if (isFloat(_equation[i]) || _equation[i] == "*") continue;
        else if (_equation[i] == "-" || _equation[i] == "+" || _equation[i] == "=") {
            if (i > 0 && i + 1 < _equation.size()&& _equation[i - 1] == "="
                && (_equation[i] == "+" || _equation[i] == "-" )) {
                _equation[i + 1] = _equation[i] + _equation[i + 1];
                _equation.erase(_equation.begin() + i);
            }            
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
    checkStructure();
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
    bool erase = false;
    std::stringstream buff;
    for (int degree = _degree; degree >= 0; degree--) {
        buff.str("");
        reduceNumber = reduceTab[degree];
        if (reduceNumber == 0 && !erase && degree != 0) continue;
        else erase = true;
        if (degree == 0)
            buff << reduceNumber;
        else if (reduceNumber >= 0)
            buff << "+ " << reduceNumber;
        else
            buff << "- " << ABS(reduceNumber);
        buff << " * X^" << degree << " ";
        _reduceForm = buff.str() + _reduceForm;
    }
    _reduceForm += "= 0";
    _equation = mysplit(_reduceForm, DELIMITER);
    std::cout << "Reduced form: " << _reduceForm << "\n";
    specialCases();
}

void Computor::specialCases() {

    if (_degree == 2 && (_equation.size() < VAR_3 || _equation[VAR_3] == "0"))
        _degree = 1;
    if (_degree == 1 && (_equation.size() < VAR_2 || _equation[VAR_2] == "0"))
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
    if (_equation[3] == "-")
        a = INVERSE(a);
    result = (0 - b) / a;
    result = result == -0 ? result = 0 : result;
    std::cout << "The solution is:\n" << result << " or ";
    printFraction(0 - b, a);
    std::cout << "\n";
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
    std::cout << "Discriminant: " << _discriminant << "\n";
    discriminantCase(a, b);
}

void Computor::discriminantCase(float a, float b ) {
    if (_discriminant > 0) {
        std::cout << "Discriminant is strictly positive, the two solutions are:\n";
        std::cout << (-b - sqrt(_discriminant)) / (2 * a) << " or ";
        printFraction(-b - sqrt(_discriminant), 2 * a);
        std::cout << "\n";
        std::cout << (-b + sqrt(_discriminant)) / (2 * a) << " or ";
        printFraction(-b + sqrt(_discriminant), 2 * a);
        std::cout << "\n";
    }
    else if (_discriminant == 0) {
        std::cout << "Discriminant is equal to 0, the only solution is:\n";
        std::cout << -b / (2 * a) << " or ";
        printFraction(-b + sqrt(_discriminant), 2 * a);
        std::cout << "\n";
    }
    else {
        std::cout << "Discriminant is strictly negative, the two complex solutions are:\n";
        std::cout << -b / (2 * a) << " - " << sqrt(ABS(_discriminant)) / (2 * a) << "i or ";
        printFraction(-b / (2 * a), (2 * a));
        std::cout << " - ";
        printFraction(sqrt(ABS(_discriminant)), (2 * a));
        std::cout << "i\n";
        std::cout << -b / (2 * a) << " + " << sqrt(ABS(_discriminant)) / (2 * a) << "i or ";
        printFraction(-b / (2 * a), (2 * a)); 
        std::cout << " + ";
        printFraction(sqrt(ABS(_discriminant)), (2 * a));
        std::cout << "i\n";
    }
}

void Computor::printFraction(float numerator, float quotient) {
    int mulNu = pow(10, countDecimal(numerator));
    numerator *= mulNu;
    int nuGCD = GCD(numerator, mulNu);
    int mulQu = pow(10, countDecimal(quotient));
    quotient *= mulQu;
    int quGCD = GCD(numerator, mulQu);

    numerator =  int((numerator / nuGCD) * (mulQu / quGCD));
    quotient =  int((mulNu / nuGCD) * (quotient / quGCD));
    int gcd = GCD(numerator, quotient);
    std::cout << std::fixed << std::setprecision(0) << numerator / gcd << " / " << quotient / gcd;
    return ;
}