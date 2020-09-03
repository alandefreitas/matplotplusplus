#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {1, 3, 5, 7, 10};
    std::vector<double> y = {2, 5, 6, 7, 11};
    auto stair_handles = stairs(x, y, x, y, x, y, x, y);

    stair_handles[0]->stair_style(stair::stair_style::fill);

    stair_handles[1]
        ->stair_style(stair::stair_style::trace_x_first)
        .line_width(4);

    stair_handles[2]
        ->stair_style(stair::stair_style::trace_y_first)
        .line_width(2);

    stair_handles[3]
        ->stair_style(stair::stair_style::histogram)
        .marker_style(line_spec::marker_style::circle)
        .line_width(1)
        .marker_color("m")
        .marker_face(true)
        .marker_size(10);

    axis({0, 12, 0, 13});

    show();
    return 0;
}