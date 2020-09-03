#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> data = {
        {24, 10}, {10, 5}, {24, 16}, {8, 3}};
    heatmap(data);
    title("Count of SelfAssessedHealthStatus vs. Smoker");
    auto ax = gca();
    ax->x_axis().ticklabels({"false", "true"});
    ax->y_axis().ticklabels({"Excellent", "Fair", "Good", "Poor"});
    xlabel(ax, "Smoker");
    ylabel(ax, "SelfAssessedHealthStatus");

    show();
    return 0;
}