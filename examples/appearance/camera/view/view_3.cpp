#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y] = meshgrid(iota(-5, .5, 5));
    auto z = transform(
        x, y, [](double x, double y) { return y * sin(x) - x * cos(y); });
    surf(x, y, z);
    xlabel("x");
    ylabel("y");
    zlabel("z");

    auto [az, el] = view();
    std::cout << "az: " << az << std::endl;
    std::cout << "el: " << el << std::endl;

    auto [naz, nel] = view(-5, -2, 5);
    std::cout << "naz: " << naz << std::endl;
    std::cout << "nel: " << nel << std::endl;

    show();
    return 0;
}