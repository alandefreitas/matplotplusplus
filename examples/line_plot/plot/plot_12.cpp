#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<int> y = {2, 4, 7, 7, 6, 3, 9, 7, 3, 5};
    plot(y);

    show();
    return 0;
}