#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<int> y1 = {1, 10, 7, 6, 7, 7, 4, 6, 1, 6};
    std::vector<int> y2 = {70, 20, 90, 10, 90, 70, 20, 10, 100, 50};
    plot(y1);
    hold(on);
    plot(y2)->use_y2(true);

    show();
    return 0;
}