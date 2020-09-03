#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> Y = {
        {1, 3, 1, 2}, {5, 2, 5, 6}, {3, 7, 3, 1}};

    auto f = gcf();
    f->width(f->width() * 2);

    subplot(1, 2, 0);
    area(Y);
    title("Stacked");

    subplot(1, 2, 1);
    area(Y, false);
    title("Not stacked");

    show();
    return 0;
}