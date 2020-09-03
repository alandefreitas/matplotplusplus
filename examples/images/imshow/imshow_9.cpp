#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    image_channels_t image = imread("lena_color.tiff");
    image = imresize(image, 0.1, image_interpolation::bilinear);
    imshow(image);

    show();
    return 0;
}