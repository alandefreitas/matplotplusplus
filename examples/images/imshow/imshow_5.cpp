#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto image = imread("lena_gray.tiff");
    imshow(image[0]);
    colormap(palette::greens());

    show();
    return 0;
}