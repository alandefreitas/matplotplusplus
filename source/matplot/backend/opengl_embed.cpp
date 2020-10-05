//
// Created by Alan Freitas on 26/08/20.
//

#include "opengl_embed.h"
#include <future>
#include <iostream>
#include <matplot/core/figure_registry.h>
#include <matplot/core/figure_type.h>
#include <matplot/util/common.h>
#include <thread>

namespace matplot::backend {

    opengl_embed::opengl_embed() { create_shaders(); }

    opengl_embed::opengl_embed(bool create_shaders_at_construction) {
        if (create_shaders_at_construction) {
            create_shaders();
        }
    }

    void opengl_embed::create_shaders() {
        // Create shaders
        const char *draw_2d_single_color_vertex_shader_source =
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "uniform float windowHeight;\n"
            "uniform float windowWidth;\n"
            //"out vec4 vertexColor;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4((aPos.x/windowWidth)*2-1, (aPos.y/windowHeight)*2-1, 0.0, 1.0);"
            //"   vertexColor = aColor;\n"
            "}\0";
        unsigned int draw_2d_single_color_vertex_shader;
        draw_2d_single_color_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(draw_2d_single_color_vertex_shader, 1, &draw_2d_single_color_vertex_shader_source, NULL);
        glCompileShader(draw_2d_single_color_vertex_shader);
        int success;
        char info_log[512];
        glGetShaderiv(draw_2d_single_color_vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(draw_2d_single_color_vertex_shader, 512, NULL, info_log);
            throw std::runtime_error(
                std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") +
                info_log);
        }

        // create and compile fragment shader
        const char *draw_2d_single_color_fragment_shader_source =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "\n"
            "uniform vec4 ourColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = ourColor;\n"
            "}";
        unsigned int draw_2d_single_color_fragment_shader;
        draw_2d_single_color_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(draw_2d_single_color_fragment_shader, 1, &draw_2d_single_color_fragment_shader_source, NULL);
        glCompileShader(draw_2d_single_color_fragment_shader);
        glGetShaderiv(draw_2d_single_color_fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(draw_2d_single_color_fragment_shader, 512, NULL, info_log);
            throw std::runtime_error(
                std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") +
                info_log);
        }

        // Link shaders into shader program
        draw_2d_single_color_shader_program_ = glCreateProgram();
        glAttachShader(draw_2d_single_color_shader_program_, draw_2d_single_color_vertex_shader);
        glAttachShader(draw_2d_single_color_shader_program_, draw_2d_single_color_fragment_shader);
        glLinkProgram(draw_2d_single_color_shader_program_);
        // check if linking was successful
        glGetProgramiv(draw_2d_single_color_shader_program_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(draw_2d_single_color_shader_program_, 512, NULL, info_log);
            throw std::runtime_error(
                std::string("ERROR::SHADER_PROGRAM::LINKING_FAILED\n") +
                info_log);
        }

        // Delete the shader objects
        glDeleteShader(draw_2d_single_color_vertex_shader);
        glDeleteShader(draw_2d_single_color_fragment_shader);

//        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_vertex_attributes_available_);
//        std::cout << "Maximum number of vertex attributes supported: " << n_vertex_attributes_available_ << std::endl;
    }

    opengl_embed::~opengl_embed() {
        glDeleteProgram(draw_2d_single_color_shader_program_);
    }

    bool opengl_embed::is_interactive() { return true; }

    const std::string &opengl_embed::output() {
        throw std::logic_error("output not implemented yet");
    }

    const std::string &opengl_embed::output_format() {
        throw std::logic_error("output_format not implemented yet");
    }

    bool opengl_embed::output(const std::string &filename) {
        throw std::logic_error("output not implemented yet");
    }

    bool opengl_embed::output(const std::string &filename,
                          const std::string &file_format) {
        throw std::logic_error("output not implemented yet");
    }

    unsigned int opengl_embed::width() {
        GLint m_viewport[4];
        glGetIntegerv( GL_VIEWPORT, m_viewport );
        return m_viewport[2];
    }

    unsigned int opengl_embed::height() {
        GLint m_viewport[4];
        glGetIntegerv( GL_VIEWPORT, m_viewport );
        return m_viewport[3];
    }

    void opengl_embed::width(unsigned int new_width) {
        throw std::logic_error("width not implemented yet");
    }

    void opengl_embed::height(unsigned int new_height) {
        throw std::logic_error("height not implemented yet");
    }

    unsigned int opengl_embed::position_x() {
        throw std::logic_error("position_x not implemented yet");
    }

    unsigned int opengl_embed::position_y() {
        throw std::logic_error("position_y not implemented yet");
    }

    void opengl_embed::position_x(unsigned int new_position_x) {
        throw std::logic_error("position_x not implemented yet");
    }

    void opengl_embed::position_y(unsigned int new_position_y) {
        throw std::logic_error("position_y not implemented yet");
    }

    bool opengl_embed::new_frame() { return true; }

    bool opengl_embed::render_data() {
        return true;
    }

    void opengl_embed::draw_rectangle(const double x1, const double x2,
                                           const double y1, const double y2,
                                           const std::array<float, 4> &color) {
        // Create and bind vertex array object
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Copy vertex data into the buffer's memory
        std::vector<float> vertices = {
            // x, y, z, r, g, b
            static_cast<float>(x2), static_cast<float>(y2), // top right
            static_cast<float>(x2), static_cast<float>(y1), // bottom right
            static_cast<float>(x1), static_cast<float>(y1), // bottom left
            static_cast<float>(x1), static_cast<float>(y2)  // top left
        };

        // Create and bind vertex buffer object
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        std::vector<unsigned int> indices = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        // Element buffer
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

        // Set the vertex attributes pointers
        int vertex_attribute_location = 0;
        size_t stride = 2 * sizeof(float);
        glVertexAttribPointer(vertex_attribute_location, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void *)0);
        glEnableVertexAttribArray(0);

        // Set the color attribute pointers
        // int color_attribute_location = 1;
        // glVertexAttribPointer(color_attribute_location, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3*sizeof(float)));
        // glEnableVertexAttribArray(1);

        // Activate our shader program
        glUseProgram(draw_2d_single_color_shader_program_);

        // Set window size
        int windowHeightLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "windowHeight");
        if (windowHeightLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform1f(windowHeightLocation, static_cast<float>(height()));

        int windowWidthLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "windowWidth");
        if (windowWidthLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform1f(windowWidthLocation, static_cast<float>(width()));

        // Set color
        int vertexColorLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "ourColor");
        if (vertexColorLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform4f(vertexColorLocation, color[1], color[2], color[3], 1.f-color[0]);

        // Bind element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

        // Unbind our vertex array
        glBindVertexArray(0);

        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void opengl_embed::draw_background(const std::array<float, 4> &color) {
        glClearColor(color[1], color[2], color[3], 1.f - color[0]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void opengl_embed::show(class matplot::figure_type *f) {
        backend_interface::show(f);
    }

    bool opengl_embed::supports_fonts() { return false; }

    void opengl_embed::draw_path(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::array<float, 4> &color) {
        // Copy vertex data into the buffer's memory
        std::vector<float> vertices;
        for (size_t i = 0; i < x.size(); ++i) {
            vertices.emplace_back(static_cast<float>(x[i]));
            vertices.emplace_back(static_cast<float>(y[i]));
        }

        // Create and bind vertex array object
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Create and bind vertex buffer object
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        // Set the vertex attributes pointers
        int vertex_attribute_location = 0;
        size_t stride = 2 * sizeof(float);
        glVertexAttribPointer(vertex_attribute_location, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void *)0);
        glEnableVertexAttribArray(0);

        // Activate our shader program
        glUseProgram(draw_2d_single_color_shader_program_);

        // Set window size
        int windowHeightLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "windowHeight");
        if (windowHeightLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform1f(windowHeightLocation, static_cast<float>(height()));

        int windowWidthLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "windowWidth");
        if (windowWidthLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform1f(windowWidthLocation, static_cast<float>(width()));

        // Set color
        int vertexColorLocation = glGetUniformLocation(draw_2d_single_color_shader_program_, "ourColor");
        if (vertexColorLocation == -1) {
            throw std::runtime_error("can't find uniform location");
        }
        glUniform4f(vertexColorLocation, color[1], color[2], color[3], 1.f-color[0]);

        // Bind element buffer
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(x.size()));

        // Unbind our vertex array
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void opengl_embed::draw_markers(const std::vector<double> &x,
                                const std::vector<double> &y,
                                const std::vector<double> &z) {
        throw std::logic_error("draw_markers not implemented yet");
    }

    void opengl_embed::draw_text(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &z) {
        throw std::logic_error("draw_text not implemented yet");
    }

    void opengl_embed::draw_image(const std::vector<std::vector<double>> &x,
                              const std::vector<std::vector<double>> &y,
                              const std::vector<std::vector<double>> &z) {
        throw std::logic_error("draw_image not implemented yet");
    }

    void opengl_embed::draw_triangle(const std::vector<double> &x,
                                 const std::vector<double> &y,
                                 const std::vector<double> &z) {
        throw std::logic_error("draw_triangle not implemented yet");
    }

    void opengl_embed::process_input(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void opengl_embed::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        // Make sure the viewport matches the new window dimensions;
        // Note that width and height will be significantly larger than
        // specified on retina displays.
        glViewport(0, 0, width, height);
        auto f = gcf();
        auto b = f->backend();
        auto ogl_b = std::dynamic_pointer_cast<opengl_embed>(b);
        if (ogl_b != nullptr) {
            f->draw();
        }
    }



} // namespace matplot::backend