//
// Created by Alan Freitas on 13/08/20.
//

#include <algorithm>
#include <matplot/freestanding/histcounts.h>

namespace matplot {
    /// Histogram count with custom binning and custom normalization
    /// \return Pair with <values per bin, edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data,
               enum histogram::binning_algorithm binning_alg,
               enum histogram::normalization normalization_alg) {
        double minx = *std::min_element(data.begin(), data.end());
        double maxx = *std::max_element(data.begin(), data.end());
        auto edges =
            histogram::histogram_edges(data, minx, maxx, binning_alg, false);
        auto bin_counts = histogram::histogram_count(data, edges);
        return std::make_pair(histogram::histogram_normalize(bin_counts, edges,
                                                             data.size(),
                                                             normalization_alg),
                              edges);
    }

    /// Histogram count with automatic binning and custom normalization
    /// \return pair with <values,edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data,
               enum histogram::normalization normalization_alg) {
        return histcounts(data, histogram::binning_algorithm::automatic,
                          normalization_alg);
    }

    /// Histogram count with fixed number of bins
    /// \return pair with <values,edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data, size_t nbins,
               enum histogram::normalization normalization_alg) {
        double minx = *std::min_element(data.begin(), data.end());
        double maxx = *std::max_element(data.begin(), data.end());
        double xrange = maxx - minx;
        auto edges = histogram::bin_picker(minx, maxx, nbins, xrange / nbins);
        auto bin_counts = histogram::histogram_count(data, edges);
        return std::make_pair(histogram::histogram_normalize(bin_counts, edges,
                                                             data.size(),
                                                             normalization_alg),
                              edges);
    }

    /// Histogram count with fixed edges
    /// \return normalized values for each edge
    std::vector<double>
    histcounts(const std::vector<double> &data,
               const std::vector<double> &edges,
               enum histogram::normalization normalization_alg) {
        auto bin_counts = histogram::histogram_count(data, edges);
        return histogram::histogram_normalize(bin_counts, edges, data.size(),
                                              normalization_alg);
    }

    /// Normalize the number of points in a bin
    std::vector<std::vector<double>>
    histnormalize2(const std::vector<std::vector<size_t>> &bin_count,
                   const std::vector<double> &xbin_edges,
                   const std::vector<double> &ybin_edges, size_t data_size,
                   enum histogram::normalization normalization_algorithm) {
        std::vector<std::vector<double>> values(
            bin_count.size(), std::vector<double>(bin_count[0].size(), 0));
        switch (normalization_algorithm) {
        case histogram::normalization::count:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                for (size_t j = 0; j < bin_count[0].size(); ++j) {
                    values[i][j] = static_cast<double>(bin_count[i][j]);
                }
            }
            break;
        case histogram::normalization::count_density:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                for (size_t j = 0; j < bin_count[0].size(); ++j) {
                    const double x_bin_width_i =
                        xbin_edges[i + 1] - xbin_edges[i];
                    const double y_bin_width_i =
                        ybin_edges[j + 1] - ybin_edges[j];
                    const double bin_area_i = x_bin_width_i * y_bin_width_i;
                    values[i][j] =
                        static_cast<double>(bin_count[i][j]) / bin_area_i;
                }
            }
            break;
        case histogram::normalization::cummulative_count:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                if (i == 0) {
                    values[0][0] = static_cast<double>(bin_count[0][0]);
                } else {
                    values[i][0] =
                        static_cast<double>(bin_count[i][0]) + values[i - 1][0];
                }
                double line_sum = values[i][0];
                for (size_t j = 1; j < bin_count[0].size(); ++j) {
                    if (i == 0) {
                        values[i][j] = bin_count[i][j] + line_sum;
                    } else {
                        values[i][j] =
                            bin_count[i][j] + line_sum + values[i - 1][j];
                    }
                    line_sum += bin_count[i][j];
                }
            }
            break;
        case histogram::normalization::probability:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                for (size_t j = 0; j < bin_count[0].size(); ++j) {
                    // const double x_bin_width_i = xbin_edges[i + 1] -
                    // xbin_edges[i]; const double y_bin_width_i = ybin_edges[j
                    // + 1] - ybin_edges[j]; const double bin_area_i =
                    // x_bin_width_i*y_bin_width_i;
                    values[i][j] =
                        static_cast<double>(bin_count[i][j]) / data_size;
                }
            }
            break;
        case histogram::normalization::pdf:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                for (size_t j = 0; j < bin_count[0].size(); ++j) {
                    const double x_bin_width_i =
                        xbin_edges[i + 1] - xbin_edges[i];
                    const double y_bin_width_i =
                        ybin_edges[j + 1] - ybin_edges[j];
                    const double bin_area_i = x_bin_width_i * y_bin_width_i;
                    values[i][j] = static_cast<double>(bin_count[i][j]) /
                                   (bin_area_i * data_size);
                }
            }
            break;
        case histogram::normalization::cdf:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                if (i == 0) {
                    values[0][0] =
                        static_cast<double>(bin_count[0][0] / data_size);
                } else {
                    values[i][0] =
                        bin_count[i][0] / data_size + values[i - 1][0];
                }
                double line_sum = values[i][0];
                for (size_t j = 1; j < bin_count[0].size(); ++j) {
                    if (i == 0) {
                        values[i][j] = (bin_count[i][j] + line_sum) / data_size;
                    } else {
                        values[i][j] =
                            (bin_count[i][j] + line_sum) / data_size +
                            values[i - 1][j];
                    }
                    line_sum += bin_count[i][j];
                }
            }
            break;
        }
        return values;
    }

    /// Count number of points in each bin defined by the 2 dimensional edges
    std::vector<std::vector<double>> histcounts2(
        const std::vector<double> &x_data, const std::vector<double> &y_data,
        const std::vector<double> &x_edges, const std::vector<double> &y_edges,
        enum histogram::normalization normalization_algorithm) {
        std::vector<std::vector<size_t>> bin_counts(
            x_edges.size() - 1, std::vector<size_t>(y_edges.size() - 1, 0));
        for (size_t i = 0; i < x_data.size(); ++i) {
            double xi = x_data[i];
            double yi = y_data[i];
            // find first edge that does not compare less than v
            auto itx = std::lower_bound(x_edges.begin(), x_edges.end(), xi);
            bool xout_of_range = itx == x_edges.begin() || itx == x_edges.end();
            if (!xout_of_range) {
                auto ity = std::lower_bound(y_edges.begin(), y_edges.end(), yi);
                bool yout_of_range =
                    ity == y_edges.begin() || ity == y_edges.end();
                if (!yout_of_range) {
                    ++bin_counts[itx - x_edges.begin() - 1]
                                [ity - y_edges.begin() - 1];
                }
            }
        }
        return histnormalize2(bin_counts, x_edges, y_edges, x_data.size(),
                              normalization_algorithm);
    }
} // namespace matplot