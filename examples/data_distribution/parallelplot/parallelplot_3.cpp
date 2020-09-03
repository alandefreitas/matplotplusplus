#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X(4);
    X[0] = randn(100, 50, 200);
    X[1] = transform(X[0], [](double x) { return x + rand(-30, +30); });
    X[2] = transform(X[0], [](double x) { return x > 50 ? +1. : -1.; });
    X[3] = transform(X[0], [](double x) { return cos(rand(-30, +30)); });
    auto colors = X[2];
    parallelplot(X, colors);

    show();
    return 0;
}