#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    subplot({0.1, 0.3, 0.3, 0.3});
    fplot("sin(x)");
    title("First Subplot");

    subplot({0.5, 0.15, 0.4, 0.7});
    fplot("cos(x)");
    title("Second Subplot");

    show();
    return 0;
}