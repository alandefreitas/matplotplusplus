#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> Y = {
        {1, 3, 4, 0}, {2, 5, 4, 7}, {6, 4, 5, 3}};

    auto f = gcf();
    f->width(f->width() * 2);

    subplot(1, 2, 0);
    area(Y, -4.);
    title("Stacked");

    subplot(1, 2, 1);
    area(Y, -4, false);
    title("Not stacked");

    save("area_4.svg");

    show();
    return 0;
}