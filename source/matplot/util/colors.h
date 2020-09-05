//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_COLORS_H
#define MATPLOTPLUSPLUS_COLORS_H

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

    std::string to_string(matplot::color c);

    matplot::color string_to_color(std::string_view s);

    matplot::color char_to_color(char c);

    bool is_valid_color_char(char c);

    std::array<float, 4> to_array(matplot::color c);

    template <class T> std::array<float, 4> to_array(std::vector<T> c) {
        std::array<float, 4> r;
        if (c.size() == 1) {
            r[0] = 0;
            r[1] = static_cast<float>(c[0]);
            r[2] = static_cast<float>(c[0]);
            r[3] = static_cast<float>(c[0]);
        } else if (c.size() == 3) {
            r[0] = 0;
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

    std::array<float, 4> to_array(std::string_view str_color);

    std::string to_string(const std::array<float, 4> &c);
    std::string to_string(const std::array<float, 3> &c);

    constexpr std::array<float, 4> default_color(size_t index) {
        constexpr size_t num_default_colors = 7;
        size_t color_index = index % 7;
        switch (color_index) {
        case 0:
            return {0, 0, 0.4470, 0.7410};
        case 1:
            return {0, 0.8500, 0.3250, 0.0980};
        case 2:
            return {0, 0.9290, 0.6940, 0.1250};
        case 3:
            return {0, 0.4940, 0.1840, 0.5560};
        case 4:
            return {0, 0.4660, 0.6740, 0.1880};
        case 5:
            return {0, 0.3010, 0.7450, 0.9330};
        case 6:
            return {0, 0.6350, 0.0780, 0.1840};
        default:
            return {0, 0.0, 0.0, 0.0};
        }
    }

    namespace palette {
        std::vector<std::vector<double>> accent(size_t n = 8);
        std::vector<std::vector<double>> blues(size_t n = 8);
        std::vector<std::vector<double>> brbg(size_t n = 8);
        std::vector<std::vector<double>> bugn(size_t n = 8);
        std::vector<std::vector<double>> bupu(size_t n = 8);
        std::vector<std::vector<double>> chromajs(size_t n = 9);
        std::vector<std::vector<double>> dark2(size_t n = 8);
        std::vector<std::vector<double>> gnbu(size_t n = 8);
        std::vector<std::vector<double>> gnpu(size_t n = 10);
        std::vector<std::vector<double>> greens(size_t n = 8);
        std::vector<std::vector<double>> greys(size_t n = 8);
        std::vector<std::vector<double>> inferno(size_t n = 256);
        std::vector<std::vector<double>> jet(size_t n = 64);
        std::vector<std::vector<double>> magma(size_t n = 256);
        std::vector<std::vector<double>> oranges(size_t n = 8);
        std::vector<std::vector<double>> orrd(size_t n = 8);
        std::vector<std::vector<double>> paired(size_t n = 8);
        std::vector<std::vector<double>> parula(size_t n = 64);
        std::vector<std::vector<double>> pastel1(size_t n = 8);
        std::vector<std::vector<double>> pastel2(size_t n = 8);
        std::vector<std::vector<double>> piyg(size_t n = 8);
        std::vector<std::vector<double>> plasma(size_t n = 256);
        std::vector<std::vector<double>> prgn(size_t n = 8);
        std::vector<std::vector<double>> pubu(size_t n = 8);
        std::vector<std::vector<double>> pubugn(size_t n = 8);
        std::vector<std::vector<double>> puor(size_t n = 8);
        std::vector<std::vector<double>> purd(size_t n = 8);
        std::vector<std::vector<double>> purples(size_t n = 8);
        std::vector<std::vector<double>> rdbu(size_t n = 8);
        std::vector<std::vector<double>> rdgy(size_t n = 8);
        std::vector<std::vector<double>> rdpu(size_t n = 8);
        std::vector<std::vector<double>> rdylbu(size_t n = 8);
        std::vector<std::vector<double>> rdylgn(size_t n = 8);
        std::vector<std::vector<double>> reds(size_t n = 8);
        std::vector<std::vector<double>> sand(size_t n = 10);
        std::vector<std::vector<double>> set1(size_t n = 8);
        std::vector<std::vector<double>> set2(size_t n = 8);
        std::vector<std::vector<double>> set3(size_t n = 8);
        std::vector<std::vector<double>> spectral(size_t n = 8);
        std::vector<std::vector<double>> viridis(size_t n = 256);
        std::vector<std::vector<double>> whylrd(size_t n = 5);
        std::vector<std::vector<double>> ylgn(size_t n = 8);
        std::vector<std::vector<double>> ylgnbu(size_t n = 8);
        std::vector<std::vector<double>> ylorbr(size_t n = 8);
        std::vector<std::vector<double>> ylorrd(size_t n = 8);
        std::vector<std::vector<double>> ylrd(size_t n = 4);
        std::vector<std::vector<double>> hsv(size_t n = 64);
        std::vector<std::vector<double>> hot(size_t n = 64);
        std::vector<std::vector<double>> cool(size_t n = 64);
        std::vector<std::vector<double>> spring(size_t n = 64);
        std::vector<std::vector<double>> summer(size_t n = 64);
        std::vector<std::vector<double>> autumn(size_t n = 64);
        std::vector<std::vector<double>> winter(size_t n = 64);
        std::vector<std::vector<double>> gray(size_t n = 64);
        std::vector<std::vector<double>> bone(size_t n = 64);
        std::vector<std::vector<double>> copper(size_t n = 64);
        std::vector<std::vector<double>> pink(size_t n = 64);
        std::vector<std::vector<double>> lines(size_t n = 64);
        std::vector<std::vector<double>> colorcube(size_t n = 64);
        std::vector<std::vector<double>> prism(size_t n = 64);
        std::vector<std::vector<double>> flag(size_t n = 64);
        std::vector<std::vector<double>> white(size_t n = 64);
        std::vector<std::vector<double>> default_map(size_t n = 64);
        std::vector<std::vector<double>> default_colors_map(size_t n = 7);
    } // namespace palette

    std::array<float, 4>
    colormap_interpolation(double value, double min, double max,
                           const std::vector<std::vector<double>> &cm);

} // namespace matplot

#endif // MATPLOTPLUSPLUS_COLORS_H
