#ifndef MATPLOTPLUSPLUS_FIGURE_TYPE_H
#define MATPLOTPLUSPLUS_FIGURE_TYPE_H

#include <array>
#include <fstream>
#include <iostream>
#include <matplot/backend/backend_registry.h>
#include <matplot/backend/gnuplot.h>
#include <matplot/util/colors.h>
#include <matplot/util/handle_types.h>
#include <matplot/util/popen.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace matplot {
    class axes_type;

    /// \class Figure
    /// From Figures, we can create plot handles and
    /// multiplot handles that contain other plot handles.
    /// - Figure objects have axis objects
    /// - Axis objects have plot object
    ///
    /// When we plot the any of these subobjects, they will
    /// send the appropriate commands to the parent Figure/pipe
    /// to plot it.
    ///
    /// If we create more Figures, we create more plot windows,
    /// but we don't really recommend doing that. It's better
    /// to only use the default pipe for everything and use
    /// multiplots if more plots are needed.
    class figure_type {
      public:
        friend class axes_type;
        // Remove the copy operators because users are not
        // supposed to use this object directly.
        // Users will use a figure_handle which can be copied
        // and still reference to the same object.
        figure_type(figure_type const &) = delete;
        void operator=(figure_type const &) = delete;

        /// \brief Create a Figure window
        figure_type();

        /// \brief Create a Figure window
        explicit figure_type(bool quiet_mode);

        /// \brief Create a Figure window
        explicit figure_type(size_t index);

        /// \brief Create a Figure window
        explicit figure_type(size_t index, bool quiet_mode);

        /// \brief The destructor closes the pipe
        virtual ~figure_type();

      public /* manage axes */:
        /// \brief Create new axes in a figure
        axes_handle add_axes(bool replace_if_overlap = false);

        /// \brief Create new axes in position
        /// Don't replace any axes that might overlap
        /// Replace any axes exactly in the same position
        /// \param position: x, y, width, height (all from 0 to 1)
        /// \return Handle to new axes
        axes_handle add_axes(std::array<float, 4> position);

        /// \brief Add existing axes to the figure
        /// If we find axes in the exact same position, we can replace
        /// the axes with the new axes or we can get a handle
        /// to the existing axes. If we don't find axes in the exact
        /// same position, we remove any other axes overlapping the new
        /// axes and return a handle to the new axes.
        /// \param ax Handle to the new axes
        /// \param replace_if_overlap Replace any axes that overlap at all
        /// \param replace_if_same_position Replace any axes that are exactly in
        /// the same position \return Handle to new axes
        axes_handle add_axes(std::shared_ptr<class axes_type> ax,
                             bool replace_if_overlap,
                             bool replace_if_same_position);

        /// \brief Add a subplot
        /// The subplot is an axes with its position determined by the number of
        /// rows, columns, and id
        axes_handle add_subplot(size_t rows, size_t cols, size_t plot_id,
                                bool replace_if_same_position = false);

        /// \brief Make axes a subplot of this figure
        axes_handle add_subplot(size_t rows, size_t cols, size_t plot_id,
                                axes_handle axes);

        /// \brief Create new axes in a subplot covering all positions in P
        axes_handle add_subplot(size_t rows, size_t cols,
                                std::initializer_list<size_t> P,
                                bool replace_if_same_position = false);

        /// \brief Create new subplot in a specific position
        axes_handle add_subplot(std::array<float, 4> position,
                                bool replace_if_same_position = false);

        /// \brief Get next tile in a tiled layout
        /// In tiled layouts, we create new axes for each new tile.
        /// The tile positions are determined according to the number
        /// of rows/columns provided by `tiledlayout()`.
        /// When creating a new tile, if there are more tiles than rows *
        /// columns, we increase the number of rows or columns to make the new
        /// tile fit. The position of old tiled is adjusted to the new number of
        /// rows and columns.
        void tiledlayout(size_t rows = 1, size_t cols = 1);

        /// \brief Add next tile to plot
        axes_handle nexttile();

        /// \brief Add next tile to plot at position index
        axes_handle nexttile(size_t index);

        /// Get reference to current axes / create new axes if it does not exist
        std::shared_ptr<class axes_type> current_axes();

        /// Get reference to current axes
        std::shared_ptr<class axes_type> current_axes() const;

        /// Set current axes in the figure
        void current_axes(const std::shared_ptr<class axes_type> &current_axes);

        /// \brief Get reference to vector with all child axes
        const std::vector<std::shared_ptr<class axes_type>> &children() const;
        void
        children(const std::vector<std::shared_ptr<class axes_type>> &children);

      protected:
        static std::array<float, 4>
        calculate_subplot_position(size_t rows, size_t cols, size_t plot_id);

      public /* manage figure reactive mode */:
        /// \brief Draw the current figure (for figures in quiet/non-reactive
        /// mode) Flush the buffer to the pipe, updating or creating the plot
        /// window
        void draw();

        /// \brief Draw the current figure and pause the console
        /// This is useful to keep the figure open, usually by the end
        /// of a program. Never use this function inside a library
        /// meant to be used non-interactively.
        void show();

        /// \brief Plot the figure if not in quiet mode
        /// In some applications, we want the default behaviour
        /// where we update the plot whenever we change a figure
        /// property. This is usually what we want when tracking
        /// the behaviour of an algorithm of something like that.
        /// In some other applications, however, we just want
        /// to plot when we explicitly call the plot function.
        /// This is the case when producing plots for papers, etc.
        /// This is also the case in applications where we need
        /// performance.
        void touch();

        /// True if user required the backend to close
        bool should_close();

        /// True if in quiet mode (not reactive)
        bool quiet_mode() const;

        /// Set quite mode (non-reactive)
        void quiet_mode(bool quiet_mode);

        /// True if in quiet mode (not reactive)
        bool reactive_mode() const;

        /// Set quite mode (non-reactive)
        void reactive_mode(bool reactive_mode);

        /// Turn on reactive mode (off quiet mode)
        void ion();

        /// Turn off reactive mode (on quiet mode)
        void ioff();

      public /* save figure */:
        /// Save figure
        bool save(const std::string &filename, const std::string &format);
        bool save(const std::string &filename);

      public /* plot functions for figure */:
        /// Create matrix of axes with scatter plots - X / Y
        std::tuple<std::vector<std::vector<scatter_handle>>,
                   std::vector<histogram_handle>,
                   std::vector<std::vector<axes_handle>>>
        plotmatrix(const std::vector<std::vector<double>> &X,
                   const std::vector<std::vector<double>> &Y,
                   std::string_view line_spec = "of",
                   bool histogram_on_diagonals = false);

        /// Create matrix of axes with scatter plots - X / X
        auto plotmatrix(const std::vector<std::vector<double>> &X,
                        std::string_view line_spec = "of",
                        bool histogram_on_diagonals = false) {
            return this->plotmatrix(X, X, line_spec, true);
        }

      public /* figure properties */:
        const std::shared_ptr<backend::backend_interface> &backend() const;
        void
        backend(const std::shared_ptr<backend::backend_interface> &new_backend);

        void name(std::string_view name);
        std::string name() const;

        size_t number() const;

        void color(const color_array &c);
        void color(const std::array<float, 3> &c);
        void color(std::string_view c);
        void color(const enum color &c);
        color_array color() const;

        bool custom_color() const;
        void custom_color(bool custom_color);

        void position(const std::array<unsigned, 4> &c);
        void position(unsigned x, unsigned y, unsigned w, unsigned h);
        void position(unsigned x, unsigned y);
        void size(unsigned w, unsigned h);
        std::array<unsigned, 4> position() const;

        unsigned x_position() const;
        void x_position(unsigned x);
        unsigned y_position() const;
        void y_position(unsigned y);
        unsigned width() const;
        void width(unsigned w);
        unsigned height() const;
        void height(unsigned h);

        void number_title(bool number_title);
        bool number_title() const;

        const std::string &font() const;
        void font(std::string_view font);

        float font_size() const;
        void font_size(float font_size);

        const std::string &title() const;
        void title(std::string_view title);

        const color_array &title_color() const;
        void title_color(const color_array &title_color);

        float title_font_size_multiplier() const;
        void title_font_size_multiplier(float title_font_size_multiplier);

        /// Return if Figure is valid
        explicit operator bool() const;

      protected:
        // Draw, depending on the backend type
        void send_draw_commands();
        void send_gnuplot_draw_commands();

      protected /* run commands on a gnuplot pipe if that's our backend */:
        /// \brief Send line and newline to gnu plot pipe and flush
        /// We can buffer the lines until the end of data is sent
        void run_command(const std::string &text);

        /// \brief Include a comment in the gnuplot code
        /// This is useful when tracing the gnuplot commands
        /// and when generating a gnuplot file.
        void include_comment(const std::string &text);

        /// \brief Send commands to gnuplots
        void flush_commands();

        /// Plots an empty plot command
        void plot_empty_plot();

        /// \brief Run commands to set the appropriate window properties
        /// This applies of member objects to the window (font, size, ...)
        void run_figure_properties_command();
        void run_terminal_init_command();
        void run_window_color_command();
        void run_unset_window_color_command();
        void run_multiplot_command();

      private:
        std::string generate_window_title() const;

      private:
        // The default backend for this figure
        std::shared_ptr<backend::backend_interface> backend_{nullptr};

        // Figure properties
        bool quiet_mode_ = true;
        bool is_plotting_{false};
        std::string name_;
        std::string title_;
        color_array title_color_{0, 0, 0, 0};
        float title_font_size_multiplier_{1.4};
        size_t number_{1};
        bool number_title_{true};
        // ARGB
        color_array color_{0., 0.9400, 0.9400, 0.9400};
        bool custom_color_{false}; // was this color defined by the user
        std::string font_{"Helvetica"};
        float font_size_{10.};

        // Axes
        std::vector<std::shared_ptr<class axes_type>> children_;
        std::shared_ptr<class axes_type> current_axes_;

        // Axes tiles
        size_t current_tile_index_ = 0;
        size_t tiledlayout_rows_ = 1;
        size_t tiledlayout_cols_ = 1;
        bool tiledlayout_flow_ = true;
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_FIGURE_TYPE_H
