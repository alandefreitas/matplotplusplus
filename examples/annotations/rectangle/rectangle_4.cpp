#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto r = rectangle(1, 2, 5, 10);
    r->fill(true);
    r->color({0, .5, .5});
    r->line_width(3);

    auto r2 = rectangle(1, 2, 5, 10);
    r2->color("blue");
    r2->line_width(3);

    show();
    return 0;
}