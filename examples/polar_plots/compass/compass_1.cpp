#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = rand(20, -1, +1);
    std::vector<double> y = rand(20, -1, +1);
    compass(x, y);

    show();
    return 0;
}