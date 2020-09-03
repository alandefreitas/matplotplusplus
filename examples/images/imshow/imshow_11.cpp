#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    image_channels_t image = imread("lena_gray.tiff");

    imshow(image[0]);
    colormap(palette::blues());

    imwrite(image[0], palette::blues(), "lena_blues.tiff");

    show();
    return 0;
}