#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    image_channels_t image = imread("lena_color.tiff");
    image = imvignette(image);

    auto image_to_show = imresize(image, 0.5);
    imshow(image_to_show);

    auto image_to_save = imresize(image, 2., image_interpolation::bicubic);
    imwrite(image_to_save, "lena_bicubic.tiff");

    imwrite(imresize(image, 2., image_interpolation::bilinear),
            "lena_bilinear.tiff");

    imwrite(imresize(image, 2., image_interpolation::grid), "lena_grid.tiff");

    imwrite(imresize(image, 2., image_interpolation::lanczos),
            "lena_lanczos.tiff");

    imwrite(imresize(image, 2., image_interpolation::moving_average),
            "lena_moving_average.tiff");

    imwrite(imresize(image, 2., image_interpolation::nearest),
            "lena_nearest.tiff");

    show();
    return 0;
}