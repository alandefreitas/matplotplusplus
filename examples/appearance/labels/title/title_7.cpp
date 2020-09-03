#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(x);
    title("X_1");
    gca()->title_enhanced(false);

    show();
    return 0;
}