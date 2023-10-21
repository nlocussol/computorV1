#include "../inc/Computor.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cout << "./computor [polynomial equation]\n";
        return EXIT_FAILURE;
    }
    try {
        std::string equation(argv[1]);
        Computor computor;
        computor.parsEquation(equation);
        computor.getReduceForm();
        computor.getSolution();
    } catch (std::exception & e) {
        std::cout << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
