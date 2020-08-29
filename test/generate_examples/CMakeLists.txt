find_package(Filesystem REQUIRED)
add_executable(generate_examples main.cpp)
target_link_libraries(generate_examples std::filesystem)
target_compile_features(generate_examples PRIVATE cxx_std_17)