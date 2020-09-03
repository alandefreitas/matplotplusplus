#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = {1, 2, 3};
    std::vector<std::vector<double>> y = {{2, 3, 6}, {11, 23, 26}};
    auto b = bar(x, y);

    std::vector<double> label_x;
    std::vector<double> label_y;
    std::vector<std::string> labels;
    for (size_t i = 0; i < y.size(); ++i) {
        for (size_t j = 0; j < x.size(); ++j) {
            label_x.emplace_back(b->x_end_point(i, j));
            label_y.emplace_back(y[i][j] + 1);
            labels.emplace_back(num2str(y[i][j]));
        }
    }

    hold(on);
    text(label_x, label_y, labels);

    show();
    return 0;
}