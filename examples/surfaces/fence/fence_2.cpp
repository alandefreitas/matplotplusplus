#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .125, 3));
    auto Z = peaks(X, Y);
    // the fence color will be the average Z in that row
    vector_1d c;
    for (const auto &row : Z) {
        c.emplace_back(mean(row));
    }
    fence(X, Y, Z, c);
    colorbar();

    show();
    return 0;
}