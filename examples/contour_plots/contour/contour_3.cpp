#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contour(X, Y, Z, std::vector<double>{1});

    show();
    return 0;
}