//
// Created by Alan Freitas on 26/08/20.
//

#ifndef MATPLOTPLUSPLUS_BACKEND_INTERFACE_H
#define MATPLOTPLUSPLUS_BACKEND_INTERFACE_H

#include <matplot/detail/config.h>
#include <memory>
#include <string>
#include <vector>

namespace matplot {
    class figure_type;
    namespace backend {
        /// Inherit from this class to create a new backend
        /// - Interactive backends show the plots on a window
        /// - Non-interactive backends save the plots to a file
        /// - The only backend for which we currently provide a
        /// concrete implementation is gnuplot pipes. A gnuplot
        /// pipe can be an interactive or non-interactive depending
        /// on the terminal.
        /// - Unfortunately, backends based on gnuplot terminals
        /// need to be treated differently because they consume
        /// commands rather vertices. It might be technically
        /// possible to come up with a workaround to make gnuplot
        /// plot each vertex but this is definitely not worth it.
        /// - The interface for backends that expect lists of
        /// vertices to draw are somewhat inspired in ImGui backends
        /// and Matplotlib backends. You can see some examples at:
        /// https://github.com/matplotlib/matplotlib/blob/master/src/_backend_agg.h
        /// https://github.com/ocornut/imgui/tree/master/examples
        /// - If implementing such an interface, the functions
        /// for gnuplot pipes should just be ignored.
        /// - These are placeholders with no concrete implementation.
        /// We still need to define the functions that generate vertices
        /// in the classes that inherit from axes_object, and these
        /// functions in the interface are likely to change as we
        /// need. A good place to start is probably the line object,
        /// which is fundamental for most plots.
        class MATPLOT_EXPORTS backend_interface {
            /// Virtual functions you can override to create any backend
          public:
            /// \brief True if backend is in interactive mode
            /// One backends might support both interactive and
            /// non-interactive mode.
            /// For instance, the gnuplot backend can support
            /// both modes by changing its terminal.
            /// It is also possible to export the OpenGL state
            /// as an image with the glReadPixels function.
            /// You can combine glReadPixels with CImg
            /// to export the image in a variety of formats.
            /// To export vector graphics from OpenGL,
            /// libraries such as http://www.geuz.org/gl2ps/
            /// can be used.
            virtual bool is_interactive();

            /// \brief If non-interactive, get the file where we should output
            /// our data
            virtual const std::string &output();

            /// \brief If non-interactive, get the format in which we should
            /// output our data
            virtual const std::string &output_format();

            /// \brief If non-interactive, set the file where we should output
            /// our data This function can use the file extension to
            /// automatically set the output format
            virtual bool output(const std::string &filename);

            /// \brief If non-interactive, set the file and the file format for
            /// outputting data
            virtual bool output(const std::string &filename,
                                const std::string &file_format);

            /// Get the current width
            /// The user might have changed the image width manually.
            /// Matplot++ needs to be aware of that.
            virtual unsigned int width();

            /// \brief Get height
            virtual unsigned int height();

            /// \brief Set width
            /// For when the user programmatically sets the width
            virtual void width(unsigned int new_width);

            /// \brief Set height
            virtual void height(unsigned int new_height);

            /// \brief Get the current position_x (for interactive backends)
            /// The user might have changed the image position_x manually.
            /// Matplot++ needs to be aware of that.
            virtual unsigned int position_x();

            /// \brief Get position_y (for interactive backends)
            virtual unsigned int position_y();

            /// \brief Set position_x (for interactive backends)
            /// For when the user programmatically sets the position_x
            virtual void position_x(unsigned int new_position_x);

            /// \brief Set position_y (for interactive backends)
            virtual void position_y(unsigned int new_position_y);

            /// \brief Set window title
            virtual void window_title(const std::string& title);

            /// \brief Get window title
            virtual std::string window_title();

            /// \brief Tell the backend we are about to draw a new image
            /// The backend might reject starting this new image
            /// For instance, the user already closed the window
            /// and there's no point in feeding commands to the backend
            virtual bool new_frame();

            /// \brief Tell the backend this new image is over
            /// The backend is free to plot whatever it's been
            /// buffering
            /// \return True if everything is ok
            virtual bool render_data();

            /// \brief Tell the backend to wait for user interaction
            /// Until then, the backend should block execution if possible
            /// Figures use this in the show function
            virtual void show(matplot::figure_type *);

            /// \brief True if the user requested to close the window
            /// This function allows the backend to send a signal
            /// indicating the user has asked to close the window
            virtual bool should_close();

            /// \brief True if the backend supports fonts
            /// We can avoid some commands if it doesn't
            virtual bool supports_fonts();

            /// Public functions you need to override to create a new
            /// backend based on vertices, such as OpenGL, Agg, etc...
            /// These functions are likely to change as we come up
            /// with concrete implementations based on vertices.
            /// \see
            /// https://github.com/matplotlib/matplotlib/blob/master/src/_backend_agg.h
            /// \see https://github.com/ocornut/imgui/tree/master/examples
          public:
            /// \brief Draws background on the image
            virtual void draw_background(const std::array<float, 4> &color);

            /// \brief Draws rectangle on the image
            virtual void draw_rectangle(const double x1, const double x2,
                                        const double y1, const double y2,
                                        const std::array<float, 4> &color);

            /// \brief Draw a path on the image
            /// Many backends will require the path to be floats
            /// but Matplot++ words with doubles, so it's up to
            /// the backend to implement this conversion however
            /// it seems more efficient
            virtual void draw_path(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   const std::array<float, 4> &color);

            /// \brief Draw markers on the image
            virtual void draw_markers(const std::vector<double> &x,
                                      const std::vector<double> &y,
                                      const std::vector<double> &z = {});

            /// \brief Draw text on the image
            virtual void draw_text(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   const std::vector<double> &z = {});

            /// \brief Draw image matrix on the image
            virtual void
            draw_image(const std::vector<std::vector<double>> &x,
                       const std::vector<std::vector<double>> &y,
                       const std::vector<std::vector<double>> &z = {});

            /// \brief Draw rectangle on the image
            virtual void draw_triangle(const std::vector<double> &x,
                                       const std::vector<double> &y,
                                       const std::vector<double> &z = {});

            /// We can certainly include more functions here, such as
            /// draw_mesh, draw_rectangle, etc...
            /// However, these functions should have a default implementation
            /// that would recur to more primitive functions.
            /// For instance, draw_rectangle should have a default
            /// implementation to draw a rectangle based on two calls to
            /// draw_triangle (for filled rectangles) or an implementation based
            /// on draw_path (for unfilled rectangles). Otherwise, it would be
            /// very expensive to start a new backend. These function should be
            /// complementary functions that would be used to improve
            /// performance on existing backends.

            /// Public functions you need to override only if your backend
            /// is based on gnuplot
            /// If not, just leave it as it is and the default implementations
            /// should do just fine.
          public:
            /// \brief If true, this backend does not work by consuming vertices
            /// Most functions above will be ignored.
            /// The figure object will send gnuplot commands to this backend
            /// instead of vertices.
            /// The default implementation returns false.
            virtual bool consumes_gnuplot_commands();

            /// \brief Send line and newline to gnu plot pipe and flush
            /// We can buffer the lines until the end of data is sent
            virtual void run_command(const std::string &text);

            /// \brief Include a comment in the gnuplot code
            /// This is useful when tracing the gnuplot commands
            /// and when generating a gnuplot file.
            virtual void include_comment(const std::string &text);
        };
    } // namespace backend

} // namespace matplot

#endif // MATPLOTPLUSPLUS_BACKEND_INTERFACE_H
