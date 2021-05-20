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

    constexpr float pi_f = 3.14f;
    axis({-pi_f, pi_f, -1.5f, +1.5f});
    fplot("cos(x)");
    h->draw();
    show();
    return 0;
}