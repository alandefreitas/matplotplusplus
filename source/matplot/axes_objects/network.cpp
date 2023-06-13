//
// Created by Alan Freitas on 2020-07-07.
//

#include <cmath>
#include <iomanip>
#include <matplot/axes_objects/network.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <nodesoup.hpp>
#include <random>
#include <regex>
#include <sstream>

namespace matplot {
    network::network(class axes_type *parent) : axes_object(parent) {}

    network::network(class axes_type *parent,
                     const std::vector<std::pair<size_t, size_t>> &edges,
                     const vector_1d &weights, size_t n_vertices,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), edges_(edges),
          n_vertices_(n_vertices), weights_(weights) {
        line_spec_.marker_face(true);
    }

    std::vector<line_spec::style_to_plot> network::styles_to_plot() {
        return {line_spec::style_to_plot::plot_line_only,
                line_spec::style_to_plot::plot_marker_only};
    }

    std::string network::plot_string() {
        maybe_update_line_spec();
        maybe_update_graph_layout();

        std::string res;
        bool first = true;
        for (const auto &style : styles_to_plot()) {
            if (!first) {
                res += ",";
            }
            const bool we_are_plotting_line =
                style == line_spec::style_to_plot::plot_line_only;
            const bool line_width_is_variable =
                !line_widths_.empty() && z_data_.empty();
            const bool we_are_plotting_marker =
                style != line_spec::style_to_plot::plot_line_only;
            const bool marker_size_is_variable = !marker_sizes_.empty();
            const bool color_is_variable = !marker_colors_.empty();

            std::string str;
            if (we_are_plotting_line && directed_) {
                str += " '-' with vectors " +
                       line_spec_.plot_string(
                           line_spec::style_to_plot::plot_line_only, false);
            } else if (we_are_plotting_line && line_width_is_variable) {
                str += " '-' with filledcurves " +
                       line_spec_.plot_string(
                           line_spec::style_to_plot::plot_line_only, false);
            } else {
                str = " '-' " + line_spec_.plot_string(style, true);
            }

            if (marker_size_is_variable && we_are_plotting_marker) {
                str = std::regex_replace(str,
                                         std::regex(" pointsize \\d*\\.?\\d* "),
                                         " pointsize variable ");
            }
            if (color_is_variable && we_are_plotting_marker) {
                str = std::regex_replace(str,
                                         std::regex(" linecolor rgb +[^ ]+ "),
                                         " linecolor palette ");
            }
            if (use_y2_) {
                str += " xlim x1y2";
            }
            res += str;
            if (first) {
                first = false;
            }
        }
        if (show_labels_ || !edge_labels_.empty()) {
            res += ", '-' with labels left font \"Helvetica,11\" offset char 1";
        }
        return res;
    }

    std::string network::legend_string(std::string_view title) {
        if (line_spec_.has_line() && line_spec_.has_non_custom_marker()) {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_line_and_marker) +
                   " title \"" + escape(title) + "\"";
        } else if (line_spec_.has_line()) {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_line_only) +
                   " title \"" + escape(title) + "\"";
        } else {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_marker_only) +
                   " title \"" + escape(title) + "\"";
        }
    }

    std::string network::data_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        bool plot_z_data = z_data_.size() == x_data_.size();
        bool line_width_is_variable = !line_widths_.empty() && z_data_.empty();
        // plot edges
        if (!directed_) {
            // when not directed, each edge is a line
            for (size_t i = 0; i < edges_.size(); ++i) {
                ss << "    " << x_data_[edges_[i].first];
                if (!line_width_is_variable) {
                    ss << "  " << y_data_[edges_[i].first];
                } else {
                    ss << "  "
                       << y_data_[edges_[i].first] - line_widths_[i] / 2.;
                    ss << "  "
                       << y_data_[edges_[i].first] + line_widths_[i] / 2.;
                }
                if (plot_z_data) {
                    ss << "  " << z_data_[edges_[i].first];
                }
                ss << "\n";
                ss << "    " << x_data_[edges_[i].second];
                if (!line_width_is_variable) {
                    ss << "  " << y_data_[edges_[i].second];
                } else {
                    ss << "  "
                       << y_data_[edges_[i].second] - line_widths_[i] / 2.;
                    ss << "  "
                       << y_data_[edges_[i].second] + line_widths_[i] / 2.;
                }
                if (plot_z_data) {
                    ss << "  " << z_data_[edges_[i].second];
                }
                ss << "\n\n";
            }
            ss << "e\n";
        } else {
            // when directed, each edge is a vector
            // for each edge, plot two vectors
            // - one from start to middle
            // - one from middle to end
            for (size_t i = 0; i < edges_.size(); ++i) {
                double x1 = x_data_[edges_[i].first];
                double y1 = y_data_[edges_[i].first];
                double x2 = x_data_[edges_[i].second];
                double y2 = y_data_[edges_[i].second];
                double xmean = 0.5 * (x1 + x2);
                double ymean = 0.5 * (y1 + y2);
                double u1 = xmean - x1;
                double v1 = ymean - y1;
                double u2 = x2 - xmean;
                double v2 = y2 - ymean;
                if (!plot_z_data) {
                    ss << "    " << x1 << "  " << y1 << "  " << u1 << "  " << v1
                       << "\n";
                    ss << "    " << xmean << "  " << ymean << "  " << u2 << "  "
                       << v2 << "\n";
                } else {
                    double z1 = z_data_[edges_[i].first];
                    double z2 = z_data_[edges_[i].second];
                    double zmean = 0.5 * (z1 + z2);
                    double w1 = zmean - z1;
                    double w2 = z2 - zmean;
                    ss << "    " << x1 << "  " << y1 << "  " << z1 << "  " << u1
                       << "  " << v1 << "  " << w1 << "\n";
                    ss << "    " << xmean << "  " << ymean << "  " << zmean
                       << "  " << u2 << "  " << v2 << "  " << w2 << "\n";
                }
            }
            ss << "e\n";
        }

        // plot vertices
        const bool marker_size_is_variable = !marker_sizes_.empty();
        const bool marker_color_is_variable = !marker_colors_.empty();
        for (size_t i = 0; i < x_data_.size(); ++i) {
            ss << "    " << x_data_[i];
            ss << "  " << y_data_[i];
            if (plot_z_data) {
                ss << "  " << z_data_[i];
            }
            if (marker_size_is_variable) {
                ss << "  " << marker_sizes_[i];
            }
            if (marker_color_is_variable) {
                ss << "  " << marker_colors_[i];
            }
            ss << "\n";
        }
        ss << "e\n";

        // plot labels
        if (show_labels_ || !edge_labels_.empty()) {
            if (show_labels_) {
                for (size_t i = 0; i < x_data_.size(); ++i) {
                    ss << "    " << x_data_[i];
                    ss << "  " << y_data_[i];
                    if (plot_z_data) {
                        ss << "  " << z_data_[i];
                    }
                    if (node_labels().size() > i) {
                        ss << "  " << std::quoted(node_labels_[i]);
                    } else {
                        ss << "  " << i;
                    }
                    ss << "\n";
                }
            }
            // plot edge labels
            if (!edge_labels_.empty()) {
                for (size_t i = 0; i < edge_labels_.size(); ++i) {
                    ss << "    "
                       << 0.5 * (x_data_[edges_[i].first] +
                                 x_data_[edges_[i].second]);
                    ss << "  "
                       << 0.5 * (y_data_[edges_[i].first] +
                                 y_data_[edges_[i].second]);
                    if (plot_z_data) {
                        ss << "  "
                           << 0.5 * (z_data_[edges_[i].first] +
                                     z_data_[edges_[i].second]);
                    }
                    ss << "  " << std::quoted(edge_labels_[i]);
                    ss << "\n";
                }
            }
            ss << "e\n";
        }

        return ss.str();
    }

    void network::maybe_update_line_spec() {
        if (line_spec_.has_line() && !line_spec_.user_color()) {
            // if user didn't set the color, get color from xlim
            auto c = parent_->get_color_and_bump();
            line_spec_.color(c);
        } else if (line_spec_.has_non_custom_marker() &&
                   !line_spec_.user_color() &&
                   !line_spec_.marker_user_color()) {
            auto c = parent_->get_color_and_bump();
            line_spec_.marker_color(c);
        }
    }

    void network::maybe_update_graph_layout() {
        const bool layout_ready = !x_data_.empty();
        if (!layout_ready) {
            switch (layout_algorithm_) {
            case layout::random:
                process_random_layout();
                break;
            case layout::force:
                process_force_layout();
                break;
            case layout::automatic: {
                size_t n_edges = edges_.size();
                size_t n_vert = n_vertices();
                // We used a simple benchmark to determine these values
                // As long as kawai is not slower than force, we use kawai
                if (n_vert < 100 && n_edges < 1000) {
                    process_kawai_layout();
                } else {
                    process_force_layout();
                }
                break;
            }
            case layout::kawai:
                process_kawai_layout();
                break;
            case layout::circle:
                process_circle_layout();
                break;
            default:
                break;
            }
        }
    }

    /// If the user has not provided a list of nodes
    /// we assume this is a list from 0 to max(edge)
    void network::infer_n_vertices() {
        // look for highest edge index
        size_t n_vertices = edges_[0].first;
        for (const auto &[edge_index_a, edge_index_b] : edges_) {
            if (edge_index_a > n_vertices) {
                n_vertices = edge_index_a;
            }
            if (edge_index_b > n_vertices) {
                n_vertices = edge_index_b;
            }
        }
        n_vertices_ = n_vertices + 1;
    }

    size_t network::n_vertices() {
        if (n_vertices_ != 0) {
            return n_vertices_;
        } else {
            infer_n_vertices();
            return n_vertices_;
        }
    }

    void network::process_random_layout() {
        x_data_.clear();
        y_data_.clear();
        z_data_.clear();
        size_t n = n_vertices();
        unsigned seed = static_cast<unsigned>(
            std::chrono::system_clock::now().time_since_epoch().count());
        std::mt19937 g(seed);
        int width =
            static_cast<int>(parent_->width() * parent_->parent()->width());
        int height =
            static_cast<int>(parent_->height() * parent_->parent()->height());
        std::uniform_real_distribution<double> dx(0, width);
        std::uniform_real_distribution<double> dy(0, height);
        for (size_t i = 0; i < n; ++i) {
            x_data_.emplace_back(dx(g));
            y_data_.emplace_back(dy(g));
        }
    }

    void network::process_force_layout() {
        x_data_.clear();
        y_data_.clear();
        z_data_.clear();

        size_t n = n_vertices();
        nodesoup::adj_list_t g(n);
        for (size_t i = 0; i < edges_.size(); ++i) {
            g[edges_[i].first].emplace_back(edges_[i].second);
        }
        std::vector<double> radiuses = nodesoup::size_radiuses(g);
        double k = layout_k_;
        if (k == -1.0) {
            k = 15.0;
        }
        int width =
            static_cast<int>(parent_->width() * parent_->parent()->width());
        int height =
            static_cast<int>(parent_->height() * parent_->parent()->height());
        int iters_count = layout_iterations_;
        if (iters_count == -1) {
            iters_count = 300;
        }
        std::vector<nodesoup::Point2D> positions =
            nodesoup::fruchterman_reingold(g, width, height, iters_count, k);
        for (size_t i = 0; i < positions.size(); ++i) {
            x_data_.emplace_back(positions[i].x);
            y_data_.emplace_back(positions[i].y);
        }
    }

    void network::process_circle_layout() {
        int previous_layout_iterations = layout_iterations_;
        double previous_layout_k = layout_k_;
        layout_iterations_ = 0;
        layout_k_ = 15.0;
        process_force_layout();
        layout_iterations_ = previous_layout_iterations;
        layout_k_ = previous_layout_k;
    }

    void network::process_kawai_layout() {
        x_data_.clear();
        y_data_.clear();
        z_data_.clear();

        size_t n = n_vertices();
        nodesoup::adj_list_t g(n);
        for (size_t i = 0; i < edges_.size(); ++i) {
            g[edges_[i].first].emplace_back(edges_[i].second);
        }
        std::vector<double> radiuses = nodesoup::size_radiuses(g);
        double k = layout_k_;
        if (k == -1.0) {
            k = 300.0;
        }
        int width =
            static_cast<int>(parent_->width() * parent_->parent()->width());
        int height =
            static_cast<int>(parent_->height() * parent_->parent()->height());
        nodesoup::iter_callback_t cback = nullptr;
        std::vector<nodesoup::Point2D> positions =
            nodesoup::kamada_kawai(g, width, height, k, energy_threshold_);
        for (size_t i = 0; i < positions.size(); ++i) {
            x_data_.emplace_back(positions[i].x);
            y_data_.emplace_back(positions[i].y);
        }
    }

    double network::xmax() {
        maybe_update_graph_layout();
        if (x_data_.empty()) {
            if (!y_data_.empty()) {
                return static_cast<double>(y_data_.size() - 1);
            } else {
                return axes_object::xmax();
            }
        }
        auto [xmin_it, xmax_it] =
            std::minmax_element(x_data_.begin(), x_data_.end());
        return *xmax_it + 0.1 * (*xmax_it - *xmin_it);
    }

    double network::xmin() {
        maybe_update_graph_layout();
        if (x_data_.empty()) {
            if (!y_data_.empty()) {
                return 0;
            } else {
                return axes_object::xmin();
            }
        }
        auto [xmin_it, xmax_it] =
            std::minmax_element(x_data_.begin(), x_data_.end());
        return *xmin_it - 0.1 * (*xmax_it - *xmin_it);
    }

    double network::ymax() {
        maybe_update_graph_layout();
        if (y_data_.empty()) {
            return axes_object::ymax();
        }
        auto [ymin_it, ymax_it] =
            std::minmax_element(y_data_.begin(), y_data_.end());
        return *ymax_it + 0.1 * (*ymax_it - *ymin_it);
    }

    double network::ymin() {
        maybe_update_graph_layout();
        if (y_data_.empty()) {
            return axes_object::ymin();
        }
        auto [ymin_it, ymax_it] =
            std::minmax_element(y_data_.begin(), y_data_.end());
        return *ymin_it - 0.1 * (*ymax_it - *ymin_it);
    }

    enum axes_object::axes_category network::axes_category() {
        if (z_data_.empty()) {
            return axes_object::axes_category::two_dimensional;
        } else {
            return axes_object::axes_category::three_dimensional;
        }
    }

    class network &network::line_style(std::string_view str) {
        line_spec_.parse_string(str);
        touch();
        return *this;
    }

    const line_spec &network::line_spec() const { return line_spec_; }

    line_spec &network::line_spec() { return line_spec_; }

    class network &network::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const std::vector<double> &network::y_data() const { return y_data_; }

    class network &network::y_data(const std::vector<double> &y_data) {
        y_data_ = y_data;
        if (!y_data.empty() && parent_->children().size() == 1) {
            parent_->y_axis().limits({ymin(), ymax()});
        }
        touch();
        return *this;
    }

    const std::vector<double> &network::x_data() const { return x_data_; }

    class network &network::x_data(const std::vector<double> &x_data) {
        x_data_ = x_data;
        if (!x_data.empty() && parent_->children().size() == 1) {
            parent_->x_axis().limits({xmin(), xmax()});
        }
        touch();
        return *this;
    }

    const std::vector<double> &network::z_data() const { return z_data_; }

    class network &network::z_data(const std::vector<double> &z_data) {
        z_data_ = z_data;
        if (!z_data.empty() && parent_->children().size() == 1) {
            auto [zmin, zmax] = minmax(z_data);
            parent_->z_axis().limits({zmin, zmax});
        }
        touch();
        return *this;
    }

    bool network::visible() const { return visible_; }

    class network &network::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float network::line_width() const { return line_spec().line_width(); }

    class network &network::line_width(float line_width) {
        line_spec().line_width(line_width);
        return *this;
    }

    enum line_spec::marker_style network::marker_style() const {
        return line_spec().marker();
    }

    enum line_spec::marker_style network::marker() const {
        return line_spec().marker();
    }

    float network::marker_size() const { return line_spec().marker_size(); }

    class network &network::marker_size(float size) {
        line_spec().marker_size(size);
        return *this;
    }

    class network &network::marker_size(const std::vector<float> &size_vector) {
        marker_sizes_ = size_vector;
        touch();
        return *this;
    }

    class network &
    network::marker_size(const std::vector<double> &size_vector) {
        std::vector<float> size_vector_float(size_vector.size());
        std::transform(size_vector.begin(), size_vector.end(),
                       size_vector_float.begin(),
                       [](const double &x) { return static_cast<float>(x); });
        marker_size(size_vector_float);
        return *this;
    }

    bool network::marker_face() const { return line_spec().marker_face(); }

    class network &network::marker_face(bool marker_face) {
        line_spec().marker_face(marker_face);
        return *this;
    }

    const std::array<float, 4> &network::color() const {
        return line_spec().color();
    }

    const std::array<float, 4> &network::marker_color() const {
        return line_spec().marker_color();
    }

    const std::array<float, 4> &network::marker_face_color() const {
        return line_spec().marker_face_color();
    }

    bool network::use_y2() const { return use_y2_; }

    class network &network::use_y2(bool use_y_2) {
        use_y2_ = use_y_2;
        touch();
        return *this;
    }

    bool network::show_labels() const { return show_labels_; }

    class network &network::show_labels(bool v) {
        show_labels_ = v;
        touch();
        return *this;
    }

    bool network::directed() const { return directed_; }

    class network &network::directed(bool v) {
        if (v != directed_) {
            directed_ = v;
            touch();
        }
        return *this;
    }

    bool network::use_y_2() const { return use_y2_; }

    class network &network::use_y_2(bool use_y_2) {
        use_y2_ = use_y_2;
        touch();
        return *this;
    }

    const std::vector<std::string> &network::edge_labels() const {
        return edge_labels_;
    }

    class network &
    network::edge_labels(const std::vector<std::string> &edge_labels) {
        edge_labels_ = edge_labels;
        touch();
        return *this;
    }

    const std::vector<std::string> &network::node_labels() const {
        return node_labels_;
    }

    class network &
    network::node_labels(const std::vector<std::string> &node_labels) {
        node_labels_ = node_labels;
        touch();
        return *this;
    }

    const std::vector<double> &network::line_widths() const {
        return line_widths_;
    }

    class network &
    network::line_widths(const std::vector<double> &line_widths) {
        line_widths_ = line_widths;
        touch();
        return *this;
    }

    const std::vector<std::pair<size_t, size_t>> &network::edges() const {
        return edges_;
    }

    class network &
    network::edges(const std::vector<std::pair<size_t, size_t>> &edges) {
        edges_ = edges;
        touch();
        return *this;
    }

    class network &network::n_vertices(size_t n_vertices) {
        n_vertices_ = n_vertices;
        touch();
        return *this;
    }

    const vector_1d &network::weights() const { return weights_; }

    class network &network::weights(const vector_1d &weights) {
        weights_ = weights;
        touch();
        return *this;
    }

    matplot::network::layout network::layout_algorithm() const {
        return layout_algorithm_;
    }

    class network &
    network::layout_algorithm(matplot::network::layout layout_algorithm) {
        if (layout_algorithm_ != layout_algorithm) {
            layout_algorithm_ = layout_algorithm;
            x_data_.clear();
            y_data_.clear();
            z_data_.clear();
            touch();
        }
        return *this;
    }

    double network::layout_k() const { return layout_k_; }

    class network &network::layout_k(double layout_k) {
        if (layout_k_ != layout_k) {
            layout_k_ = layout_k;
            x_data_.clear();
            y_data_.clear();
            z_data_.clear();
            touch();
        }
        return *this;
    }

    int network::layout_iterations() const { return layout_iterations_; }

    class network &network::layout_iterations(int layout_iterations) {
        if (layout_iterations_ != layout_iterations) {
            layout_iterations_ = layout_iterations;
            x_data_.clear();
            y_data_.clear();
            z_data_.clear();
            touch();
        }
        return *this;
    }

    double network::energy_threshold() const { return energy_threshold_; }

    class network &network::energy_threshold(double energy_threshold) {
        if (energy_threshold_ != energy_threshold) {
            energy_threshold_ = energy_threshold;
            x_data_.clear();
            y_data_.clear();
            z_data_.clear();
            touch();
        }
        return *this;
    }

    const std::vector<float> &network::marker_sizes() const {
        return marker_sizes_;
    }

    class network &
    network::marker_sizes(const std::vector<float> &marker_sizes) {
        marker_sizes_ = marker_sizes;
        touch();
        return *this;
    }

    const std::vector<double> &network::marker_colors() const {
        return marker_colors_;
    }

} // namespace matplot