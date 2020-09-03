#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = iota(0, 20, 100);
    vector_1d y = {88, 67, 98, 43, 45, 65};
    bar(x, y);
    ytickformat("usd");
    std::cout << "ytickformat(): " << ytickformat() << std::endl;
    ytickformat("$%.0f");

    show();
    return 0;
}