#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();

    subplot(3, 3, 0);
    surf(x, y, z)->lighting(true).primary(0.0).specular(0.0);

    subplot(3, 3, 1);
    surf(x, y, z)->lighting(true).primary(0.5).specular(0.0);

    subplot(3, 3, 2);
    surf(x, y, z)->lighting(true).primary(1.0).specular(0.0);

    subplot(3, 3, 3);
    surf(x, y, z)->lighting(true).primary(0.0).specular(0.5);

    subplot(3, 3, 4);
    surf(x, y, z)->lighting(true).primary(0.5).specular(0.5);

    subplot(3, 3, 5);
    surf(x, y, z)->lighting(true).primary(1.0).specular(0.5);

    subplot(3, 3, 6);
    surf(x, y, z)->lighting(true).primary(0.0).specular(1.0);

    subplot(3, 3, 7);
    surf(x, y, z)->lighting(true).primary(0.5).specular(1.0);

    subplot(3, 3, 8);
    surf(x, y, z)->lighting(true).primary(1.0).specular(1.0);

    show();
    return 0;
}