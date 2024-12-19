#include "../inc/Computor.hpp"

void plotSecondDegree(float a, float b, float c) {
    #ifndef PLOT
        return ;
    #endif
    
    std::vector<double> x;
    std::vector<double> vline_x;
    for (double i = -PLOT_LENGTH; i <= PLOT_LENGTH; i += 0.1) {
        x.push_back(i);
        vline_x.push_back(0.0);
    }
    std::vector<double> y;
    for (double i : x)
        y.push_back(a * i * i + b * i + c);
    plt::plot(x, y);
    plt::plot(x, vline_x, "r--");
    plt::title("Second Degree Equation: y = ax^2 + bx + c");
    plt::xlabel("x");
    plt::ylabel("y");
    plt::show();
    Py_Finalize();
}

void plotFirstDegree(float a, float b, float result) {
    #ifndef PLOT
        return ;
    #endif

    std::vector<double> x;
    std::vector<double> vline_x;
    for (double i = result - PLOT_LENGTH; i <= result + PLOT_LENGTH; i += 0.1) {
        x.push_back(i);
        vline_x.push_back(0.0);
    }
    std::vector<double> y;
    for (double i : x)
        y.push_back(a * i + b);
    plt::plot(x, y);
    plt::plot(x, vline_x, "r--");
    plt::title("First Degree Equation: y = ax + b");
    plt::xlabel("x");
    plt::ylabel("y");
    plt::show();
    Py_Finalize();
}