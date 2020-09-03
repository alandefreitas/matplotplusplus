#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::vector<double>> C = {
        {0, 2, 4, 6}, {8, 10, 12, 14}, {16, 18, 20, 22}};
    image(5, 8, 3, 6, C);

    show();
    return 0;
}