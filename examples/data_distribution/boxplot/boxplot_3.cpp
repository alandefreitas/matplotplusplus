#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> x(25);
    for (auto &xi : x) {
        xi = randn(100, 0, 1);
    }

    subplot(2, 1, 0);
    boxplot(x);

    subplot(2, 1, 1);
    boxplot(x)->box_style(box_chart::box_style_option::outline);

    show();
    return 0;
}