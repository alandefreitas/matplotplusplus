#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    for (size_t i = 0; i < Z.size(); ++i) {
        Z[i][25] = NaN;
    }
    contour(X, Y, Z)->contour_text(true);

    show();
    return 0;
}