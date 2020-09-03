#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto image = imread("lena_gray.tiff");

    auto &gray = image[0];
    auto [h, w] = size(gray);
    double mean_intensity = 0;
    for (const auto &row : gray) {
        for (const auto &pixel : row) {
            mean_intensity += pixel;
        }
    }
    mean_intensity /= (h * w);

    for (auto &row : gray) {
        for (auto &pixel : row) {
            pixel = pixel > mean_intensity ? 255 : 0;
        }
    }

    imshow(gray);

    show();
    return 0;
}