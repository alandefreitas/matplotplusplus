#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(x);
    title("α^2 and X_1");

    show();
    return 0;
}