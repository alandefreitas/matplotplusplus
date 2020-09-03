#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(1, 3);
    std::vector<std::string> labels = {"Taxes", "Expenses", "Profit"};
    pie(x, labels);

    show();
    return 0;
}