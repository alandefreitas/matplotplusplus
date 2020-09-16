//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_NETWORK_H
#define MATPLOTPLUSPLUS_NETWORK_H

#include <array>
#include <matplot/core/figure_type.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/common.h>

namespace matplot {
    class axes_type;
    class network : public axes_object {
      public:
        enum class layout { automatic, force, circle, kawai, random };

      public:
        explicit network(class axes_type *parent);
        network(class axes_type *parent,
                const std::vector<std::pair<size_t, size_t>> &edges,
                const std::vector<double> &weights, size_t n_vertices,
                std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        network(const axes_handle &parent, Args&&... args)
            : network(parent.get(), std::forward<Args>(args)...) {}

        virtual ~network() = default;
      public /* mandatory virtual functions */:
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        class network &line_style(std::string_view line_spec);

        const matplot::line_spec &line_spec() const;
        matplot::line_spec &line_spec();
        class network &line_spec(const class line_spec &line_spec);

        const std::vector<double> &y_data() const;
        class network &y_data(const std::vector<double> &y_data);

        const std::vector<double> &x_data() const;
        class network &x_data(const std::vector<double> &x_data);

        const std::vector<double> &z_data() const;
        class network &z_data(const std::vector<double> &z_data);

        const std::vector<size_t> &marker_indices() const;
        class network &
        marker_indices(const std::vector<size_t> &marker_indices);

        bool use_y2() const;
        class network &use_y2(bool use_y_2);

        bool use_y_2() const;
        class network &use_y_2(bool use_y_2);

        bool show_labels() const;
        class network &show_labels(bool show_labels);

        bool directed() const;
        class network &directed(bool directed);

        const std::vector<std::string> &edge_labels() const;
        class network &edge_labels(const std::vector<std::string> &edge_labels);

        template <class C>
        network &edge_labels(const IterableValues<C> &e_labels) {
            std::vector<std::string> str_labels;
            std::stringstream ss;
            for (const auto &edge_label : e_labels) {
                ss << "{/:Italic " << edge_label << " }";
                str_labels.emplace_back(ss.str());
                ss.str("");
            }
            edge_labels(str_labels);
            return *this;
        }

        const std::vector<std::string> &node_labels() const;
        class network &node_labels(const std::vector<std::string> &node_labels);

        template <class C>
        network &node_labels(const IterableValues<C> &e_labels) {
            std::vector<std::string> str_labels;
            std::stringstream ss;
            for (const auto &edge_label : e_labels) {
                ss << edge_label;
                str_labels.emplace_back(ss.str());
                ss.str("");
            }
            node_labels(str_labels);
            return *this;
        }

        const std::vector<double> &line_widths() const;
        class network &line_widths(const std::vector<double> &line_widths);

        const std::vector<std::pair<size_t, size_t>> &edges() const;
        class network &
        edges(const std::vector<std::pair<size_t, size_t>> &edges);

        class network &n_vertices(size_t n_vertices);

        const vector_1d &weights() const;
        class network &weights(const vector_1d &weights);

        layout layout_algorithm() const;
        class network &layout_algorithm(layout layout_algorithm);

        double layout_k() const;
        class network &layout_k(double layout_k);

        int layout_iterations() const;
        class network &layout_iterations(int layout_iterations);

        double energy_threshold() const;
        class network &energy_threshold(double energy_threshold);

        const std::vector<float> &marker_sizes() const;
        class network &marker_sizes(const std::vector<float> &marker_sizes);

        const std::vector<double> &marker_colors() const;

        bool visible() const;
        class network &visible(bool visible);

      public /* getters and setters bypassing the line_spec */:
        float line_width() const;
        class network &line_width(float line_width);

        enum line_spec::marker_style marker_style() const;
        template <class T> network &marker_style(T marker_style) {
            line_spec_.marker_style(marker_style);
            return *this;
        }

        enum line_spec::marker_style marker() const;
        template <class T> network &marker(T marker) {
            line_spec_.marker(marker);
            return *this;
        }

        float marker_size() const;
        class network &marker_size(float size);
        class network &marker_size(const std::vector<float> &size_vector);
        class network &marker_size(const std::vector<double> &size_vector);

        bool marker_face() const;
        class network &marker_face(bool size);

        const std::array<float, 4> &color() const;
        template <class T> network &color(T c) {
            line_spec().color(c);
            return *this;
        }
        inline class network &color(std::initializer_list<float> c) {
            line_spec().color(c);
            return *this;
        }

        const std::array<float, 4> &marker_color() const;

        template <class T> network &marker_color(T c) {
            line_spec().marker_color(c);
            return *this;
        }

        template <class T> network &node_color(T c) {
            marker_color(c);
            return *this;
        }

        inline class network &marker_color(std::initializer_list<float> c) {
            line_spec().marker_color(c);
            return *this;
        }

        inline class network &marker_colors(const std::vector<double> &cs) {
            marker_colors_ = cs;
            touch();
            return *this;
        }

        const std::array<float, 4> &marker_face_color() const;

        template <class T> network &marker_face_color(T c) {
            line_spec().marker_face_color(c);
            return *this;
        }

        inline class network &
        marker_face_color(std::initializer_list<float> c) {
            line_spec().marker_face_color(c);
            return *this;
        }

        inline float marker_face_alpha() {
            return line_spec().marker_face_alpha();
        }

        inline class network &marker_face_alpha(float a) {
            line_spec().marker_face_alpha(a);
            return *this;
        }

        size_t n_vertices();

      protected:
        virtual std::vector<line_spec::style_to_plot> styles_to_plot();
        void maybe_update_line_spec();
        void maybe_update_graph_layout();
        void infer_n_vertices();
        void process_random_layout();
        void process_force_layout();
        void process_circle_layout();
        void process_kawai_layout();

      protected:
        /// Line style
        class line_spec line_spec_;

        /// Original graph
        std::vector<std::pair<size_t, size_t>> edges_{};
        /// The number of number is declared by the user or infered from the
        /// edges
        size_t n_vertices_{0};
        /// Weight for each edge
        vector_1d weights_{0};

        /// True for directed graphs
        bool directed_{false};

        /// Data in the xlim
        layout layout_algorithm_{layout::automatic};
        double layout_k_ = -1.;
        int layout_iterations_ = -1;
        double energy_threshold_ = 1e-2;
        std::vector<double> x_data_{};
        std::vector<double> y_data_{};
        std::vector<double> z_data_{};

        /// Style
        std::vector<float> marker_sizes_{};
        std::vector<double> marker_colors_{};
        bool show_labels_{true};
        std::vector<std::string> edge_labels_{};
        std::vector<std::string> node_labels_{};
        std::vector<double> line_widths_{};

        /// Use the y2 xlim
        bool use_y2_{false};

        /// True if visible
        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_NETWORK_H
