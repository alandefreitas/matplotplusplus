#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    save("img/peaks.gif");
    save("img/peaks.eps");
    show();
    return 0;
}