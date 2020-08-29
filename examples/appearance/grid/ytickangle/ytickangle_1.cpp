#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 50, 20);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    stem(x, y);
    ytickangle(90);

    wait();
    return 0;
}