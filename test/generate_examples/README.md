# Generate all examples

This is a visual unit test where the `generate_examples` target will run all other executables from the `examples` directory and save the resulting images to the `documentation/examples` directory.

Everytime we update the library, we regenerate the examples to make sure everything is not only working ok but also *looking* good.

The library needs to be compiled with the CMake option `MATPLOTPP_BUILD_FOR_DOCUMENTATION_IMAGES` for this test to work. Also, you need to run this target from the project root directory so that the filesystem can find the build and documentation directories.
