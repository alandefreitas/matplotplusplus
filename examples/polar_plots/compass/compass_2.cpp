#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = rand(20, -1, +1);
    std::vector<double> y = rand(20, -1, +1);
    auto ax1 = subplot(1, 2, 0);
    compass(ax1, x, y);

    auto ax2 = subplot(1, 2, 1);
    vectors_handle l = std::make_shared<class vectors>(ax2, x, y);
    ax2->emplace_object(l);

    show();
    return 0;
}