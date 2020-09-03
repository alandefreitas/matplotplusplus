#include <cmath>
#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto xt = [](double t) {
        return exp(-std::abs(t) / 10) * sin(5 * std::abs(t));
    };
    auto yt = [](double t) {
        return exp(-std::abs(t) / 10) * cos(5 * std::abs(t));
    };
    auto zt = [](double t) { return t; };
    auto fp = fplot3(xt, yt, zt)->t_range({-10, 10}).color("r");
    xlabel("e^{-|z|/10} sin(2|z|)");
    ylabel("e^{-|z|/10} cos(2|z|)");
    zlabel("z");
    grid(true);

    auto ax = gca();
    float da = ax->azimuth();
    float de = ax->elevation();
    for (size_t i = 0; i <= 180; ++i) {
        view(da + 2 * i, de + i);
        title("Azimuth: " + num2str(da + 2 * i) +
              " Elevation: " + num2str(de + i));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    show();
    return 0;
}