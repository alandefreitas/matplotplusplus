#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    plot(iota(1, 3));
    hold(on);
    vector_2d C = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    auto im = image(C);
    im->alpha(0.5);
    gca()->y_axis().reverse(false);

    show();
    return 0;
}