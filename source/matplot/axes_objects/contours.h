//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_CONTOURS_H
#define MATPLOTPLUSPLUS_CONTOURS_H

#include <matplot/detail/config.h>

#include <array>
#include <matplot/util/common.h>
#include <matplot/util/concepts.h>
#include <matplot/util/contourc.h>
#include <matplot/util/handle_types.h>

#include <matplot/core/figure_type.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/line_spec.h>

namespace matplot {
    class axes_type;

    /// Create and store a set of contour lines or filled regions.
    /// We have a class just for contours instead of using the
    /// gnuplot surface. So we include functions here to
    /// calculate the contour lines outside gnuplot.
    /// The reason is that converting the  gnuplot contours to
    /// 2d xlim involves workarounds a 3d map view or plotting to a
    /// temporary table, that don't interact well with other 2d
    /// objects. It's also no possible to plot filled contours
    /// with gnuplot. At most, one can plot a very high resolution
    /// under the contour lines, but this is very. The only
    /// way to have full control of how the contour lines will look
    /// like is to calculate the contour lines outside gnuplot,
    /// even if our algorithm is not perfect.
    class MATPLOT_EXPORTS contours : public axes_object {
      public:
        /// X, Y grids and their Z values
        /// Every X column has the same value for all rows.
        ///        This value increases in the next column.
        /// Every Y row has the same value for all columns.
        ///          This value increases in the next rows.
        /// Thus, the high values indices represent values
        ///               that are graphically on the top:
        /// (X[3][0], Y[3][0]) (X[3][1], Y[3][1]) (X[3][2], Y[3][2])
        /// (X[2][0], Y[2][0]) (X[2][1], Y[2][1]) (X[2][2], Y[2][2])
        /// (X[1][0], Y[1][0]) (X[1][1], Y[1][1]) (X[1][2], Y[1][2])
        /// (X[0][0], Y[0][0]) (X[0][1], Y[0][1]) (X[0][2], Y[0][2])
        /// \param parent Parent xlim
        /// \param X X grid
        /// \param Y Y grid
        /// \param Z Z heights
        /// \param line_spec Line properties
        contours(class axes_type *parent, const vector_2d &X,
                 const vector_2d &Y, const vector_2d &Z,
                 std::string_view line_spec = "");

        contours(class axes_type *parent, const vector_2d &Z,
                 std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        contours(const axes_handle &parent, Args &&... args)
            : contours(parent.get(), std::forward<Args>(args)...) {}

        virtual ~contours() = default;

      public /* mandatory virtual functions */:
        std::string set_variables_string() override;
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        bool requires_colormap() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        class contours &line_style(std::string_view line_spec);

        const matplot::line_spec &line_spec() const;
        matplot::line_spec &line_spec();
        class contours &line_spec(const class line_spec &line_spec);

        const vector_2d &Y_data() const;
        class contours &Y_data(const vector_2d &Y_data);

        const vector_2d &X_data() const;
        class contours &X_data(const vector_2d &X_data);

        const vector_2d &Z_data() const;
        class contours &Z_data(const vector_2d &Z_data);

        const vector_2d &x_data() const;
        class contours &x_data(const vector_2d &x_data);

        const vector_2d &y_data() const;
        class contours &y_data(const vector_2d &y_data);

        const vector_2d &z_data() const;
        class contours &z_data(const vector_2d &z_data);

        bool contour_text() const;
        class contours &contour_text(bool contour_text);

        float font_size() const;
        class contours &font_size(const float &font_size);

        const std::string font() const;
        class contours &font(std::string_view font);

        const std::string &font_weight() const;
        class contours &font_weight(std::string_view font_weight);

        const color_array &font_color() const;
        class contours &font_color(const color_array &font_color);
        class contours &font_color(std::string_view font_color);

        bool visible() const;
        class contours &visible(bool visible);

        size_t n_levels() const;
        class contours &n_levels(size_t n_levels);

        const std::vector<double> &levels() const;
        class contours &levels(const std::vector<double> &levels);

        bool filled() const;
        class contours &filled(bool filled);

        bool colormap_line_when_filled() const;

        class contours &
        colormap_line_when_filled(bool colormap_line_when_filled);

      public /* getters and setters bypassing the line_spec */:
        float line_width() const;
        class contours &line_width(float line_width);

        const std::array<float, 4> &color() const;

        template <class T> contours &color(T c) {
            line_spec().color(c);
            return *this;
        }
        inline class contours &color(std::initializer_list<float> c) {
            line_spec().color(c);
            return *this;
        }

      protected:
        void make_sure_data_is_preprocessed();
        void clear_preprocessed_data();
        bool is_lower_level(size_t line_index, size_t segment_begin,
                            size_t segment_end);
        std::pair<vector_1d, vector_1d>
        fill_border_jump(double start_x, double start_y, double end_x,
                         double end_y, double x_min, double x_max, double y_min,
                         double y_max, bool is_parent);

      public:
        enum class extend_option { neither, min, max, both };

        /// Determine some automatic contour levels for data ranging from z_min
        /// to z_max Each library will use a different algorithm to determine
        /// levels. In our case, this is similar to the algorithm for automatic
        /// histogram edges. The difference is that the final histogram edges
        /// fall outside the range [z_min,z_max] while we want all contour lines
        /// to fall inside [z_min,z_max] Select contour levels to span the data.
        //        The target number of levels, *N*, is used only when the
        //        scale is not log and default locator is used.
        //
        //        We need two more levels for filled contours than for
        //        line contours, because for the latter we need to specify
        //        the lower and upper boundary of each range.
        //
        //        For example,
        //        a single contour boundary, say at z = 0, requires only
        //        one contour line, but two filled regions, and therefore
        //        three levels to provide boundaries for both regions.
        ///  Assign values to :attr:`layers` based on :attr:`levels`,
        ///        adding extended layers as needed if contours are filled.
        ///        For line contours, layers simply coincide with levels;
        ///        a line is a thin layer.  No extended levels are needed
        ///        with line contours.
        static std::vector<double>
        determine_contour_levels(double z_min, double z_max, size_t n_levels_,
                                 extend_option ext = extend_option::neither);
        static std::vector<double>
        determine_contour_levels(const vector_2d &Z, size_t n_levels_,
                                 extend_option ext = extend_option::neither);

        /// Return ``(lowers, uppers)`` levels for filled contours.
        std::pair<vector_1d, vector_1d> get_lowers_and_uppers();

        void process_contour_levels();
        void initialize_preprocessed_data();
        void check_xyz();
        void initialize_x_y();

        void process_all_segs_and_all_kinds();

        double zmin() override;
        double zmax() override;

      protected:
        /// Pre-processed contour lines
        std::vector<QuadContourGenerator::vertices_list_type> lines_;

        /// Pre-processed contour lines for filled plots
        /// This uses another algorithm for pairs of levels
        /// We generate the lines twice for filled plots because
        /// the closed polygons of filled plots are not good enough
        /// to represent everything.
        /// Also, this makes it much easier to label the unfilled lines.
        std::vector<QuadContourGenerator::vertices_list_type> filled_lines_;

        /// These codes indicate what's happening in a filled line
        /// Starting a polygon, closing a polygon, or middle of a polygon
        std::vector<QuadContourGenerator::codes_list_type> codes_;

        /// Object with the algorithm to generate contour lines
        QuadContourGenerator contour_generator_;

        /// Segments of filled contours
        using level_index_type = size_t;
        using begin_index_type = size_t;
        using end_index_type = size_t;
        using area_type = double;
        using line_segment_type =
            std::tuple<level_index_type, begin_index_type, end_index_type>;
        using parent_and_children_type =
            std::tuple<line_segment_type, std::vector<line_segment_type>,
                       area_type>;
        std::vector<parent_and_children_type> line_segments_;

      protected:
        /// Line style
        class line_spec line_spec_;

        /// Data in the xlim
        vector_2d X_data_{};
        vector_2d Y_data_{};
        vector_2d Z_data_{};

        /// Parameters
        size_t n_levels_{0};
        bool manual_n_levels_{false};
        std::vector<double> levels_{};
        bool manual_levels_{false};
        std::vector<double> _levels{};
        std::vector<double> layers_ = {};

        /// Style
        bool filled_{false};
        vector_1d linewidths_ = {};
        bool antialiased_ = false;
        bool _corner_mask = false;
        bool colormap_line_when_filled_ = false;

        /// Pre-processed data
        std::vector<class line_spec> linestyles_{};
        vector_1d extent_ = {};
        std::vector<color_array> cmap_ = {};
        std::vector<color_array> colors_ = {};
        double norm_ = NaN;
        double vmin_ = NaN;
        double vmax_ = NaN;
        extend_option extend_ = extend_option::neither;
        size_t nchunk_ = 0;
        double zmin_{NaN};
        double zmax_{NaN};

        /// Text around the lines
        bool contour_text_{false};
        std::optional<float> font_size_{std::nullopt};
        std::optional<std::string> font_{std::nullopt};
        std::string font_weight_{"normal"};
        color_array font_color_{0, 0, 0, 0};

        /// True if visible
        bool visible_{true};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_CONTOURS_H
