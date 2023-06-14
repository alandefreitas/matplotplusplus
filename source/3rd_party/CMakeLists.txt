set(EXPORTED_TARGETS)

#######################################################
### NodeSoup                                        ###
#######################################################
if(WITH_SYSTEM_NODESOUP)
  find_path(NODESOUP_INCLUDE_DIR nodesoup.hpp REQUIRED)
  find_library(NODESOUP_LIB nodesoup REQUIRED)

  add_library(nodesoup INTERFACE)
  target_include_directories(nodesoup INTERFACE $<BUILD_INTERFACE:${NODESOUP_INCLUDE_DIR}>)
  target_link_libraries(nodesoup INTERFACE ${NODESOUP_LIB})
else()
  add_library(nodesoup STATIC
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/algebra.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/algebra.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/fruchterman_reingold.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/fruchterman_reingold.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/kamada_kawai.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/kamada_kawai.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/layout.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/layout.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/src/nodesoup.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/include/nodesoup.hpp
          )
  set_target_properties(nodesoup PROPERTIES
          CXX_VISIBILITY_PRESET "hidden")
  target_include_directories(nodesoup
          PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/nodesoup/include>)
  target_compile_features(nodesoup PUBLIC cxx_std_17)

  # Hackfix to support MSVC standard library
  # https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=vs-2019
  target_compile_definitions(nodesoup PRIVATE _USE_MATH_DEFINES)

  if (${BUILD_SHARED_LIBS})
    set_target_properties(nodesoup PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
  endif ()

  if (MSVC)
    target_compile_options(nodesoup PRIVATE /EHsc)
  endif ()
endif()

# Install (only necessary for static lib build)
if(MASTER_PROJECT AND NOT BUILD_SHARED_LIBS)
  install(TARGETS nodesoup
      EXPORT Matplot++Targets
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib)
endif()


#######################################################
### CImg                                            ###
#######################################################
add_library(cimg INTERFACE)
list(APPEND EXPORTED_TARGETS cimg)
if(WITH_SYSTEM_CIMG)
  find_path(CIMG_INCLUDE_DIR CImg.h REQUIRED)
else()
  set(CIMG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cimg)
endif()
target_include_directories(cimg INTERFACE $<BUILD_INTERFACE:${CIMG_INCLUDE_DIR}>)

find_package(PkgConfig)

# Add plugins for CImg library
# Future work: download some of these libraries with CPM instead of trying to find them
#              OR just move them to a subdirectory.
# Rationale: Some libraries, such as the JPG library, are very small (cost almost zero) and
#            too important to not have here by default.
#            Lots of optional packages are not a good idea in general.
#            It makes the library much less "packagable" (https://youtu.be/sBP17HQAQjk)
#            and much more difficult to make sure it works on multiple OSs

if(MATPLOTPP_WITH_JPEG)
  find_package(JPEG REQUIRED)
else()
  find_package(JPEG QUIET)
endif()
if(JPEG_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_jpeg)
  target_link_libraries(cimg INTERFACE ${JPEG_LIBRARIES})
  target_include_directories(cimg INTERFACE ${JPEG_INCLUDE_DIRS})
endif()

if(MATPLOTPP_WITH_TIFF)
  find_package(TIFF REQUIRED)
else()
  find_package(TIFF QUIET)
endif()
if(TIFF_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_tiff)
  target_link_libraries(cimg INTERFACE ${TIFF_LIBRARIES})
  target_include_directories(cimg INTERFACE ${TIFF_INCLUDE_DIRS})
endif()

if(MATPLOTPP_WITH_PNG)
  find_package(ZLIB REQUIRED)
  find_package(PNG REQUIRED)
else()
  find_package(ZLIB QUIET)
  find_package(PNG QUIET)
endif()
if(ZLIB_FOUND AND PNG_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_zlib cimg_use_png)
  if (NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB INTERFACE)
    target_include_directories(ZLIB::ZLIB INTERFACE ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(ZLIB::ZLIB INTERFACE ${ZLIB_LIBRARIES})
    list(APPEND EXPORTED_TARGETS ZLIB::ZLIB)
  endif()
  target_link_libraries(cimg INTERFACE ZLIB::ZLIB)
  if (NOT TARGET png)
    add_library(png INTERFACE)
    target_include_directories(png INTERFACE ${PNG_INCLUDE_DIRS})
    target_link_libraries(cimg INTERFACE ${PNG_LIBRARIES})
    list(APPEND EXPORTED_TARGETS png)
  endif()
  target_link_libraries(cimg INTERFACE png)
endif()

if(MATPLOTPP_WITH_LAPACK)
  find_package(LAPACK REQUIRED)
else()
  find_package(LAPACK QUIET)
endif()
if(LAPACK_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_lapack)
  target_link_libraries(cimg INTERFACE ${LAPACK_LIBRARIES})
  target_include_directories(cimg INTERFACE ${LAPACK_INCLUDE_DIRS})
endif()

if(MATPLOTPP_WITH_BLAS)
  find_package(BLAS REQUIRED)
else()
  find_package(BLAS QUIET)
endif()
if(BLAS_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_blas)
  target_link_libraries(cimg INTERFACE ${BLAS_LIBRARIES})
  target_include_directories(cimg INTERFACE ${BLAS_INCLUDE_DIRS})
endif()

if(MATPLOTPP_WITH_FFTW)
  find_package(FFTW3)
  if(NOT FFTW3_FOUND)
    find_package(FFTW REQUIRED)
  endif()
else()
  find_package(FFTW3 QUIET)
  if(NOT FFTW3_FOUND)
    find_package(FFTW QUIET)
  endif()
endif()
if(FFTW3_FOUND OR FFTW_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_fftw3)
  if (NOT TARGET FFTW3::fftw3)
    add_library(FFTW3::fftw3 INTERFACE)
    target_include_directories(FFTW3::fftw3 INTERFACE ${FFTW3_INCLUDE_DIRS} ${FFTW_INCLUDE_DIRS})
    target_link_libraries(FFTW3::fftw3 INTERFACE ${FFTW3_LIBRARIES} ${FFTW_LIBRARIES})
    list(APPEND EXPORTED_TARGETS FFTW3::fftw3)
  endif()
  target_link_libraries(cimg INTERFACE FFTW3::fftw3)
endif()

if (CMAKE_MODULE_PATH)
  if(MATPLOTPP_WITH_OpenCV)
    find_package(OpenCV REQUIRED)
  else()
    find_package(OpenCV QUIET)
  endif()
  if (OpenCV_FOUND)
    target_compile_definitions(cimg INTERFACE cimg_use_opencv)
    if(NOT TARGET opencv_core)
      add_library(opencv_core INTERFACE)
      target_include_directories(opencv_core INTERFACE ${OpenCV_INCLUDE_DIRS})
      target_link_libraries(opencv_core INTERFACE ${OpenCV_LIBRARIES})
      list(APPEND EXPORTED_TARGETS opencv_core)
    endif()
    target_link_libraries(cimg INTERFACE opencv_core)
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
  target_compile_definitions(cimg INTERFACE cimg_display=0)
else()
  target_compile_definitions(cimg INTERFACE cimg_display=2)
  target_link_libraries(cimg INTERFACE gdi32)
endif()

# Install (only necessary for static lib build)
if(MASTER_PROJECT AND NOT BUILD_SHARED_LIBS)
  install(TARGETS ${EXPORTED_TARGETS}
      EXPORT Matplot++Targets)
endif()
