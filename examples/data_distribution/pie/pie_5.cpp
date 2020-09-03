#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> y2010 = {50, 0, 100, 95};
    std::vector<double> y2011 = {65, 22, 97, 120};
    std::vector<std::string> labels = {"Investments", "Cash", "Operations",
                                       "Sales"};

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    pie(ax1, y2010);
    title("2010");

    auto ax2 = nexttile();
    pie(ax2, y2011);
    title("2011");

    show();
    return 0;
}