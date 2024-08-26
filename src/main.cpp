#include "../inc/Computor.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: ./computor [polynomial equation]\n";
        return EXIT_FAILURE;
    }
    std::string equation(argv[1]);
    if (equation.empty()) {
        std::cout << "Error: empty input!\n";
        return EXIT_FAILURE;
    }
    Computor computor;
    try {
        computor.parsEquation(equation);
        computor.getReduceForm();
        computor.getSolution();
    } catch (std::exception & e) {
        std::cout << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
