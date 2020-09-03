#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    using namespace std;

    vector<double> x = {0, 8, 8, 0};
    vector<double> y = {0, 0, 4, 4};
    polygon(x, y, "g");
    rectangle(0, 0, 2, 4, 0)->fill(true).color("red");
    rectangle(3, 0, 2, 4, 1)->fill(true).color("magenta");
    rectangle(6, 0, 2, 4, 0.5)->fill(true).color("yellow");
    text(2, 2, "Curvature 0")->alignment(labels::alignment::right);
    text(4, 2, "Curvature 1")->alignment(labels::alignment::center);
    text(6, 2, "Curvature 0.5")->alignment(labels::alignment::left);
    arrow(4, 2.2, 3, 3);
    textarrow(6, 1, 8, 0, "Last corner");

    show();
    return 0;
}