#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto sin_t = [](double t) { return sin(t); };
    auto cos_t = [](double t) { return cos(t); };
    auto t_t = [](double t) { return t; };
    auto fp = fplot3(sin_t, cos_t, t_t, std::array<double, 2>{0, 2 * pi})
                  ->line_width(2);
    hold(on);
    fplot3(sin_t, cos_t, t_t, std::array<double, 2>{2 * pi, 4 * pi}, "--or");
    fplot3(sin_t, cos_t, t_t, std::array<double, 2>{4 * pi, 6 * pi}, "-.*c");
    hold(off);

    show();
    return 0;
}