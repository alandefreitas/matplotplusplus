#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    std::vector<int> y = {2, 4, 7, 7, 6, 3, 9, 7, 3, 5};
    plot(y);

    wait();
    return 0;
}