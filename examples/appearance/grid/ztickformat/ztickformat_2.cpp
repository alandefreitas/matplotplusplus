#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto z = rand(5, 5, 0, 10);
    stem3(z);
    ztickformat("usd");

    std::cout << "ztickformat(): " << ztickformat() << std::endl;
    ztickformat("$%.0f");

    show();
    return 0;
}
