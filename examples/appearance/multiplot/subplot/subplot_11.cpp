#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    subplot({0.1, 0.3, 0.3, 0.3});
    std::vector<std::vector<double>> y = {
        {16, 5, 9, 4}, {2, 11, 7, 14}, {3, 10, 6, 15}, {13, 8, 12, 1}};
    plot(y);
    title("First Subplot");

    subplot({0.5, 0.15, 0.4, 0.7});
    bar(y);
    title("Second Subplot");

    show();
    return 0;
}