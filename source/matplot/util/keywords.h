//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_KEYWORDS_H
#define MATPLOTPLUSPLUS_KEYWORDS_H

#include <limits>

namespace matplot {
    class keyword_automatic_type {};
    constexpr keyword_automatic_type automatic;

    class keyword_manual_type {};
    constexpr keyword_manual_type manual;

    class keyword_equal_type {};
    constexpr keyword_equal_type equal;

    class keyword_square_type {};
    constexpr keyword_square_type square;

    class keyword_tight_type {};
    constexpr keyword_tight_type tight;

    class keyword_ij_type {};
    constexpr keyword_ij_type ij;

    class keyword_minor_type {};
    constexpr keyword_minor_type minor;

    constexpr bool on = true;
    constexpr bool off = false;

    constexpr bool left = false;
    constexpr bool right = true;

    constexpr double inf = std::numeric_limits<double>::infinity();

} // namespace matplot

#endif // MATPLOTPLUSPLUS_KEYWORDS_H
