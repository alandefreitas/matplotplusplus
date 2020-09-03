#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    surf(X, Y, Z);
    zlabel("Population Change");
    gca()->z_axis().label_color("red");

    show();
    return 0;
}