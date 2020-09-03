#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    subplot(2, 1, 0);
    fplot("sin(x)");

    subplot(2, 1, 1);
    fplot("sin(5*x)");

    show();
    return 0;
}