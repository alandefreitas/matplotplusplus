//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_HISTOGRAM_H
#define MATPLOTPLUSPLUS_HISTOGRAM_H

#include <matplot/detail/config.h>
#include <matplot/core/axes_object.h>
#include <matplot/core/figure_type.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/common.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>
#include <array>
#include <cmath>
#include <map>

namespace matplot {
    class axes_type;
    class MATPLOT_EXPORTS histogram : public axes_object {
      public:
        // source code in 'edit histcount'
        enum class binning_algorithm {
            automatic, // Scott's rule for doubles or integers rule for data
                       // distributed around integers
            scott,     // Scott’s rule - bin_width = 3.5*std(x)*|X|^(-1/3)
            fd,        // Freedman-Diaconis rule - bin width =
                // 2*IQR(X(:))*numel(X)^(-1/3) (for heavy tailed distributions,
                // less sensitive to outliers) - IQR = interquartile range of X
            integers, // Integer rule - bin width = 1
            sturges,  // Sturges' rule - number of bins = ceil(1+log2(numel(X)))
            sqrt // Square root rule - number of bins = ceil(sqrt(numel(X)))
        };

        enum class normalization {
            count,             // value_i = count_i
            count_density,     // value_i = count_i / bin_width_i
            cummulative_count, // value_i = sum_j=1^i count_j
            probability, // value_i = count_i / size -> (sum(heights) <= 1)
            pdf, // value_i = count_i / (size * width_i) -> (sum(areas) <= 1)
            cdf  // value_i = sum_j=1^i count_j / size
        };

      public:
        explicit histogram(class axes_type *parent);
        histogram(class axes_type *parent, const std::vector<double> &data,
                  size_t n_bins,
                  enum histogram::normalization normalization_alg =
                      histogram::normalization::count);
        histogram(class axes_type *parent, const std::vector<double> &data,
                  const std::vector<double> &edges,
                  enum histogram::normalization normalization_alg =
                      histogram::normalization::count);
        histogram(class axes_type *parent, const std::vector<double> &data,
                  binning_algorithm algorithm = binning_algorithm::automatic,
                  histogram::normalization normalization_alg =
                      histogram::normalization::count);

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        histogram(const axes_handle &parent, Args&&... args)
            : histogram(parent.get(), std::forward<Args>(args)...) {}

        virtual ~histogram() = default;
      public /* xlim object virtual functions */:
        // std::string set_variables_string() override;
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        // std::string unset_variables_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* useful functions for histograms */:
        /// Increase number of bins
        size_t morebins(double bin_increase = 0.1);

        /// Decrease number of bins
        size_t fewerbins(double bin_decrease = 0.1);

        /// Find appropriate edges given a fixed number of bins and target bin
        /// width If nbins == 0, round the bin width and create
        /// (xmax-xmin)/bin_width bins If nbins != 0, bin width is set to
        /// (xmax-xmin)/nbins and create nbins bins
        static std::vector<double> bin_picker(double xmin, double xmax,
                                              size_t nbins, double bin_width);

        /// Find appropriate edges with a given algorithm
        static std::vector<double> histogram_edges(
            const std::vector<double> &data, double minx, double maxx,
            binning_algorithm algorithm = binning_algorithm::automatic,
            bool hard_limits = false);

        /// Count points within each pair of edges
        static std::vector<size_t>
        histogram_count(const std::vector<double> &values,
                        const std::vector<double> &edges);

        /// Normalize the instagram count
        static std::vector<double>
        histogram_normalize(const std::vector<size_t> &bin_count,
                            const std::vector<double> &bin_edges,
                            size_t data_size,
                            enum normalization normalization_algorithm);

      public /* getters and setters */:
        const std::vector<double> &data() const;
        class histogram &data(const std::vector<double> &data);

        const std::vector<double> &values() const;
        class histogram &values(const std::vector<double> &values);

        const std::vector<size_t> &bin_counts() const;
        class histogram &bin_counts(const std::vector<size_t> &bin_counts);

        size_t num_bins();
        class histogram &num_bins(size_t num_bins);

        binning_algorithm algorithm() const;
        class histogram &algorithm(binning_algorithm algorithm);

        std::vector<double> &bin_edges();
        class histogram &bin_edges(const std::vector<double> &bin_edges);

        double bin_width() const;
        class histogram &bin_width(double bin_width);

        double bin_limits_min() const;
        class histogram &bin_limits_min(double bin_limits_min);

        double bin_limits_max() const;
        class histogram &bin_limits_max(double bin_limits_max);

        normalization normalization() const;
        class histogram &normalization(enum normalization normalization);

        const color_array &face_color() const;
        class histogram &face_color(const color_array &face_color);
        class histogram &face_color(std::initializer_list<float> face_color);
        class histogram &face_color(std::string_view color);

        class histogram &face_alpha(float alpha);
        class histogram &edge_alpha(float alpha);

        bool manual_face_color() const;
        class histogram &manual_face_color(bool manual_face_color);

        const color_array &edge_color() const;
        class histogram &edge_color(const color_array &edge_color);
        class histogram &edge_color(std::initializer_list<float> face_color);
        class histogram &edge_color(std::string_view edge_color);

        const line_spec &edge_style() const;
        class histogram &edge_style(const line_spec &edge_style);

        float line_width() const;
        class histogram &line_width(float line_width);

        bool vertical_orientation() const;
        class histogram &vertical_orientation(bool vertical_orientation);

        bool visible() const;
        class histogram &visible(bool visible);

        float bar_width() const;
        class histogram &bar_width(float bar_width);

        bool polar() const;
        class histogram &polar(bool polar);

        bool stairs_only() const;
        class histogram &stairs_only(bool stairs_only);

        /// Algorithms to determine histogram edges
        static std::vector<double> scotts_rule(const std::vector<double> &x,
                                               double minx, double maxx,
                                               bool hard_limits);
        static std::vector<double> fd_rule(const std::vector<double> &x,
                                           double minx, double maxx,
                                           bool hard_limits);
        static std::vector<double> integers_rule(const std::vector<double> &x,
                                                 double minx, double maxx,
                                                 bool hard_limits);
        static std::vector<double> sqrt_rule(const std::vector<double> &x,
                                             double minx, double maxx,
                                             bool hard_limits);
        static std::vector<double> sturges_rule(const std::vector<double> &x,
                                                double minx, double maxx,
                                                bool hard_limits);
        static std::vector<double> automatic_rule(const std::vector<double> &x,
                                                  double minx, double maxx,
                                                  bool hard_limits);

      public /* getters and setters bypassing the histogram_spec */:
      private:
        /// If the user has not set the face color,
        /// we get a color from the xlim
        void maybe_update_face_color();

        /// Make sure we have already calculated the normalized values
        /// for the current parameters. We reset the normalized values
        /// every time the user changes the algorithm.
        void make_sure_data_is_preprocessed();

      protected:
        // original data
        std::vector<double> data_;

        // normalized values (in the simplest case, values_ = bin_counts_)
        std::vector<double> values_;

        // number of elements in each bin
        std::vector<size_t> bin_counts_;

        // target num of bins (0 indicates no target: the algorithm should
        // decide)
        size_t num_bins_{0};

        // algorithm we use to create the bins
        binning_algorithm algorithm_;

        // where edges start
        std::vector<double> bin_edges_;

        // binning mode
        enum binning_mode_type {
            use_algorithm,       // use algorithm to decide everything
            use_bin_limits,      // use algorithm with bin limits
            use_fixed_num_bins,  // use fixed num of bins
            use_fixed_bin_width, // use fixed num of bins
            use_fixed_edges,     // use fixed edges, including edges of variable
                                 // size
        };
        binning_mode_type binning_mode_{binning_mode_type::use_algorithm};

        // bin width (if zero, the algorithm determines the bin width)
        double bin_width_{0.0};

        // bin limits
        double bin_limits_min_;
        double bin_limits_max_;

        // normalization algorithm
        enum normalization normalization_ { normalization::count };

        // color and style
        color_array face_color_{0.4f, 0, 0, 0};
        bool manual_face_color_{false};
        color_array edge_color_{0, 0, 0, 0};
        bool manual_edge_color_{false};
        line_spec edge_style_{"-"};
        float line_width_{0.5};
        bool vertical_orientation_{true};
        float bar_width_{1.0};
        bool polar_{false};
        bool stairs_only_{false};

        // True if visible
        bool visible_{true};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_HISTOGRAM_H
