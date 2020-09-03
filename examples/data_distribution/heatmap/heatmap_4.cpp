#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> data = {
        {45, 60, 32}, {43, 54, 76}, {32, 94, 68}, {23, 95, 58}};
    heatmap(data);
    title("T-Shirt Orders");
    auto ax = gca();
    ax->x_axis().ticklabels({"Small", "Medium", "Large"});
    ax->y_axis().ticklabels({"Green", "Red", "Blue", "Gray"});
    xlabel(ax, "Sizes");
    ylabel(ax, "Colors");

    show();
    return 0;
}