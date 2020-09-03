#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y, Z] = peaks();
    mesh(X, Y, Z)->hidden_3d(false);

    show();
    return 0;
}