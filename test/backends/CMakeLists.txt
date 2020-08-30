if (TARGET matplot_opengl)
    add_executable(matplot_opengl_test main.cpp)
    target_link_libraries(matplot_opengl_test PUBLIC matplot_opengl)
endif()