#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 180, 7);
    std::vector<double> y = {0.8, 0.9, 0.1, 0.9, 0.6, 0.1, 0.3};
    plot(x, y);
    title("Time Plot");
    xlabel("Time");
    yrange({0, 1});
    xticks({0, 30, 60, 90, 120, 150, 180});
    xticklabels(
        {"00:00s", "30:00", "01:00", "01:30", "02:00", "02:30", "03:00"});

    show();
    return 0;
}