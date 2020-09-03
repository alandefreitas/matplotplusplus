#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::vector<double>> C = {
        {0, 2, 4, 6}, {8, 10, 12, 14}, {16, 18, 20, 22}};
    auto im = imagesc(5, 8, 3, 6, C);
    gca()->color_box_range(4, 18);
    im->alpha(0.5);

    show();
    return 0;
}