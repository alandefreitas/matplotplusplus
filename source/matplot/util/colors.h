//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_COLORS_H
#define MATPLOTPLUSPLUS_COLORS_H

#include <matplot/detail/config.h>
#include <array>
#include <string>
#include <vector>

namespace matplot {
    enum class color {
        blue,
        black,
        red,
        green,
        yellow,
        cyan,
        magenta,
        white,
        none,
    };

    MATPLOT_EXPORTS
    std::string to_string(matplot::color c);

    MATPLOT_EXPORTS
    matplot::color string_to_color(std::string_view s);

    MATPLOT_EXPORTS
    matplot::color char_to_color(char c);

    MATPLOT_EXPORTS
    bool is_valid_color_char(char c);

    MATPLOT_EXPORTS
    std::array<float, 4> to_array(matplot::color c);

    template <class T> std::array<float, 4> to_array(std::vector<T> c) {
        std::array<float, 4> r{0.f, 0.f, 0.f, 0.f};
        if (c.size() == 1) {
            r[0] = 0.f;
            r[1] = static_cast<float>(c[0]);
            r[2] = static_cast<float>(c[0]);
            r[3] = static_cast<float>(c[0]);
        } else if (c.size() == 3) {
            r[0] = 0.f;
            r[1] = static_cast<float>(c[0]);
            r[2] = static_cast<float>(c[1]);
            r[3] = static_cast<float>(c[2]);
        } else if (c.size() == 4) {
            r[0] = static_cast<float>(c[0]);
            r[1] = static_cast<float>(c[1]);
            r[2] = static_cast<float>(c[2]);
            r[3] = static_cast<float>(c[3]);
        }
        return r;
    }

    MATPLOT_EXPORTS
    std::array<float, 4> to_array(std::string_view str_color);

    MATPLOT_EXPORTS
    std::string to_string(const std::array<float, 4> &c);
    MATPLOT_EXPORTS
    std::string to_string(const std::array<float, 3> &c);

    constexpr std::array<float, 4> default_color(size_t index) {
        constexpr size_t num_default_colors = 7;
        size_t color_index = index % num_default_colors;
        switch (color_index) {
        case 0:
            return {0, 0, 0.447f, 0.741f};
        case 1:
            return {0, 0.85f, 0.325f, 0.098f};
        case 2:
            return {0, 0.929f, 0.694f, 0.125f};
        case 3:
            return {0, 0.494f, 0.184f, 0.556f};
        case 4:
            return {0, 0.466f, 0.674f, 0.188f};
        case 5:
            return {0, 0.301f, 0.745f, 0.933f};
        case 6:
            return {0, 0.635f, 0.078f, 0.184f};
        default:
            return {};
        }
    }

    namespace palette {
        /*
         * \see
         * - https://doi.org/10.1179%2Fcaj.1996.33.2.79
         * - https://bids.github.io/colormap/
         * - https://dx.doi.org/10.1371/journal.pone.0199239
         * - https://dx.doi.org/10.5670/oceanog.2016.66
         * - https://dx.doi.org/10.1071/aseg2015ab107
         * - https://zenodo.org/record/4153113
         */
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> accent(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> blues(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> brbg(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> bugn(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> bupu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> chromajs(size_t n = 9);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> dark2(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> gnbu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> gnpu(size_t n = 10);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> greens(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> greys(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> inferno(size_t n = 256);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> jet(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> turbo(size_t n = 256);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> magma(size_t n = 256);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> oranges(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> orrd(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> paired(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> parula(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> pastel1(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> pastel2(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> piyg(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> plasma(size_t n = 256);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> prgn(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> pubu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> pubugn(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> puor(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> purd(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> purples(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> rdbu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> rdgy(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> rdpu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> rdylbu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> rdylgn(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> reds(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> sand(size_t n = 10);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> set1(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> set2(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> set3(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> spectral(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> viridis(size_t n = 256);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> whylrd(size_t n = 5);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> ylgn(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> ylgnbu(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> ylorbr(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> ylorrd(size_t n = 8);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> ylrd(size_t n = 4);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> hsv(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> hot(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> cool(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> spring(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> summer(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> autumn(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> winter(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> gray(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> bone(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> copper(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> pink(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> lines(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> colorcube(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> prism(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> flag(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> white(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> default_map(size_t n = 64);
        MATPLOT_EXPORTS
        std::vector<std::vector<double>> default_colors_map(size_t n = 7);
    } // namespace palette

    MATPLOT_EXPORTS
    std::array<float, 4>
    colormap_interpolation(double value, double min, double max,
                           const std::vector<std::vector<double>> &cm);

} // namespace matplot

#endif // MATPLOTPLUSPLUS_COLORS_H
