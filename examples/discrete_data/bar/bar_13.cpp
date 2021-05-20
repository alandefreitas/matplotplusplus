#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> y = {
        {10, 30, 50}, {15, 35, 55}, {20, 40, 62}};
    auto b = bar(y);
    b->face_colors()[2] = {0.f, .2f, .6f, .5f};
    gcf()->draw();

    show();
    return 0;
}