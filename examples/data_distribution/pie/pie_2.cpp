#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {1, 3, 0.5, 2.5, 2};
    std::vector<double> explode = {0, 1, 0, 1, 0};
    pie(x, explode);

    show();
    return 0;
}