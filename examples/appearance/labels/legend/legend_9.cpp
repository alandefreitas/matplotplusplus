#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto random_lines = rand(4, 4, 0, 1);
    auto ls = plot(random_lines);
    ::matplot::legend({"Line 1", "Line 2"});
    ls[2]->display_name("Line 3");

    show();
    return 0;
}