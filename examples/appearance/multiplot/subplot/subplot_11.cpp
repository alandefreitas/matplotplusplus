#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    subplot({0.1f, 0.3f, 0.3f, 0.3f});
    std::vector<std::vector<double>> y = {
        {16, 5, 9, 4}, {2, 11, 7, 14}, {3, 10, 6, 15}, {13, 8, 12, 1}};
    plot(y);
    title("First Subplot");

    subplot({0.5f, 0.15f, 0.4f, 0.7f});
    bar(y);
    title("Second Subplot");

    show();
    return 0;
}