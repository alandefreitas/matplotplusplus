#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f = figure(true);

    std::vector<double> x = {2, 4, 7, 2, 4, 5, 2, 5, 1, 4};
    bar(x);

    save("img/barchart_ps", "postscript");
    save("img/barchart_latex", "epslatex");
    /*
     * Add to your latex document:
     * \begin{figure}
     *    \input{Barchart_latex}
     * \end{figure}
     */

    save("img/barchart_gif", "gif");
    save("img/barchart_jpeg", "jpeg");

    return 0;
}