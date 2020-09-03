#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto ind = linspace(0, 51);
    auto drivers =
        transform(ind, [](double ind) { return pow(ind, 2) + randn(0, 50); });
    scatter(ind, drivers);
    title("Highway data");
    xlabel("States");
    ylabel("Licensed Drivers");
    ylim({0, inf});

    hold(on);
    auto pop = transform(
        ind, [](double ind) { return 10000 * (pow(ind, 2) + randn(0, 100)); });
    scatter(ind, pop)->use_y2(true);
    y2label("Vehicle Miles Traveled");
    y2lim({0, inf});

    show();
    return 0;
}