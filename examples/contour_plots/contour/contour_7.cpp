#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    auto c = contour(X, Y, Z);
    c->contour_text(true);
    c->font_size(15);
    c->font_color("blue");
    c->font_weight("bold");

    show();
    return 0;
}