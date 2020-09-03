#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    figure();

    auto ax1 = subplot(2, 1, 0);
    auto theta = linspace(0, 2 * pi, 50);
    auto rho =
        transform(theta, [](double theta) { return sin(theta) * cos(theta); });
    polarplot(ax1, theta, rho);

    auto ax2 = subplot(2, 1, 1);
    polarscatter(ax2, theta, rho);

    show();
    return 0;
}