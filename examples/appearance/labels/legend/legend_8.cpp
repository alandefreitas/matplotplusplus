#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto rdm = rand(4, 4, 0, 1);

    plot(rdm);

    auto lgd = ::matplot::legend({"Line 1", "Line 2", "Line 3", "Line 4"});
    lgd->font_size(12);
    lgd->text_color("blue");
    lgd->num_columns(2);

    show();
    return 0;
}