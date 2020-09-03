#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = rand(50, 0, 10);
    auto y = rand(50, 0, 10);
    auto z = rand(50, 0, 10);
    scatter3(x, y, z);
    ztickformat("%.2f");

    show();
    return 0;
}
