#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto ax1 = subplot(2, 2, 0);
    fplot("cos(x)");
    title("Subplot 1: Cosine");

    auto ax2 = subplot(2, 2, 1);
    fplot("1 - x**2/2 + x**4/24");
    title("Subplot 2: Polynomial");

    auto ax3 = subplot(2, 2, {2, 3});
    fplot("cos(x)", "b");
    hold(on);
    fplot("1 - x**2/2 + x**4/24", "g");
    title("Subplot 3 and 4: Both");

    axis({ax1, ax2, ax3}, {-4, 4, inf, inf});

    show();
    return 0;
}