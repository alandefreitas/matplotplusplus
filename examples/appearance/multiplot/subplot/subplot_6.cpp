#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    auto ax1 = subplot(2, 1, 0);
    fplot("sin(x)");
    title("First Subplot");

    auto ax2 = subplot(2, 1, 1);
    fplot("cos(x)");
    title("Second Subplot");

    ax1->font_size(15);
    ax2->line_width(2);

    show();
    return 0;
}