#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {0.19, 0.22, 0.41};
    pie(x);

    show();
    return 0;
}