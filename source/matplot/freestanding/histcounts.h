//
// Created by Alan Freitas on 10/08/20.
//

#ifndef MATPLOTPLUSPLUS_HISTCOUNTS_H
#define MATPLOTPLUSPLUS_HISTCOUNTS_H

#include <vector>

#include <matplot/axes_objects/histogram.h>

namespace matplot {
    /// Histogram count with custom binning and custom normalization
    /// \return Pair with <values per bin, edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data,
               enum histogram::binning_algorithm binning_alg =
                   histogram::binning_algorithm::automatic,
               enum histogram::normalization normalization_alg =
                   histogram::normalization::count);

    /// Histogram count with automatic binning and custom normalization
    /// \return pair with <values,edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data,
               enum histogram::normalization normalization_alg);

    /// Histogram count with fixed number of bins
    /// \return pair with <values,edges>
    std::pair<std::vector<double>, std::vector<double>>
    histcounts(const std::vector<double> &data, size_t nbins,
               enum histogram::normalization normalization_alg =
                   histogram::normalization::count);

    /// Histogram count with fixed edges
    /// \return normalized values for each edge
    std::vector<double>
    histcounts(const std::vector<double> &data,
               const std::vector<double> &edges,
               enum histogram::normalization normalization_alg =
                   histogram::normalization::count);

    /// Normalize the number of points in a bin
    std::vector<std::vector<double>>
    histnormalize2(const std::vector<std::vector<size_t>> &bin_count,
                   const std::vector<double> &xbin_edges,
                   const std::vector<double> &ybin_edges, size_t data_size,
                   enum histogram::normalization normalization_algorithm);

    /// Count number of points in each bin defined by the 2 dimensional edges
    std::vector<std::vector<double>> histcounts2(
        const std::vector<double> &x_data, const std::vector<double> &y_data,
        const std::vector<double> &x_edges, const std::vector<double> &y_edges,
        enum histogram::normalization normalization_algorithm =
            histogram::normalization::count);
} // namespace matplot

#endif // MATPLOTPLUSPLUS_HISTCOUNTS_H
