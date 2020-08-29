#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    subplot(2,2,0);
    fplot("sin(x)");
    title("Subplot 1: sin(x)");

    subplot(2,2,1);
    fplot("sin(2*x)");
    title("Subplot 2: sin(2x)");

    subplot(2,2,2);
    fplot("sin(4*x)");
    title("Subplot 3: sin(4x)");

    subplot(2,2,3);
    fplot("sin(8*x)");
    title("Subplot 4: sin(8x)");

    wait();
    return 0;
}