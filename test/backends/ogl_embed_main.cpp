#include <matplot/backend/opengl_embed.h>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    // Initialize
    if (!glfwInit()) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Initialization hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow *window =
        glfwCreateWindow(backend::opengl_embed::default_screen_width,
                         backend::opengl_embed::default_screen_height,
                         "My Existing Application", nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Set window as context
    glfwMakeContextCurrent(window);

    // Set callback when we resize the window
    glfwSetFramebufferSizeCallback(
        window, backend::opengl_embed::framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Create figure with backend
    auto f = figure<backend::opengl_embed>(true);
    auto ax = f->current_axes();
    ax->xlim({0.,2. * pi});
    ax->ylim({-1.5,1.5});
    ax->yticks(iota(-1.5,0.5,+1.5));
    ax->xticks(iota(0.,1.,2. * pi));

    // Start rendering
    while (!glfwWindowShouldClose(window)) {
        // Check if there is any input to process
        backend::opengl_embed::process_input(window);

        // Create plots
        float timeValue = glfwGetTime();
        std::vector<double> x = linspace(0., 2. * pi);
        std::vector<double> y = transform(x, [&](auto x) { return sin(x + timeValue); });
        ax->hold(off);
        ax->plot(x, y, "-o");
        ax->hold(on);
        ax->plot(x, transform(y, [](auto y) { return -y; }), "--xr");
        ax->plot(x, transform(x, [](auto x) { return x / pi - 1.; }), "-:gs");
        ax->plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

        // Draw the figure
        f->draw();

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}