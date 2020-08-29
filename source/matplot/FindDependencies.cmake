# Add nodesoup library
CPMAddPackage(NAME nodesoup VERSION 0.1 GITHUB_REPOSITORY olvb/nodesoup GIT_TAG 4bcd8cd2d6c963ccf053f8c19dbb63c065467527 DOWNLOAD_ONLY TRUE)
add_library(nodesoup
        ${nodesoup_SOURCE_DIR}/src/algebra.cpp
        ${nodesoup_SOURCE_DIR}/src/algebra.hpp
        ${nodesoup_SOURCE_DIR}/src/fruchterman_reingold.cpp
        ${nodesoup_SOURCE_DIR}/src/fruchterman_reingold.hpp
        ${nodesoup_SOURCE_DIR}/src/kamada_kawai.cpp
        ${nodesoup_SOURCE_DIR}/src/kamada_kawai.hpp
        ${nodesoup_SOURCE_DIR}/src/layout.cpp
        ${nodesoup_SOURCE_DIR}/src/layout.hpp
        ${nodesoup_SOURCE_DIR}/src/nodesoup.cpp
        ${nodesoup_SOURCE_DIR}/include/nodesoup.hpp
        )
target_include_directories(nodesoup PUBLIC ${nodesoup_SOURCE_DIR}/include/)

# Add CImg library
CPMAddPackage(NAME cimg GITHUB_REPOSITORY dtschump/CImg VERSION 0.221 GIT_TAG v.221 DOWNLOAD_ONLY TRUE)
find_package(PkgConfig)
add_library(cimg INTERFACE)
target_include_directories(cimg INTERFACE ${cimg_SOURCE_DIR})

# Add plugins for CImg library
# Future work: download some of these libraries with CPM instead of trying to find them
# Rationale: Some libraries, such as the JPG library, are very small (cost almost zero) and
#            too important to not have here by default.
find_package(JPEG)
if(JPEG_FOUND)
    message("JPEG Found")
    target_compile_definitions(cimg INTERFACE cimg_use_jpeg)
    target_link_libraries(cimg INTERFACE ${JPEG_LIBRARIES})
    target_include_directories(cimg INTERFACE ${JPEG_INCLUDE_DIRS})
endif()

find_package(TIFF)
if(TIFF_FOUND)
    message("TIFF Found")
    target_compile_definitions(cimg INTERFACE cimg_use_tiff)
    target_link_libraries(cimg INTERFACE ${TIFF_LIBRARIES})
    target_include_directories(cimg INTERFACE ${TIFF_INCLUDE_DIRS})
endif()

find_package(ZLIB)
if(ZLIB_FOUND)
    message("ZLIB Found")
    target_compile_definitions(cimg INTERFACE cimg_use_zlib)
    target_link_libraries(cimg INTERFACE ${ZLIB_LIBRARIES})
    target_include_directories(cimg INTERFACE ${ZLIB_INCLUDE_DIRS})
    # PNG requires ZLIB
    find_package(PNG)
    if (PNG_FOUND)
        message("PNG Found")
        target_compile_definitions(cimg INTERFACE cimg_use_png)
        target_link_libraries(cimg INTERFACE ${PNG_LIBRARIES})
        target_include_directories(cimg INTERFACE ${PNG_INCLUDE_DIRS})
    endif()
endif()

find_package(LAPACK)
if(LAPACK_FOUND)
    message("LAPACK Found")
    target_compile_definitions(cimg INTERFACE cimg_use_lapack)
    target_link_libraries(cimg INTERFACE ${LAPACK_LIBRARIES})
    target_include_directories(cimg INTERFACE ${LAPACK_INCLUDE_DIRS})
endif()

find_package(BLAS)
if(BLAS_FOUND)
    message("BLAS Found")
    target_compile_definitions(cimg INTERFACE cimg_use_blas)
    target_link_libraries(cimg INTERFACE ${BLAS_LIBRARIES})
    target_include_directories(cimg INTERFACE ${BLAS_INCLUDE_DIRS})
endif()

find_package(FFTW)
if(FFTW_FOUND)
    message("FFTW Found")
    target_compile_definitions(cimg INTERFACE cimg_use_fftw3)
    target_link_libraries(cimg INTERFACE ${FFTW_LIBRARIES})
    target_include_directories(cimg INTERFACE ${FFTW_INCLUDE_DIRS})
endif()

pkg_check_modules(OPENEXR OpenEXR)
if(OPENEXR_FOUND)
    message("OPENEXR Found")
    target_compile_definitions(cimg INTERFACE cimg_use_openexr)
    target_link_libraries(cimg INTERFACE ${OPENEXR_LIBRARIES})
    target_include_directories(cimg INTERFACE ${OPENEXR_INCLUDE_DIRS})
endif()

pkg_check_modules(MAGICK Magick++)
if(MAGICK_FOUND)
    message("MAGICK Found")
    target_compile_definitions(cimg INTERFACE cimg_use_magick)
    target_link_libraries(cimg INTERFACE ${MAGICK_LIBRARIES})
    target_include_directories(cimg INTERFACE ${MAGICK_INCLUDE_DIRS})
endif()

if (CMAKE_MODULE_PATH)
    find_package(OpenCV QUIET)
    if (OpenCV_FOUND)
        message("OpenCV Found")
        target_compile_definitions(cimg INTERFACE cimg_use_opencv)
        target_link_libraries(cimg INTERFACE ${OpenCV_LIBRARIES})
        target_include_directories(cimg INTERFACE ${OpenCV_INCLUDE_DIRS})
    endif()
else()
    message("No CMAKE_MODULE_PATH path for OpenCV configured")
endif()


if(LIBAVCODEC_FOUND AND LIBAVFORMAT_FOUND AND LIBSWSCALE_FOUND AND LIBAVUTIL_FOUND)
    target_compile_definitions(cimg INTERFACE cimg_use_ffmpeg)
    target_include_directories(cimg INTERFACE ${LIBAVFORMAT_INCLUDE_DIRS} ${LIBAVFORMAT_INCLUDE_DIRS}/libavformat)
    target_include_directories(cimg INTERFACE ${LIBAVCODEC_INCLUDE_DIRS} ${LIBAVCODEC_INCLUDE_DIRS}/libavcodec )
    target_include_directories(cimg INTERFACE ${LIBSWSCALE_INCLUDE_DIRS} ${LIBSWSCALE_INCLUDE_DIRS}/libswscale)
    target_include_directories(cimg INTERFACE ${LIBAVUTIL_INCLUDE_DIRS} ${LIBAVUTIL_INCLUDE_DIRS}/libavutil )
    target_link_libraries(cimg INTERFACE ${LIBAVFORMAT_LIBRARIES} )
    target_link_libraries(cimg INTERFACE ${LIBAVCODEC_LIBRARIES} )
    target_link_libraries(cimg INTERFACE ${LIBSWSCALE_LIBRARIES} )
    target_link_libraries(cimg INTERFACE ${LIBAVUTIL_LIBRARIES} )
endif()

if(NOT WIN32)
    if (NOT APPLE)
        find_package(Threads REQUIRED)
        target_link_libraries(cimg INTERFACE ${CMAKE_THREAD_LIBS_INIT})
    endif()

    find_package(X11)
    if(X11_FOUND)
        target_compile_definitions(cimg INTERFACE cimg_display=1)
        target_compile_definitions(cimg INTERFACE cimg_use_xshm cimg_use_xrandr)
        target_link_libraries(cimg INTERFACE ${X11_LIBRARIES})
        target_include_directories(cimg INTERFACE ${X11_INCLUDE_DIRS})
    else()
        target_compile_definitions(cimg INTERFACE cimg_display=0)
    endif()
else()
    target_compile_definitions(cimg INTERFACE cimg_display=2)
    target_link_libraries(cimg INTERFACE gdi32)
endif()
