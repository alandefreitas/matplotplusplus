#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    subplot(2, 2, 0);
    fplot("sin(x)");
    title("Subplot 1: sin(x)");

    subplot(2, 2, 1);
    fplot("sin(2*x)");
    title("Subplot 2: sin(2x)");

    subplot(2, 2, 2);
    fplot("sin(4*x)");
    title("Subplot 3: sin(4x)");

    subplot(2, 2, 3);
    fplot("sin(8*x)");
    title("Subplot 4: sin(8x)");

    show();
    return 0;
}