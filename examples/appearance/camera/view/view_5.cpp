#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks(20);
    auto s = surf(x, y, z);
    xlabel("x-axis");
    ylabel("y-axis");
    zlabel("z-axis");

    rotate(20, 30);

    show();
    return 0;
}