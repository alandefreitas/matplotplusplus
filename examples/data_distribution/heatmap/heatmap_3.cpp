#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> data = {
        {38.46, 39}, {39.7, 36.2}, {38.13, 38.88}, {33.88, 43}};
    heatmap(data);
    title("Mean of Age");
    auto ax = gca();
    ax->x_axis().ticklabels({"false", "true"});
    ax->y_axis().ticklabels({"Excellent", "Fair", "Good", "Poor"});
    xlabel(ax, "Smoker");
    ylabel(ax, "SelfAssessedHealthStatus");

    show();
    return 0;
}