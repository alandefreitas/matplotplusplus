#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    image_channels_t image = imread("lena_color.tiff");
    image = imvignette(image);
    imshow(image);

    show();
    return 0;
}