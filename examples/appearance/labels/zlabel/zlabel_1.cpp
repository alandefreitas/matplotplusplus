#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y, Z] = peaks();
    surf(X, Y, Z);
    zlabel("Height");

    show();
    return 0;
}