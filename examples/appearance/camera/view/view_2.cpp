#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    xlabel("x");
    ylabel("y");
    zlabel("z");
    view(2);

    wait();
    return 0;
}