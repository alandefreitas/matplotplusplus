if (MATPLOTPP_BUILD_EXPERIMENTAL_OPENGL_BACKEND)
    # Use this to create an OpenGL window as backend
    add_executable(matplot_opengl_test ogl_main.cpp)
    target_link_libraries(matplot_opengl_test PUBLIC matplot_opengl)

    # Use this to create plots inside an existing OpenGL application
    add_executable(matplot_opengl_embed_test ogl_embed_main.cpp)
    target_link_libraries(matplot_opengl_embed_test PUBLIC matplot_opengl)
endif ()
