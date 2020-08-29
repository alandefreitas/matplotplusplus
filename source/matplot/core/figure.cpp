//
// Created by Alan Freitas on 2020-07-03.
//

#include <algorithm>
#include <map>
#include <matplot/backend/backend_registry.h>
#include <matplot/core/axes.h>
#include <matplot/core/figure.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {
    figure::figure() : figure(1, true) {}

    figure::figure(bool quiet_mode) : figure(1, quiet_mode) {}

    figure::figure(size_t index) : figure(index, true) {}

    figure::figure(size_t index, bool quiet_mode)
        : number_(index), quiet_mode_(quiet_mode) {
        backend_ = create_default_backend();
    }

#ifdef MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES
    figure::~figure() { save("example.svg", "svg"); }
#else
    figure::~figure() = default;
#endif

    void figure::include_comment(const std::string &text) {
        backend_->include_comment(text);
    }

    void figure::run_command(const std::string &command) {
        backend_->run_command(command);
    }

    void figure::draw() {
        // we cannot call draw if we are already drawing
        // this could create infinite loops
        if (backend_ == nullptr || is_plotting_) {
            return;
        }

        // Avoid infinite loops if the draw function calls
        // "touch" for some reason
        is_plotting_ = true;

        // There are two different strategies
        // Gnuplot backends do not rely on vertices
        // because that would be very ineficient
        // So we unfortunately need a different strategy
        // for gnuplot
        if (!backend_->consumes_gnuplot_commands()) {
            send_draw_commands();
        } else {
            send_gnuplot_draw_commands();
        }
        backend_->render_data();

        is_plotting_ = false;
    }

    void figure::send_draw_commands() {
        // This mode is not implemented yet

        // Send some draw commands to the backend here
        // ...

        // Iterate children axes
        // Ask them to send their draw commands to the backend
    }

    void figure::send_gnuplot_draw_commands() {
        include_comment("Setting figure properties");
        run_figure_properties_command();
        if (children_.empty()) {
            include_comment("An empty plot");
            plot_empty_plot();
        } else {
            include_comment("Start multiplot");
            run_multiplot_command();
            bool first = true;
            include_comment("Window background");
            if (backend_->output().empty()) {
                run_window_color_command();
            }
            size_t index = 1;
            for (auto &axes : children_) {
                include_comment("Plotting axes - " + num2str(index++));
                if (!first) {
                    include_comment("    Reset object from multiplot - " +
                                    num2str(index - 2));
                    run_command("    reset");
                }
                axes->run_commands();
                if (first) {
                    include_comment("Unset window background so it does repeat "
                                    "on other subplots");
                    run_unset_window_color_command();
                    first = false;
                }
            }
            if (children_.size() > 1) {
                include_comment(
                    "Multiplots don't work well with the mouse in Gnuplot");
                run_command("unset mouse");
            }
            include_comment("Finalizing the multiplot");
            run_command("unset multiplot");
        }
    }

    void figure::show() {
        draw();
        backend_->wait();
    }

    void figure::touch() {
        if (!quiet_mode_) {
            draw();
        }
    }

    void figure::flush_commands() {
        // render data
        backend_->render_data();
    }

    bool figure::save(const std::string &filename, const std::string &format) {
        try {
            auto poutput = backend_->output();
            auto pformat = backend_->output_format();
            if (backend_->output(filename, format)) {
                draw();
            }
            backend_->output(poutput, pformat);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool figure::save(const std::string &filename) {
        try {
            auto pout = backend_->output();
            auto pformat = backend_->output_format();
            if (backend_->output(filename)) {
                draw();
            }
            backend_->output(pout, pformat);
            return true;
        } catch (...) {
            return false;
        }
    }

    figure::operator bool() const { return backend_ != nullptr; }

    void figure::plot_empty_plot() {
        // Plot something out of yrange
        run_command("set yrange [0:1]");
        run_command("set xrange [0:1]");
        run_command("set key off");
        run_command("plot 2 with lines");
    }

    void figure::name(const std::string &name) {
        name_ = name;
        touch();
    }

    std::string figure::name() const { return name_; }

    void figure::color(const std::array<float, 3> &c) {
        color_ = {0, c[0], c[1], c[2]};
        touch();
    }

    void figure::color(const color_array &c) {
        color_ = c;
        custom_color_ = true;
        touch();
    }

    void figure::color(const std::string &c) {
        color(to_array(string_to_color(c)));
    }

    void figure::color(const enum color &c) { color(to_array(c)); }

    color_array figure::color() const { return color_; }

    void figure::position(const std::array<unsigned, 4> &c) {
        backend_->position_x(c[0]);
        backend_->position_y(c[1]);
        backend_->width(c[2]);
        backend_->height(c[3]);
        touch();
    }

    unsigned figure::x_position() const {
        // get x_position from the backend because the user
        // might have changed it manually by dragging the
        // window
        return backend_->position_x();
    }

    void figure::x_position(unsigned x) {
        backend_->position_x(x);
        touch();
    }

    unsigned figure::y_position() const {
        // get y_position from the backend because the user
        // might have changed it manually by dragging the
        // window
        return backend_->position_y();
    }

    void figure::y_position(unsigned y) {
        backend_->position_y(y);
        touch();
    }

    unsigned figure::width() const {
        // get width from the backend because the user
        // might have changed it manually by dragging the
        // window
        return backend_->width();
    }

    void figure::width(unsigned w) {
        backend_->width(w);
        touch();
    }

    unsigned figure::height() const {
        // get height from the backend because the user
        // might have changed it manually by dragging the
        // window
        return backend_->height();
    }

    void figure::height(unsigned h) {
        backend_->height(h);
        touch();
    }

    void figure::position(unsigned x, unsigned y, unsigned w, unsigned h) {
        position({x, y, w, h});
    }

    void figure::position(unsigned x, unsigned y) {
        backend_->position_x(x);
        backend_->position_y(y);
        touch();
    }

    void figure::size(unsigned w, unsigned h) {
        backend_->width(w);
        backend_->height(h);
        touch();
    }

    std::array<unsigned, 4> figure::position() const {
        return {x_position(), y_position(), width(), height()};
    }

    size_t figure::number() const { return number_; }

    void figure::number_title(bool number_title) {
        number_title_ = number_title;
        touch();
    }

    bool figure::number_title() const { return number_title_; }

    axes_handle figure::add_axes(bool replace_if_overlap) {
        axes_handle h = std::make_shared<class axes>(this);
        return this->add_axes(h, replace_if_overlap, replace_if_overlap);
    }

    axes_handle figure::add_axes(std::array<float, 4> position) {
        axes_handle h = std::make_shared<class axes>(this, position);
        h->box(true);
        return this->add_axes(h, false, true);
    }

    axes_handle figure::add_axes(axes_handle h, bool replace_if_overlap,
                                 bool replace_if_same_position) {
        auto it = std::find_if(children_.begin(), children_.end(),
                               [&h](const auto &child) {
                                   return child->position() == h->position();
                               });
        if (it != children_.end()) {
            if (replace_if_same_position) {
                children_.erase(it);
            } else {
                return *it;
            }
        }
        if (replace_if_overlap) {
            // look for overlap
            auto overlap = [&h](const axes_handle &x) {
                const float x_min = x->x_origin();
                const float x_max = x->x_origin() + x->width();
                const float hx_min = h->x_origin();
                const float hx_max = h->x_origin() + h->width();
                const float y_min = x->y_origin();
                const float y_max = x->y_origin() + x->height();
                const float hy_min = h->y_origin();
                const float hy_max = h->y_origin() + h->height();
                // for dimensions 0 and 1, check min and max intersection
                return !(x_min > hx_max || hx_min > x_max || y_min > hy_max ||
                         hy_min > y_max);
            };
            auto overlap_it =
                std::remove_if(children_.begin(), children_.end(), overlap);
            children_.erase(overlap_it, children_.end());
        }
        children_.emplace_back(h);
        current_axes_ = children_.back();
        current_axes_->parent(this);
        touch();
        return h;
    }

    std::array<float, 4> figure::calculate_subplot_position(size_t rows,
                                                            size_t cols,
                                                            size_t plot_id) {
        if (plot_id >= rows * cols) {
            plot_id = plot_id % (rows * cols);
        }
        size_t row = rows - plot_id / cols - 1;
        size_t col = plot_id % cols;
        constexpr size_t row_width = 1; // max(row) - min(row) + 1
        constexpr size_t col_width = 1; // max(col) - min(col) + 1
        const size_t min_row = row;
        const size_t min_col = col;
        const size_t max_row = row;
        const size_t max_col = col;
        float width =
            axes::default_axes_position[2] /
            (static_cast<float>(cols) - axes::default_subplot_inset[0] -
             axes::default_subplot_inset[2]); // 1,0197
        float height =
            axes::default_axes_position[3] /
            (static_cast<float>(rows) - axes::default_subplot_inset[1] -
             axes::default_subplot_inset[3]); // 0,4738
        std::array<float, 4> subplot_inset{
            axes::default_subplot_inset[0] * width,
            axes::default_subplot_inset[1] * height,
            axes::default_subplot_inset[2] * width,
            axes::default_subplot_inset[3] * height};
        std::array<float, 4> outer_pos{axes::default_axes_position[0] +
                                           min_col * width - subplot_inset[0],
                                       axes::default_axes_position[1] +
                                           min_row * height - subplot_inset[1],
                                       width * col_width, height * row_width};
        if (min_col == 0) {
            subplot_inset[0] = axes::default_axes_position[0];
            outer_pos[2] = outer_pos[0] + outer_pos[2];
            outer_pos[0] = 0;
        }
        if (min_row == 0) {
            subplot_inset[1] = axes::default_axes_position[1];
            outer_pos[3] = outer_pos[1] + outer_pos[3];
            outer_pos[1] = 0;
        }
        if (max_col == cols - 1) {
            subplot_inset[2] =
                std::max(0.f, 1.f - axes::default_axes_position[0] -
                                  axes::default_axes_position[2]);
            outer_pos[2] = 1 - outer_pos[0];
        }
        if (max_row == rows - 1) {
            subplot_inset[3] =
                std::max(0., 1. - axes::default_axes_position[1] -
                                 axes::default_axes_position[3]);
            outer_pos[3] = 1 - outer_pos[1];
        }
        std::array<float, 4> position = {
            outer_pos[0] + subplot_inset[0], outer_pos[1] + subplot_inset[1],
            outer_pos[2] - subplot_inset[0] - subplot_inset[2],
            outer_pos[3] - subplot_inset[1] - subplot_inset[3]};
        return position;
    }

    axes_handle figure::add_subplot(size_t rows, size_t cols, size_t plot_id,
                                    bool replace_if_same_position) {
        if (rows == 1 && cols == 1 && plot_id == 0) {
            return add_axes();
        }
        return this->add_subplot(
            calculate_subplot_position(rows, cols, plot_id),
            replace_if_same_position);
    }

    axes_handle figure::add_subplot(size_t rows, size_t cols, size_t plot_id,
                                    axes_handle ax) {
        ax->position(calculate_subplot_position(rows, cols, plot_id));
        auto it = std::find(children_.begin(), children_.end(), ax);
        if (it == children_.end()) {
            return add_axes(ax, true, false);
        }
        return ax;
    }

    axes_handle figure::add_subplot(size_t rows, size_t cols,
                                    std::initializer_list<size_t> positions,
                                    bool replace_if_same_position) {
        if (rows == 1 && cols == 1 && positions.size() == 1 &&
            *positions.begin() == 1) {
            return this->add_axes(true);
        }
        std::vector plot_ids = std::vector(positions);
        size_t max_plot_id =
            *std::max_element(plot_ids.begin(), plot_ids.end());
        if (max_plot_id >= rows * cols) {
            throw std::invalid_argument("Plot index is too large");
        }
        std::vector<size_t> row(plot_ids.size());
        std::vector<size_t> col(plot_ids.size());
        for (size_t i = 0; i < plot_ids.size(); ++i) {
            row[i] = rows - plot_ids[i] / cols - 1;
            col[i] = plot_ids[i] % cols;
        }
        const size_t min_row = *std::min_element(row.begin(), row.end());
        const size_t min_col = *std::min_element(col.begin(), col.end());
        const size_t max_row = *std::max_element(row.begin(), row.end());
        const size_t max_col = *std::max_element(col.begin(), col.end());
        const size_t row_width = max_row - min_row + 1;
        const size_t col_width = max_col - min_col + 1;
        float width =
            axes::default_axes_position[2] /
            (static_cast<float>(cols) - axes::default_subplot_inset[0] -
             axes::default_subplot_inset[2]); // 1,0197
        float height =
            axes::default_axes_position[3] /
            (static_cast<float>(rows) - axes::default_subplot_inset[1] -
             axes::default_subplot_inset[3]); // 0,4738
        std::array<float, 4> subplot_inset{
            axes::default_subplot_inset[0] * width,
            axes::default_subplot_inset[1] * height,
            axes::default_subplot_inset[2] * width,
            axes::default_subplot_inset[3] * height};
        std::array<float, 4> outer_pos{axes::default_axes_position[0] +
                                           min_col * width - subplot_inset[0],
                                       axes::default_axes_position[1] +
                                           min_row * height - subplot_inset[1],
                                       width * col_width, height * row_width};
        if (min_col == 0) {
            subplot_inset[0] = axes::default_axes_position[0];
            outer_pos[2] = outer_pos[0] + outer_pos[2];
            outer_pos[0] = 0;
        }
        if (min_row == 0) {
            subplot_inset[1] = axes::default_axes_position[1];
            outer_pos[3] = outer_pos[1] + outer_pos[3];
            outer_pos[1] = 0;
        }
        if (max_col == cols - 1) {
            subplot_inset[2] =
                std::max(0.f, 1.f - axes::default_axes_position[0] -
                                  axes::default_axes_position[2]);
            outer_pos[2] = 1 - outer_pos[0];
        }
        if (max_row == rows - 1) {
            subplot_inset[3] =
                std::max(0., 1. - axes::default_axes_position[1] -
                                 axes::default_axes_position[3]);
            outer_pos[3] = 1 - outer_pos[1];
        }
        std::array<float, 4> position = {
            outer_pos[0] + subplot_inset[0], outer_pos[1] + subplot_inset[1],
            outer_pos[2] - subplot_inset[0] - subplot_inset[2],
            outer_pos[3] - subplot_inset[1] - subplot_inset[3]};
        return this->add_subplot(position, replace_if_same_position);
    }

    axes_handle figure::add_subplot(std::array<float, 4> position,
                                    bool replace_if_same_position) {
        axes_handle h = std::make_shared<class axes>(this, position);
        h->box(true);
        return this->add_axes(h, true, replace_if_same_position);
    }

    void figure::tiledlayout(size_t rows, size_t cols) {
        tiledlayout_rows_ = rows;
        tiledlayout_cols_ = cols;
        current_tile_index_ = 0;
        tiledlayout_flow_ = rows == 1 && cols == 1;
        children_.clear();
    }

    axes_handle figure::nexttile() {
        const bool next_tile_fits =
            current_tile_index_ < tiledlayout_rows_ * tiledlayout_cols_;
        if (next_tile_fits || !tiledlayout_flow_) {
            if (!next_tile_fits) {
                current_tile_index_ -= tiledlayout_rows_ * tiledlayout_cols_;
            }
            axes_handle h = this->add_subplot(
                tiledlayout_rows_, tiledlayout_cols_, current_tile_index_);
            current_tile_index_++;
            return h;
        } else {
            if (tiledlayout_rows_ <= tiledlayout_cols_) {
                ++tiledlayout_rows_;
            } else {
                ++tiledlayout_cols_;
            }
            // reposition existing axes, assuming they are all independent tiles
            auto f = gcf();
            for (size_t j = 0; j < f->children().size(); ++j) {
                auto p = calculate_subplot_position(tiledlayout_rows_,
                                                    tiledlayout_cols_, j);
                f->children()[j]->position(p);
            }
            return nexttile();
        }
    }

    axes_handle figure::nexttile(size_t index) {
        current_tile_index_ = index;
        return nexttile();
    }

    axes_handle figure::current_axes() {
        axes_handle h = current_axes_;
        if (!h) {
            h = this->add_axes();
        }
        return h;
    }

    axes_handle figure::current_axes() const { return current_axes_; }

    void figure::current_axes(const axes_handle &current_axes) {
        current_axes_ = current_axes;
        // bring axes to front
        auto it = std::find(children_.begin(), children_.end(), current_axes);
        if (it != children_.end()) {
            if (*it != children_.back()) {
                std::swap(*it, children_.back());
                touch();
            }
        }
    }

    void figure::run_figure_properties_command() {
        run_command("reset");
        run_terminal_init_command();
        if (children_.empty()) {
            run_command("unset border");
            run_command("unset xtics");
            run_command("unset ytics");
        }

        if (children_.empty()) {
            run_window_color_command();
        }
    }

    void figure::run_terminal_init_command() {
        std::stringstream ss;
        auto &terminal = backend_->output_format();
        ss << "set terminal " + terminal;

        if (terminal.empty() &&
            backend::gnuplot::terminal_has_title_option(terminal)) {
            std::string title;
            if (number_title_) {
                title += "Figure " + num2str(number_);
                if (!name_.empty()) {
                    title += ": " + name_;
                }
            } else {
                title = name_;
            }
            ss << " title \"" << escape(title) << "\"";
            run_command(ss.str());
        }

        if (backend::gnuplot::terminal_has_size_option(terminal)) {
            ss << " size " << num2str(width()) << "," << num2str(height());
        }

        if (backend::gnuplot::terminal_has_enhanced_option(terminal)) {
            ss << " enhanced";
        }

        if (backend::gnuplot::terminal_has_color_option(terminal)) {
            ss << " color";
        }

        if (backend::gnuplot::terminal_has_font_option(terminal)) {
            ss << " font \"" + escape(font_) + "," +
                      num2str(unsigned(font_size_)) + "\"";
        }

        run_command(ss.str());
        const auto &output = backend_->output();
        if (!output.empty()) {
            run_command("set output \"" + escape(output) + "\"");
        }
    }

    void figure::run_window_color_command() {
        // In gnuplot 5.5 we have the wall function to set the axes color
        // with a rectangle workaround, which does not work well for 3d.
        static const auto v = backend::gnuplot::gnuplot_version();
        const bool has_wall_option =
            v.first > 5 || (v.first == 5 && v.second >= 5);
        // So we only plot the default background if it's not 3d or version is
        // higher than 5.5. Otherwise, gnuplot won't be able to set the axes
        // colors.
        const bool three_d_is_not_a_problem =
            children_.empty() || !children_[0]->is_3d() ||
            children_[0]->is_3d_map() || has_wall_option;
        // If the background is custom, we plot it, but
        // gnuplot will not be able to plot the axes background if it's 3d and
        // its version is less than 5.5. It's the user's choice then.
        const bool force_background_anyway = custom_color_;

        if (title_.empty()) {
            if (three_d_is_not_a_problem || force_background_anyway) {
                if (to_array(color::white) != color_) {
                    run_command("set object 1 rectangle from screen 0,0 to "
                                "screen 1,1 behind fillcolor rgb \"" +
                                to_string(color_) +
                                "\" fillstyle solid 1.0 noborder");
                }
            }
        }
    }

    void figure::run_unset_window_color_command() {
        if (custom_color_ || children_.empty() || !children_[0]->is_3d() ||
            children_[0]->is_3d_map()) {
            if (to_array(color::white) != color_) {
                run_command("unset object 1");
            }
        }
    }

    void figure::run_multiplot_command() {
        std::string cmd = "set multiplot";
        if (!title_.empty()) {
            cmd += " title \"{/:Bold " + escape(title_) + "}\"";
            cmd += " font '" + font_ + "," +
                   num2str(unsigned(font_size_ * title_font_size_multiplier_)) +
                   "'";
            cmd += " textcolor '" + to_string(title_color_) + "'";
        }
        run_command(cmd);
    }

    const std::string &figure::font() const { return font_; }

    void figure::font(const std::string &font) {
        font_ = font;
        touch();
    }

    float figure::font_size() const { return font_size_; }

    void figure::font_size(float font_size) {
        font_size_ = font_size;
        touch();
    }

    const std::string &figure::title() const { return title_; }

    void figure::title(const std::string &title) {
        title_ = title;
        touch();
    }

    const color_array &figure::title_color() const { return title_color_; }

    void figure::title_color(const color_array &title_color) {
        title_color_ = title_color;
        touch();
    }

    float figure::title_font_size_multiplier() const {
        return title_font_size_multiplier_;
    }

    void figure::title_font_size_multiplier(float title_font_size_multiplier) {
        title_font_size_multiplier_ = title_font_size_multiplier;
        touch();
    }

    bool figure::custom_color() const { return custom_color_; }

    void figure::custom_color(bool custom_color) {
        custom_color_ = custom_color;
        touch();
    }

    bool figure::quiet_mode() const { return quiet_mode_; }

    void figure::quiet_mode(bool quiet_mode) { quiet_mode_ = quiet_mode; }

    bool figure::reactive_mode() const { return !quiet_mode_; }

    void figure::reactive_mode(bool reactive_mode) {
        quiet_mode_ = !reactive_mode;
    }

    void figure::ion() { quiet_mode_ = false; }

    void figure::ioff() { quiet_mode_ = true; }

    namespace detail {
        /// Map with all figure handles there are
        std::map<size_t, figure_handle> &global_figure_handles() {
            static std::map<size_t, figure_handle> registry;
            return registry;
        }

        /// Global handle to the figure considered the current figure
        figure_handle &current_figure_handle() {
            static figure_handle cfh;
            return cfh;
        }

        void set_current_figure_handle(figure_handle &h) {
            current_figure_handle() = h;
        }

        /// Register a figure handler with a given index
        void register_figure_handle(size_t figure_index,
                                    const figure_handle &h) {
            global_figure_handles()[figure_index] = h;
        }

        /// Find an index for a new handler and register it there
        void register_figure_handle(const figure_handle &h) {
            size_t index_candidate = 1;
            for (const auto &[index, figure_handle] : global_figure_handles()) {
                if (index_candidate != index) {
                    break;
                }
                ++index_candidate;
            }
            register_figure_handle(index_candidate, h);
        }

        figure_handle register_figure_handle(bool quiet_mode) {
            size_t index_candidate = 1;
            for (const auto &[index, figure_handle] : global_figure_handles()) {
                if (index_candidate != index) {
                    break;
                }
                ++index_candidate;
            }
            figure_handle h =
                std::make_shared<class figure>(index_candidate, quiet_mode);
            register_figure_handle(index_candidate, h);
            return h;
        }
    } // namespace detail

    figure_handle figure() { return figure(false); }

    figure_handle figure(bool quiet_mode) {
        figure_handle h = detail::register_figure_handle(quiet_mode);
        detail::set_current_figure_handle(h);
        h->touch();
        return h;
    }

    figure_handle figure(figure_handle h) {
        detail::set_current_figure_handle(h);
        return h;
    }

    figure_handle figure(class figure *h) { return figure(figure_handle{h}); }

    figure_handle gcf() {
        figure_handle h = detail::current_figure_handle();
        if (!h) {
            h = figure();
        }
        return h;
    }

    const std::vector<std::shared_ptr<struct axes>> &figure::children() const {
        return children_;
    }

    void figure::children(
        const std::vector<std::shared_ptr<struct axes>> &children) {
        children_ = children;
    }

    std::tuple<std::vector<std::vector<scatter_handle>>,
               std::vector<histogram_handle>,
               std::vector<std::vector<axes_handle>>>
    figure::plotmatrix(const std::vector<std::vector<double>> &X,
                       const std::vector<std::vector<double>> &Y,
                       const std::string &line_spec,
                       bool histogram_on_diagonals) {
        bool p = this->quiet_mode();
        this->quiet_mode(true);

        // data
        size_t n_rows = X.size();
        size_t n_cols = Y.size();

        // results
        std::vector<std::vector<scatter_handle>> S(
            n_rows, std::vector<scatter_handle>(n_cols, nullptr));
        std::vector<histogram_handle> H(n_cols);

        // get min/max to adjust axes
        // adjust x-y ranges
        std::vector<double> X_min(X.size());
        std::vector<double> X_max(X.size());
        for (size_t i = 0; i < X.size(); ++i) {
            auto [xmin_it, xmax_it] =
                std::minmax_element(X[i].begin(), X[i].end());
            X_min[i] = *xmin_it;
            X_max[i] = *xmax_it;
        }
        std::vector<double> Y_min(Y.size());
        std::vector<double> Y_max(Y.size());
        for (size_t i = 0; i < Y.size(); ++i) {
            auto [ymin_it, ymax_it] =
                std::minmax_element(Y[i].begin(), Y[i].end());
            Y_min[i] = *ymin_it;
            Y_max[i] = *ymax_it;
        }

        // create scatter plots and histograms
        std::vector<std::vector<axes_handle>> axs(
            n_rows, std::vector<axes_handle>(n_cols, nullptr));
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols; ++j) {
                axs[i][j] = this->add_subplot(n_rows, n_cols, i * n_rows + j);
                if (i != j || !histogram_on_diagonals) {
                    scatter_handle s = axs[i][j]->scatter(X[j], Y[i]);
                    S[i][j] = s;
                    s->line_style(line_spec);
                    // set limits
                    axs[i][j]->x_axis().limits({X_min[i], X_max[i]});
                    axs[i][j]->y_axis().limits({Y_min[j], Y_max[j]});
                } else {
                    histogram_handle h = axs[i][j]->hist(X[i]);
                    H[i] = h;
                    // set limits
                    // we let the Y axis be defined automatically because
                    // it is in a different range from the other Y axes
                    axs[i][j]->x_axis().limits({X_min[i], X_max[i]});
                }
                if (i != n_rows - 1) {
                    axs[i][j]->xticks({});
                }
                if (j != 0) {
                    axs[i][j]->yticks({});
                }
                if (histogram_on_diagonals) {
                    if (i % 2 == 0) {
                        if (j == 0) {
                            axs[i][j]->y_axis().visible(false);
                        } else if (j == n_cols - 1) {
                            axs[i][j]->y2_axis().visible(true);
                        }
                    }
                    if (j % 2 == 1) {
                        if (i == 0) {
                            axs[i][j]->x2_axis().visible(true);
                        } else if (i == n_rows - 1) {
                            axs[i][j]->x_axis().visible(false);
                        }
                    }
                    if (n_rows % 2 == 1 && i == n_rows - 1) {
                        if (j == 0) {
                            axs[i][j]->y_axis().visible(true);
                        } else if (j == n_cols - 1) {
                            axs[i][j]->y2_axis().visible(false);
                        }
                    }
                }
            }
        }

        // get limits from the histograms and use the more extreme limit
        if (histogram_on_diagonals) {
            std::vector<double> H_min(X.size());
            std::vector<double> H_max(X.size());
            for (size_t i = 0; i < X.size(); ++i) {
                histogram_handle h = std::dynamic_pointer_cast<histogram>(
                    *axs[0][0]->children().begin());
                H_min[i] = *h->bin_edges().begin();
                H_max[i] = *std::prev(h->bin_edges().end());
            }
            for (size_t i = 0; i < axs.size(); ++i) {
                for (size_t j = 0; j < axs.size(); ++j) {
                    axs[i][j]->x_axis().limits({std::min(X_min[j], H_min[j]),
                                                std::max(X_max[j], H_max[j])});
                }
            }
        }

        // compact subplots
        float l_margin = axs[0][0]->x_origin();
        float r_margin =
            1.f - axs[0][n_cols - 1]->y_origin() - axs[0][n_cols - 1]->width();
        float b_margin = axs[n_rows - 1][0]->y_origin();
        float t_margin = 1.f - axs[0][0]->y_origin() - axs[0][0]->height();
        float subplot_width = (1.f - l_margin - r_margin) / n_cols;
        float subplot_height = (1.f - t_margin - b_margin) / n_rows;
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols; ++j) {
                axs[i][j]->width(subplot_width - 0.01);
                axs[i][j]->height(subplot_height - 0.01);
                axs[i][j]->x_origin(l_margin + subplot_width * j);
                axs[i][j]->y_origin(1.f - t_margin - subplot_height -
                                    subplot_height * i);
            }
        }
        this->quiet_mode(p);
        if (!p) {
            this->draw();
        }

        return std::make_tuple(S, H, axs);
    }

    bool save(const std::string &filename, const std::string &format) {
        return save(gcf(), filename, format);
    }

    bool save(const std::string &filename) { return save(gcf(), filename); }

    bool save(figure_handle f, const std::string &filename,
              const std::string &format) {
        return f->save(filename, format);
    }

    bool save(figure_handle f, const std::string &filename) {
        return f->save(filename);
    }

    const std::shared_ptr<backend::backend_interface> &figure::backend() const {
        return backend_;
    }

    void figure::backend(
        const std::shared_ptr<backend::backend_interface> &new_backend) {
        backend_ = new_backend;
    }

} // namespace matplot