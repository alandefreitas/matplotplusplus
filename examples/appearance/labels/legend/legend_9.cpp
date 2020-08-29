#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto random_lines = rand(4, 4, 0, 1);
    auto ls = plot(random_lines);
    legend("Line 1", "Line 2");
    ls[2]->display_name("Line 3");

    wait();
    return 0;
}