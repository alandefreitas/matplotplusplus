#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto image = imread("lena_gray.tiff");
    imshow(image);

    show();
    return 0;
}