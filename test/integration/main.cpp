//
// Created by Alan Freitas on 05/10/20.
//

#include <matplot/matplot.h>

int main() {
    std::vector<double> x = {1,2,3,4,5};
    matplot::plot(x);
    matplot::show();
}
