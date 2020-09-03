#include <matplot/matplot.h>
#include <set>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(-pi, +pi, 20);
    // test with set, just because...
    std::set<double> x2(x.begin(), x.end());

    std::vector<double> y =
        transform(x, [](auto x) { return tan(sin(x)) - sin(tan(x)); });
    plot(x2, y, "--gs")
        ->line_width(2)
        .marker_size(10)
        .marker_color("b")
        .marker_face_color({.5, .5, .5});

    show();
    return 0;
}