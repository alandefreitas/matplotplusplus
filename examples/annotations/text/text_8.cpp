#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto y = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(y);

    text(3, 90, "λ_{12}^{3/2}/π - πΔ^{2/3}");
    text(4, 60, "lambda12^{3/2}/pi - pi delta^{2/3}");

    show();
    return 0;
}