//
// Created by Alan Freitas on 20/07/20.
//

#ifndef MATPLOTPLUSPLUS_GEODATA_H
#define MATPLOTPLUSPLUS_GEODATA_H

#include <string>
#include <vector>

namespace matplot {
    std::pair<std::vector<double>, std::vector<double>> &world_map_10m();
    std::pair<std::vector<double>, std::vector<double>> &world_map_50m();
    std::pair<std::vector<double>, std::vector<double>> &world_map_110m();
    std::tuple<std::vector<double>, std::vector<double>,
               std::vector<std::string>> &
    world_cities();
    std::tuple<std::vector<double>, std::vector<double>,
               std::vector<std::string>>
    world_cities(double min_x_distance_per_char, double min_y_distance);

    std::tuple<std::vector<double>, std::vector<double>>
    greedy_tsp(const std::vector<double> &x, const std::vector<double> &y,
               size_t starting_city = 0);

    std::tuple<std::vector<double>, std::vector<double>, std::vector<size_t>>
    greedy_tsp_with_idx(const std::vector<double> &x,
                        const std::vector<double> &y, size_t starting_city = 0);

    std::tuple<std::vector<double>, std::vector<double>,
               std::vector<std::string>>
    clear_overlapping_labels(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<std::string> &names,
                             double min_x_distance_per_char,
                             double min_y_distance);

} // namespace matplot

#endif // MATPLOTPLUSPLUS_GEODATA_H
