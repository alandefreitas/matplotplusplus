#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto image = imread("lena_color.tiff");
    imshow(image);

    wait();
    return 0;
}