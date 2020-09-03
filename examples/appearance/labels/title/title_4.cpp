#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto t = iota(0, 0.01, 0.2);
    auto y = transform(t, [](double x) { return exp(-25 * x); });
    plot(t, y);
    title("y = {/:Italic e^{λt}}", "b");

    show();
    return 0;
}