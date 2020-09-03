#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10000, 21);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    stem(x, y);
    xtickangle(45);

    show();
    return 0;
}