#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto h = figure(true);
    h->name("Measured Data");
    h->number_title(false);
    h->color("green");
    h->position({0, 0, 600, 600});
    h->size(500, 500);
    h->draw();
    h->font("Arial");
    h->font_size(40);
    h->title("My experiment");

    constexpr double pi = 3.14;
    axis({-pi, pi, -1.5, +1.5});
    fplot("cos(x)");
    h->draw();
    show();
    return 0;
}