#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::vector<double>> C = {
        {0, 2, 4, 6}, {8, 10, 12, 14}, {16, 18, 20, 22}};
    image(C);
    colorbar();

    show();
    return 0;
}