#include <matplot/matplot.h>
#include <set>

int main() {
    using namespace matplot;

    std::set<std::vector<double>> Y = {
        {16, 5, 9, 4}, {2, 11, 7, 14}, {3, 10, 6, 15}, {13, 8, 12, 1}};
    plot(Y);

    show();
    return 0;
}