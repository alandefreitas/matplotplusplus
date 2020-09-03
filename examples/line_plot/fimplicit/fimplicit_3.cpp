#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fimplicit([](double x, double y) { return pow(x, 2) + pow(y, 2) - 1; });
    hold(on);
    fimplicit([](double x, double y) { return pow(x, 2) + pow(y, 2) - 2; },
              "--g")
        ->line_width(2);
    hold(off);

    show();
    return 0;
}