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
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/Matplot++)
endif()


#######################################################
### CImg                                            ###
#######################################################
add_library(cimg INTERFACE)
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
find_package(JPEG)
if(JPEG_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_jpeg)
  target_link_libraries(cimg INTERFACE ${JPEG_LIBRARIES})
  target_include_directories(cimg INTERFACE ${JPEG_INCLUDE_DIRS})
endif()

find_package(TIFF)
if(TIFF_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_tiff)
  target_link_libraries(cimg INTERFACE ${TIFF_LIBRARIES})
  target_include_directories(cimg INTERFACE ${TIFF_INCLUDE_DIRS})
endif()

find_package(ZLIB)
if(ZLIB_FOUND)
  find_package(PNG)
  if (PNG_FOUND)
    target_compile_definitions(cimg INTERFACE cimg_use_zlib cimg_use_png)
    target_include_directories(cimg INTERFACE ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
    target_link_libraries(cimg INTERFACE ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})
  endif ()
endif()

find_package(LAPACK)
if(LAPACK_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_lapack)
  target_link_libraries(cimg INTERFACE ${LAPACK_LIBRARIES})
  target_include_directories(cimg INTERFACE ${LAPACK_INCLUDE_DIRS})
endif()

find_package(BLAS)
if(BLAS_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_blas)
  target_link_libraries(cimg INTERFACE ${BLAS_LIBRARIES})
  target_include_directories(cimg INTERFACE ${BLAS_INCLUDE_DIRS})
endif()

find_package(FFTW)
if(FFTW_FOUND)
  target_compile_definitions(cimg INTERFACE cimg_use_fftw3)
  target_link_libraries(cimg INTERFACE ${FFTW_LIBRARIES})
  target_include_directories(cimg INTERFACE ${FFTW_INCLUDE_DIRS})
endif()

if (CMAKE_MODULE_PATH)
  find_package(OpenCV QUIET)
  if (OpenCV_FOUND)
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
  target_compile_definitions(cimg INTERFACE cimg_display=0)
else()
  target_compile_definitions(cimg INTERFACE cimg_display=2)
  target_link_libraries(cimg INTERFACE gdi32)
endif()

# Install (only necessary for static lib build)
if(MASTER_PROJECT AND NOT BUILD_SHARED_LIBS)
  install(TARGETS cimg
      EXPORT Matplot++Targets)
endif()