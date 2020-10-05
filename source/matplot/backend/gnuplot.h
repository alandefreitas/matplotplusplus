//
// Created by Alan Freitas on 26/08/20.
//

#ifndef MATPLOTPLUSPLUS_BACKEND_GNUPLOT_H
#define MATPLOTPLUSPLUS_BACKEND_GNUPLOT_H

#include <array>
#include <chrono>
#include <matplot/backend/backend_interface.h>
#include <tuple>

namespace matplot::backend {
    class gnuplot : public backend_interface {
      public:
        gnuplot();
        virtual ~gnuplot();
        bool is_interactive() override;
        const std::string &output() override;
        const std::string &output_format() override;
        bool output(const std::string &filename) override;
        bool output(const std::string &filename,
                    const std::string &file_format) override;
        unsigned int width() override;
        unsigned int height() override;
        unsigned int position_x() override;
        unsigned int position_y() override;
        void position_x(unsigned int new_position_x) override;
        void position_y(unsigned int new_position_y) override;
        void width(unsigned int new_width) override;
        void height(unsigned int new_height) override;
        bool new_frame() override;
        bool render_data() override;
        bool supports_fonts() override;

      public:
        bool consumes_gnuplot_commands() override;
        void run_command(const std::string &command) override;
        void include_comment(const std::string &comment) override;

      public /* gnuplot pipe functions */:
        /// We "render the data" by flushing the commands
        bool flush_commands();

        /// Identify the default terminal type in the system
        static std::string default_terminal_type();
        static bool terminal_is_available(std::string_view);
        static std::tuple<int, int, int> gnuplot_version();
        static bool terminal_has_title_option(const std::string &t);
        static bool terminal_has_size_option(const std::string &t);
        static bool terminal_has_position_option(const std::string &t);
        static bool terminal_has_enhanced_option(const std::string &t);
        static bool terminal_has_color_option(const std::string &t);
        static bool terminal_has_font_option(const std::string &t);

      public: /* gnuplot pipe constexprs */
        // True if the windows persist after closing the program
        // False by default because this is VERY annoying in
        // some repetitive tasks involving lots of plots
        // You can change the default here or run the command
        // set terminal "name" persist
        static constexpr bool windows_should_persist_by_default = false;

        // True if we should wait at least 5 seconds after the last
        // flush to close gnuplot
        static constexpr bool dont_let_it_close_too_fast = false;

        // True if we allow to use the mouse interactively in the window
        // Most of the time, this is just annoying when we scroll without
        // meaning to It is very very hard to use the mouse properly
        // http://www.gnuplot.info/files/gpReadMouseTest.c
        static constexpr bool allow_using_mouse = true;

#if defined(TRACE_GNUPLOT_COMMANDS) &&                                         \
    !defined(MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES)
        static constexpr bool trace_commands = true;
#else
        static constexpr bool trace_commands = false;
#endif

        static constexpr size_t pipe_capacity_worst_case = BUFSIZ;
        /// File formats for figures and properties of terminals
        static constexpr std::array<
            std::pair<std::string_view, std::string_view>, 33>
        extension_terminal() {
            return std::array<std::pair<std::string_view, std::string_view>,
                              33>{
                std::pair{".html", "canvas"}, std::pair{".cgm", "cgm"},
                std::pair{".txt", "dumb"}, std::pair{".md", "dumb"},
                std::pair{".dxf", "dxf"}, std::pair{".emf", "emf"},
                // std::pair{".emtex", "emtex"},
                std::pair{".eps", "postscript"}, std::pair{".eps", "epscairo"},
                std::pair{".eps", "epslatex"}, std::pair{".gif", "gif"},
                std::pair{".hpgl", "hpgl"}, std::pair{".jpeg", "jpeg"},
                std::pair{".jpg", "jpeg"}, std::pair{".tex", "epslatex"},
                std::pair{".mf", "mf"}, std::pair{".mp", "mp"},
                std::pair{".pcl5", "pcl5"}, std::pair{".pdf", "pdf"},
                std::pair{".pdf", "pdfcairo"}, std::pair{".png", "pngcairo"},
                std::pair{".png", "png"}, std::pair{".tex", "pslatex"},
                std::pair{".tex", "context"}, std::pair{".tex", "texdraw"},
                std::pair{".tex", "tikz"},
                // std::pair{".tex", "eepic"},
                // std::pair{".tex", "tpic"},
                std::pair{".pstex", "pstex"},
                std::pair{".pstricks", "pstricks"},
                // std::pair{".qms", "qms"},
                std::pair{".sixel", "sixelgd"}, std::pair{".svg", "svg"},
                std::pair{".tek40xx", "tek40xx"},
                std::pair{".tek410x", "tek410x"},
                // std::pair{".tgif", "tgif"},
                std::pair{".tkcanvas", "tkcanvas"},
                // std::pair{".tpic", "tpic"},
                std::pair{".vttek", "vttek"}};
        }

      private:
        // Pipe to gnuplot process
        FILE *pipe_;

        // How many bytes we put in the pipe
        size_t bytes_in_pipe_{0};

        // Current gnuplot terminal we should
        std::string terminal_{"qt"};

        // Position and size
        std::array<unsigned, 4> position_{680, 558, 560, 420};

        // Time we last flush, to avoid flushing results too fast
        std::chrono::high_resolution_clock::time_point last_flush_;

        // File output, if non-interactive
        std::string output_{};

        // Whether we should include comments in the commands
        bool include_comments_ = trace_commands;
    };
} // namespace matplot::backend

#endif // MATPLOTPLUSPLUS_BACKEND_GNUPLOT_H
