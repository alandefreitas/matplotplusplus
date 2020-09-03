#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> data = {
        {45, 60, 32}, {43, 54, 76}, {32, 94, 68}, {23, 95, 58}};
    heatmap(data);

    show();
    return 0;
}