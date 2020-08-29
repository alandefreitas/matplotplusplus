#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    for (size_t k = 0; k < 4; ++k) {
        auto data = rand(10,0,1);
        subplot(2,2,k);
        stem(data);
    }

    subplot(2,2,1,true);

    wait();
    return 0;
}