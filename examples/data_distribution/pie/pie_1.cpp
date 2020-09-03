#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {1, 3, 0.5, 2.5, 2};
    pie(x);

    show();
    return 0;
}