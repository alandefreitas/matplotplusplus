#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> newcolors = {
        {0, 0.5, 1}, {0.5, 0, 1}, {0.7, 0.7, 0.7}};
    colororder(newcolors);
    bar(vector_2d{{10, 20, 30}, {25, 35, 45}, {30, 40, 52}});

    show();
    return 0;
}