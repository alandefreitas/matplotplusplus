//
// Created by Alan Freitas on 11/07/20.
//

#include <algorithm>
#include <cmath>
#include <matplot/axes_objects/histogram.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {

    histogram::histogram(class axes_type *parent) : axes_object(parent) {
        if (parent_->y_axis().limits_mode_auto()) {
            parent_->y_axis().limits({0, inf});
        }
    }

    histogram::histogram(class axes_type *parent,
                         const std::vector<double> &data, size_t n_bins,
                         enum histogram::normalization normalization_alg)
        : axes_object(parent), data_(data), num_bins_(n_bins),
          binning_mode_{binning_mode_type::use_fixed_num_bins},
          normalization_{normalization_alg} {
        if (parent_->y_axis().limits_mode_auto()) {
            parent_->y_axis().limits({0, inf});
        }
    }

    histogram::histogram(class axes_type *parent,
                         const std::vector<double> &data,
                         const std::vector<double> &edges,
                         enum histogram::normalization normalization_alg)
        : axes_object(parent), data_(data),
          bin_edges_(edges), binning_mode_{binning_mode_type::use_fixed_edges},
          normalization_{normalization_alg} {
        if (parent_->y_axis().limits_mode_auto()) {
            parent_->y_axis().limits({0, inf});
        }
    }

    histogram::histogram(class axes_type *parent,
                         const std::vector<double> &data,
                         binning_algorithm algorithm,
                         enum histogram::normalization normalization_alg)
        : axes_object(parent), data_(data), algorithm_(algorithm),
          binning_mode_{binning_mode_type::use_algorithm},
          normalization_{normalization_alg} {
        if (parent_->y_axis().limits_mode_auto()) {
            parent_->y_axis().limits({0, inf});
        }
    }

    std::string histogram::plot_string() {
        maybe_update_face_color();
        std::stringstream ss;
        if (!is_polar()) {
            if (!stairs_only_) {
                ss << " '-' with boxes fillstyle solid";
                ss << " border rgb '" + to_string(edge_color_) + "'";
                ss << " fillcolor '" + to_string(face_color_) + "'";
            } else {
                ss << " '-' with boxes fillstyle empty";
                ss << " border rgb '" + to_string(edge_color_) + "'";
                ss << " fillcolor '" + to_string(face_color_) + "'";
            }
        } else {
            if (!stairs_only_) {
                ss << " '-' with filledcurves fillstyle solid";
                ss << " border rgb '" + to_string(edge_color_) + "'";
                ss << " fillcolor '" + to_string(face_color_) + "'";
            } else {
                ss << " '-' with lines linecolor rgb '" +
                          to_string(edge_color_) + "'";
            }
        }
        return ss.str();
    }

    std::string histogram::legend_string(std::string_view title) {
        return " keyentry with boxes fillstyle solid border rgb '" +
               to_string(edge_color_) + "' fillcolor '" +
               to_string(face_color_) + "' title \"" + escape(title) + "\"";
    }

    std::string histogram::data_string() {
        make_sure_data_is_preprocessed();
        std::stringstream ss;
        if (!is_polar()) {
            for (size_t i = 0; i < values_.size(); ++i) {
                // <box center>    <box height>    <box width>
                ss << "    " << (bin_edges_[i] + bin_edges_[i + 1]) * 0.5 << " "
                   << values_[i] << " "
                   << (bin_edges_[i + 1] - bin_edges_[i]) * bar_width_ << "\n";
            }
            ss << "e\n";

        } else {
            // resolutions of the petals
            constexpr double points_per_circle = 90;
            const double n_histogram_bins = bin_edges_.size() - 1.;
            const double points_per_bin = points_per_circle / n_histogram_bins;
            if (!stairs_only_) {
                for (size_t i = 0; i < values_.size(); ++i) {
                    // make a petal for each value
                    // theta = edge_begin           rho = 0
                    // theta = edge_begin           rho = bin_value
                    // theta = edge_end             rho = bin_value
                    // theta = edge_end             rho = 0
                    ss << "    " << bin_edges_[i] << "  " << 0 << "\n";
                    auto arc_between_edges =
                        linspace(bin_edges_[i], bin_edges_[i + 1],
                                 static_cast<size_t>(ceil(points_per_bin)));
                    for (size_t j = 0; j < arc_between_edges.size(); ++j) {
                        ss << "    " << arc_between_edges[j] << "  "
                           << values_[i] << "\n";
                    }
                    ss << "    " << bin_edges_[i + 1] << "  " << 0 << "\n";
                }
            } else {
                for (size_t i = 0; i < values_.size(); ++i) {
                    // stair petals don't start at rho 0
                    // theta = edge_begin           rho = bin_value
                    // theta = edge_end             rho = bin_value
                    // theta = edge_end             rho = next bin value
                    auto arc_between_edges =
                        linspace(bin_edges_[i], bin_edges_[i + 1],
                                 static_cast<size_t>(ceil(points_per_bin)));
                    for (size_t j = 0; j < arc_between_edges.size(); ++j) {
                        ss << "    " << arc_between_edges[j] << "  "
                           << values_[i] << "\n";
                    }
                    ss << "    " << bin_edges_[i + 1] << "  "
                       << values_[(i + 1) % values_.size()] << "\n";
                }
            }

            ss << "e\n";
        }
        return ss.str();
    }

    double histogram::xmax() {
        make_sure_data_is_preprocessed();
        if (!is_polar()) {
            return *std::max_element(bin_edges_.begin(), bin_edges_.end());
        } else {
            return round_polar_max(
                *std::max_element(values_.begin(), values_.end()));
        }
    }

    double histogram::xmin() {
        make_sure_data_is_preprocessed();
        if (!is_polar()) {
            return *std::min_element(bin_edges_.begin(), bin_edges_.end());
        } else {
            return -round_polar_max(
                *std::max_element(values_.begin(), values_.end()));
        }
    }

    double histogram::ymax() {
        make_sure_data_is_preprocessed();
        if (!is_polar()) {
            return *std::max_element(values_.begin(), values_.end());
        } else {
            return round_polar_max(
                *std::max_element(values_.begin(), values_.end()));
        }
    }

    double histogram::ymin() {
        make_sure_data_is_preprocessed();
        if (!is_polar()) {
            return *std::min_element(values_.begin(), values_.end());
        } else {
            return -round_polar_max(
                *std::max_element(values_.begin(), values_.end()));
        }
    }

    enum axes_object::axes_category histogram::axes_category() {
        return !polar_ ? axes_object::axes_category::two_dimensional
                       : axes_object::axes_category::polar;
    }

    void histogram::maybe_update_face_color() {
        if (!manual_face_color_) {
            face_color_ = parent_->get_color_and_bump();
            manual_face_color_ = true;
            // add the default alpha 0.4 we need for histograms
            face_color_[0] = 0.4f + 0.6f * face_color_[0];
        }
        if (stairs_only_ && !manual_edge_color_) {
            if (manual_face_color_) {
                edge_color_ = face_color_;
            } else {
                edge_color_ = parent_->get_color_and_bump();
            }
            manual_edge_color_ = true;
        }
    }

    void histogram::make_sure_data_is_preprocessed() {
        const bool data_is_ok = !values_.empty();
        if (!data_is_ok) {
            switch (binning_mode_) {
            case binning_mode_type::use_algorithm: {
                double minx = *std::min_element(data_.begin(), data_.end());
                double maxx = *std::max_element(data_.begin(), data_.end());
                bin_edges_ =
                    histogram_edges(data_, minx, maxx, algorithm_, false);
                break;
            }
            case binning_mode_type::use_bin_limits: {
                bin_edges_ = histogram_edges(data_, bin_limits_min_,
                                             bin_limits_max_, algorithm_, true);
                break;
            }
            case binning_mode_type::use_fixed_num_bins: {
                double minx = *std::min_element(data_.begin(), data_.end());
                double maxx = *std::max_element(data_.begin(), data_.end());
                double xrange = maxx - minx;
                bin_edges_ =
                    bin_picker(minx, maxx, num_bins_, xrange / num_bins_);
                break;
            }
            case binning_mode_type::use_fixed_bin_width: {
                double minx = *std::min_element(data_.begin(), data_.end());
                double maxx = *std::max_element(data_.begin(), data_.end());
                double xrange = maxx - minx;
                double left_edge = bin_width_ * floor(minx / bin_width_);
                size_t nbins = std::max(
                    static_cast<size_t>(1),
                    static_cast<size_t>(ceil((maxx - left_edge) / bin_width_)));
                constexpr size_t maximum_bins = 65536;
                if (nbins > maximum_bins) {
                    nbins = maximum_bins;
                    bin_width_ = xrange / (maximum_bins - 1);
                    left_edge = bin_width_ * floor(minx / bin_width_);
                    if (maxx <= left_edge + (nbins - 1) * bin_width_) {
                        bin_width_ = xrange / maximum_bins;
                        left_edge = minx;
                    }
                }
                bin_edges_ = transform(
                    iota(0., static_cast<double>(nbins)),
                    [&](double x) { return left_edge + x * bin_width_; });
                break;
            }
            case binning_mode_type::use_fixed_edges: {
                break;
            }
            }
            bin_counts_ = histogram_count(data_, bin_edges_);
            values_ = histogram_normalize(bin_counts_, bin_edges_, data_.size(),
                                          normalization_);
        }
    }

    /// Calculate edges given max values and a target width
    /// This will round the bin edges to values more appropriate for
    /// visualization
    std::vector<double> histogram::bin_picker(double xmin, double xmax,
                                              size_t nbins, double bin_width) {
        double xscale = std::max(std::abs(xmin), std::abs(xmax));
        double xrange = xmax - xmin;
        bin_width = std::max(bin_width, nextafter(xscale, xscale + 1) - xscale);
        double left_edge = 0.;
        double right_edge = 0.;
        double n_bins_actual = 0.;
        const bool data_is_not_constant =
            xrange > std::max(sqrt(nextafter(xscale, xscale + 1) - xscale),
                              std::numeric_limits<double>::min());
        if (data_is_not_constant) {
            double pow_of_ten = pow(10., floor(log10(bin_width)));
            double relative_size = bin_width / pow_of_ten;
            if (nbins == 0) {
                if (relative_size < 1.5) {
                    bin_width = 1 * pow_of_ten;
                } else if (relative_size < 2.5) {
                    bin_width = 2 * pow_of_ten;
                } else if (relative_size < 4) {
                    bin_width = 3 * pow_of_ten;
                } else if (relative_size < 7.5) {
                    bin_width = 5 * pow_of_ten;
                } else {
                    bin_width = 10 * pow_of_ten;
                }
                left_edge = std::max(
                    std::min(bin_width * floor(xmin / bin_width), xmin),
                    -std::numeric_limits<double>::max());
                n_bins_actual =
                    std::max(1., ceil((xmax - left_edge) / bin_width));
                right_edge = std::min(
                    std::max(left_edge + n_bins_actual * bin_width, xmax),
                    std::numeric_limits<double>::max());
            } else {
                bin_width = pow_of_ten * floor(relative_size);
                left_edge = std::max(
                    std::min(bin_width * floor(xmin / bin_width), xmin),
                    -std::numeric_limits<double>::max());
                if (nbins > 1) {
                    double ll = (xmax - left_edge) / nbins;
                    double ul =
                        (xmax - left_edge) / (static_cast<double>(nbins - 1));
                    double p_10 = pow(10, floor(log10(ul - ll)));
                    bin_width = p_10 * ceil(ll / p_10);
                }
                n_bins_actual = static_cast<double>(nbins);
                right_edge = std::min(
                    std::max(left_edge + n_bins_actual * bin_width, xmax),
                    std::numeric_limits<double>::max());
            }
        } else {
            if (nbins == 0) {
                nbins = 1;
            }
            double bin_range = std::max(
                1., ceil(nbins * nextafter(xscale, xscale + 1) - xscale));
            left_edge = floor(2 * (xmin - bin_range / 4)) / 2;
            right_edge = ceil(2 * (xmax + bin_range / 4)) / 2;
            bin_width = (right_edge - left_edge) / nbins;
            n_bins_actual = static_cast<double>(nbins);
        }

        if (!std::isfinite(bin_width)) {
            return linspace(left_edge, right_edge,
                            static_cast<size_t>(n_bins_actual + 1));
        } else {
            std::vector<double> edges;
            edges.emplace_back(left_edge);
            auto middle_edges =
                transform(iota(1, n_bins_actual - 1),
                          [&](double x) { return left_edge + x * bin_width; });
            edges.insert(edges.end(), middle_edges.begin(), middle_edges.end());
            edges.emplace_back(right_edge);
            return edges;
        }
    }

    std::vector<double> bin_pickerbl(double xmin, double xmax, double minlimit,
                                     double maxlimit, double bin_width) {
        double xscale = std::max(std::abs(xmin), std::abs(xmax));
        double xrange = xmax - xmin;
        bin_width = std::max(bin_width, nextafter(xscale, xscale + 1 - xscale));
        bool non_constant_data =
            xrange > std::max(sqrt(nextafter(xscale, xscale + 1 - xscale)),
                              std::numeric_limits<double>::min());
        if (non_constant_data) {
            size_t nbins = std::max(
                static_cast<size_t>(((maxlimit - minlimit) / bin_width) + 0.5),
                static_cast<size_t>(1));
            return linspace(minlimit, maxlimit, nbins + 1);
        } else {
            return std::vector<double>({minlimit, maxlimit});
        }
    }

    std::vector<double> histogram::scotts_rule(const std::vector<double> &x,
                                               double minx, double maxx,
                                               bool hard_limits) {
        double binwidth =
            3.5 * stddev(x) / (pow(static_cast<double>(x.size()), 1. / 3.));
        if (!hard_limits) {
            return bin_picker(minx, maxx, 0, binwidth);
        } else {
            double max_x = *std::max_element(x.begin(), x.end());
            double min_x = *std::min_element(x.begin(), x.end());
            return bin_pickerbl(min_x, max_x, minx, maxx, binwidth);
        }
    }

    std::vector<double> histogram::fd_rule(const std::vector<double> &x,
                                           double minx, double maxx,
                                           bool hard_limits) {
        size_t n = x.size();
        double xrange = *std::max_element(x.begin(), x.end()) -
                        *std::min_element(x.begin(), x.end());
        double bin_width = 1.0;
        bool iqr_not_too_small = n > 1;
        if (iqr_not_too_small) {
            size_t q1_index = static_cast<size_t>(n * 0.25);
            size_t q3_index = n - static_cast<size_t>(n * 0.25);
            auto x_copy = x;
            std::nth_element(x_copy.begin(), x_copy.begin() + q1_index,
                             x_copy.end());
            std::nth_element(x_copy.begin(), x_copy.begin() + q3_index,
                             x_copy.end());
            double interquartile_range = x_copy[q3_index] - x_copy[q1_index];
            double iq = std::max(interquartile_range, xrange / 10.);
            bin_width = 2 * iq * pow(n, -1. / 3.);
        }
        if (!hard_limits) {
            return bin_picker(minx, maxx, 0, bin_width);
        } else {
            double max_x = *std::max_element(x.begin(), x.end());
            double min_x = *std::min_element(x.begin(), x.end());
            return bin_pickerbl(min_x, max_x, minx, maxx, bin_width);
        }
    }

    std::vector<double> histogram::integers_rule(const std::vector<double> &x,
                                                 double minx, double maxx,
                                                 bool hard_limits) {
        constexpr size_t max_num_of_bins = 65536;
        double xrange = maxx - minx;
        double binwidth = 1.0;
        if (!x.empty()) {
            std::vector abs_x =
                transform(x, [](double x) { return std::abs(x); });
            double xscale = *std::max_element(abs_x.begin(), abs_x.end());
            xrange = *std::max_element(x.begin(), x.end()) -
                     *std::min_element(x.begin(), x.end());
            if (xrange > max_num_of_bins) {
                binwidth = pow(10, ceil(log10(xrange / max_num_of_bins)));
            } else if (nextafter(xscale, xscale + 1) - xscale > 1.) {
                binwidth = pow(
                    10, ceil(log10(nextafter(xscale, xscale + 1) - xscale)));
            } else {
                binwidth = 1.;
            }
            if (!hard_limits) {
                minx = binwidth * round(minx / binwidth);
                maxx = binwidth * round(maxx / binwidth);
                return iota(floor(minx) - .5 * binwidth, binwidth,
                            ceil(maxx) + .5 * binwidth);
            } else {
                double minxi = binwidth * ceil(minx / binwidth) + 0.5;
                double maxxi = binwidth * floor(maxx / binwidth) - 0.5;
                std::vector<double> edges = {minx};
                auto mid = iota(minxi, binwidth, maxxi);
                edges.insert(edges.end(), mid.begin(), mid.end());
                edges.emplace_back(maxx);
                return edges;
            }
        } else {
            if (!hard_limits) {
                return std::vector<double>{-0.5, 0.5};
            } else {
                double minxi = ceil(minx) + 0.5;
                double maxxi = floor(maxx) - 0.5;
                std::vector<double> edges = {minx};
                std::vector<double> mid = iota(minxi, maxxi);
                edges.insert(edges.end(), mid.begin(), mid.end());
                edges.emplace_back(maxx);
                return edges;
            }
        }
    }

    std::vector<double> histogram::sqrt_rule(const std::vector<double> &x,
                                             double minx, double maxx,
                                             bool hard_limits) {
        size_t nbins =
            static_cast<size_t>(std::max(ceil(log2(x.size()) + 1.), 1.));
        if (!hard_limits) {
            double binwidth = (maxx - minx) / nbins;
            if (std::isfinite(binwidth)) {
                return bin_picker(minx, maxx, 0, binwidth);
            } else {
                return bin_picker(minx, maxx, nbins, binwidth);
            }
        } else {
            return linspace(minx, maxx, nbins + 1);
        }
    }

    std::vector<double> histogram::sturges_rule(const std::vector<double> &x,
                                                double minx, double maxx,
                                                bool hard_limits) {
        size_t nbins =
            static_cast<size_t>(std::max(ceil(log2(x.size()) + 1.), 1.));
        if (!hard_limits) {
            double binwidth = (maxx - minx) / nbins;
            if (std::isfinite(binwidth)) {
                return bin_picker(minx, maxx, 0, binwidth);
            } else {
                return bin_picker(minx, maxx, nbins, binwidth);
            }
        } else {
            return linspace(minx, maxx, nbins + 1);
        }
    }

    std::vector<double> histogram::automatic_rule(const std::vector<double> &x,
                                                  double minx, double maxx,
                                                  bool hard_limits) {
        double xrange = maxx - minx;
        bool is_around_integers = std::all_of(x.begin(), x.end(), [](double x) {
            return std::abs(x - round(x)) < 0.01;
        });
        if (is_around_integers && xrange <= 50) {
            return integers_rule(x, minx, maxx, hard_limits);
        } else {
            return scotts_rule(x, minx, maxx, hard_limits);
        }
    }

    std::vector<double>
    histogram::histogram_edges(const std::vector<double> &data, double minx,
                               double maxx, binning_algorithm algorithm,
                               bool hard_limits) {
        switch (algorithm) {
        case binning_algorithm::automatic:
            return automatic_rule(data, minx, maxx, hard_limits);
        case binning_algorithm::scott:
            return scotts_rule(data, minx, maxx, hard_limits);
        case binning_algorithm::fd:
            return fd_rule(data, minx, maxx, hard_limits);
        case binning_algorithm::integers:
            return integers_rule(data, minx, maxx, hard_limits);
        case binning_algorithm::sturges:
            return sturges_rule(data, minx, maxx, hard_limits);
        case binning_algorithm::sqrt:
            return sqrt_rule(data, minx, maxx, hard_limits);
        }
        throw std::logic_error("histogram::histrogram_edges: could not find the binning algorithm");
    }

    std::vector<size_t>
    histogram::histogram_count(const std::vector<double> &data,
                               const std::vector<double> &edges) {
        std::vector<size_t> bin_counts(edges.size() - 1, 0);
        for (const double &v : data) {
            // find first edge that does not compare less than v
            auto it = std::lower_bound(edges.begin(), edges.end(), v);
            bool out_of_range = it == edges.begin() || it == edges.end();
            if (!out_of_range) {
                ++bin_counts[it - edges.begin() - 1];
            } else if (it == edges.begin()) {
                if (v == *it) {
                    ++bin_counts[0];
                }
            }
        }
        return bin_counts;
    }

    std::vector<double>
    histogram::histogram_normalize(const std::vector<size_t> &bin_count,
                                   const std::vector<double> &bin_edges,
                                   size_t data_size,
                                   enum normalization normalization_algorithm) {
        std::vector<double> values(bin_count.size());
        switch (normalization_algorithm) {
        case normalization::count:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                values[i] = static_cast<double>(bin_count[i]);
            }
            break;
        case normalization::count_density:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                const double bin_width_i = bin_edges[i + 1] - bin_edges[i];
                values[i] = static_cast<double>(bin_count[i]) / bin_width_i;
            }
            break;
        case normalization::cummulative_count:
            values[0] = static_cast<double>(bin_count[0]);
            for (size_t i = 1; i < bin_count.size(); ++i) {
                values[i] = bin_count[i] + values[i - 1];
            }
            break;
        case normalization::probability:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                values[i] = static_cast<double>(bin_count[i]) / data_size;
            }
            break;
        case normalization::pdf:
            for (size_t i = 0; i < bin_count.size(); ++i) {
                const double bin_width_i = bin_edges[i + 1] - bin_edges[i];
                values[i] = static_cast<double>(bin_count[i]) /
                            (bin_width_i * data_size);
            }
            break;
        case normalization::cdf:
            double sum = 0.0;
            for (size_t i = 1; i < bin_count.size(); ++i) {
                sum += bin_count[i];
                values[i] = sum / data_size;
            }
            break;
        }
        return values;
    }

    size_t histogram::num_bins() {
        if (binning_mode_ == binning_mode_type::use_fixed_num_bins) {
            return num_bins_;
        } else {
            make_sure_data_is_preprocessed();
            return bin_counts_.size();
        }
    }

    const std::vector<double> &histogram::data() const { return data_; }

    class histogram &histogram::data(const std::vector<double> &data) {
        data_ = data;
        touch();
        return *this;
    }

    const std::vector<double> &histogram::values() const { return values_; }

    class histogram &histogram::values(const std::vector<double> &values) {
        values_ = values;
        touch();
        return *this;
    }

    const std::vector<size_t> &histogram::bin_counts() const {
        return bin_counts_;
    }

    class histogram &
    histogram::bin_counts(const std::vector<size_t> &bin_counts) {
        bin_counts_ = bin_counts;
        touch();
        return *this;
    }

    class histogram &histogram::num_bins(size_t num_bins) {
        num_bins_ = num_bins;
        values_.clear();
        binning_mode_ = binning_mode_type::use_fixed_num_bins;
        touch();
        return *this;
    }

    matplot::histogram::binning_algorithm histogram::algorithm() const {
        return algorithm_;
    }

    class histogram &
    histogram::algorithm(matplot::histogram::binning_algorithm algorithm) {
        algorithm_ = algorithm;
        touch();
        return *this;
    }

    std::vector<double> &histogram::bin_edges() {
        make_sure_data_is_preprocessed();
        return bin_edges_;
    }

    class histogram &
    histogram::bin_edges(const std::vector<double> &bin_edges) {
        bin_edges_ = bin_edges;
        values_.clear();
        binning_mode_ = binning_mode_type::use_fixed_edges;
        touch();
        return *this;
    }

    double histogram::bin_width() const { return bin_width_; }

    class histogram &histogram::bin_width(double bin_width) {
        if (bin_width != bin_width_) {
            bin_width_ = bin_width;
            binning_mode_ = binning_mode_type::use_fixed_bin_width;
            values_.clear();
            touch();
        }
        return *this;
    }

    double histogram::bin_limits_min() const { return bin_limits_min_; }

    class histogram &histogram::bin_limits_min(double bin_limits_min) {
        bin_limits_min_ = bin_limits_min;
        touch();
        return *this;
    }

    double histogram::bin_limits_max() const { return bin_limits_max_; }

    class histogram &histogram::bin_limits_max(double bin_limits_max) {
        bin_limits_max_ = bin_limits_max;
        touch();
        return *this;
    }

    enum histogram::normalization histogram::normalization() const {
        return normalization_;
    }

    class histogram &
    histogram::normalization(enum histogram::normalization normalization) {
        if (normalization != normalization_) {
            normalization_ = normalization;
            values_.clear();
            touch();
        }
        return *this;
    }

    const color_array &histogram::face_color() const { return face_color_; }

    class histogram &histogram::face_color(const color_array &face_color) {
        face_color_ = face_color;
        touch();
        return *this;
    }

    class histogram &histogram::face_color(std::initializer_list<float> color) {
        face_color(to_array<4>(color));
        return *this;
    }

    class histogram &histogram::face_color(std::string_view color) {
        face_color(to_array(color));
        return *this;
    }

    class histogram &histogram::face_alpha(float alpha) {
        face_color_[0] = alpha;
        touch();
        return *this;
    }

    class histogram &histogram::edge_alpha(float alpha) {
        edge_color_[0] = alpha;
        touch();
        return *this;
    }

    bool histogram::manual_face_color() const { return manual_face_color_; }

    class histogram &histogram::manual_face_color(bool manual_face_color) {
        manual_face_color_ = manual_face_color;
        touch();
        return *this;
    }

    const color_array &histogram::edge_color() const { return edge_color_; }

    class histogram &histogram::edge_color(const color_array &edge_color) {
        edge_color_ = edge_color;
        manual_edge_color_ = true;
        touch();
        return *this;
    }

    class histogram &histogram::edge_color(std::initializer_list<float> color) {
        edge_color(to_array<4>(color));
        return *this;
    }

    class histogram &histogram::edge_color(std::string_view color) {
        edge_color(to_array(color));
        return *this;
    }

    const line_spec &histogram::edge_style() const { return edge_style_; }

    class histogram &histogram::edge_style(const line_spec &edge_style) {
        edge_style_ = edge_style;
        touch();
        return *this;
    }

    float histogram::line_width() const { return line_width_; }

    class histogram &histogram::line_width(float line_width) {
        line_width_ = line_width;
        touch();
        return *this;
    }

    bool histogram::vertical_orientation() const {
        return vertical_orientation_;
    }

    class histogram &
    histogram::vertical_orientation(bool vertical_orientation) {
        vertical_orientation_ = vertical_orientation;
        touch();
        return *this;
    }

    bool histogram::visible() const { return visible_; }

    class histogram &histogram::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float histogram::bar_width() const { return bar_width_; }

    class histogram &histogram::bar_width(float bar_width) {
        if (bar_width != bar_width_) {
            bar_width_ = bar_width;
            touch();
        }
        return *this;
    }

    bool histogram::polar() const { return polar_; }

    class histogram &histogram::polar(bool polar) {
        polar_ = polar;
        touch();
        return *this;
    }

    bool histogram::stairs_only() const { return stairs_only_; }

    class histogram &histogram::stairs_only(bool stairs_only) {
        stairs_only_ = stairs_only;
        touch();
        return *this;
    }

    size_t histogram::morebins(double bin_increase) {
        size_t old_size = this->num_bins();
        size_t new_size = static_cast<size_t>(
            ceil(static_cast<double>(this->num_bins()) * (1. + bin_increase)));
        new_size += new_size == old_size;
        new_size = std::max(new_size, size_t(1));
        this->num_bins(new_size);
        return this->num_bins();
    }

    size_t histogram::fewerbins(double bin_decrease) {
        size_t old_size = this->num_bins();
        size_t new_size = static_cast<size_t>(
            ceil(static_cast<double>(this->num_bins()) * (1. - bin_decrease)));
        new_size -= new_size == old_size;
        new_size = std::max(new_size, size_t(1));
        this->num_bins(new_size);
        return this->num_bins();
    }

} // namespace matplot