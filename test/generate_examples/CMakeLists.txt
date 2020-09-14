find_package(Filesystem REQUIRED COMPONENTS Final Experimental)

if(CXX_FILESYSTEM_IS_EXPERIMENTAL)
	message(WARNING "Only std::experimental::filesystem is available, cannot build generate_examples")
else()
	add_executable(generate_examples main.cpp)
	target_link_libraries(generate_examples std::filesystem)
	target_compile_features(generate_examples PRIVATE cxx_std_17)
endif()