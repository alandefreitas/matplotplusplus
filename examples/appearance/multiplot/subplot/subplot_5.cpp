#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    subplot({0.1f, 0.3f, 0.3f, 0.3f});
    fplot("sin(x)");
    title("First Subplot");

    subplot({0.5f, 0.15f, 0.4f, 0.7f});
    fplot("cos(x)");
    title("Second Subplot");

    show();
    return 0;
}