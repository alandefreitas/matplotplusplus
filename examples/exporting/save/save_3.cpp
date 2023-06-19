#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto f = figure(true);
    f->font("Helvetica");

    std::vector<double> x = {2, 4, 7, 2, 4, 5, 2, 5, 1, 4};
    bar(x);

    const auto &m = backend::gnuplot::extension_terminal();
    for (const auto &[extension, format] : m) {
        (void) extension;
        save(std::string("formats/barchart ") + std::string(format),
             std::string(format));
    }

    return 0;
}