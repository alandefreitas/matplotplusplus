#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = rand(30, 0, 1);
    auto y = rand(30, 0, 1);
    scatter(x, y);
    ytickformat("%.2f");

    show();
    return 0;
}