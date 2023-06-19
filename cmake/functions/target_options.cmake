# @brief Enable pedantic warnings for a target
# - This does not propagate to other targets
function(target_pedantic_warnings TARGET_NAME)
    # Set warning levels to about the same level for MSVC, GCC, and Clang
    if (BUILD_SHARED_LIBS AND WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|MSVC|MinGW")
        # STL classes don't need to have dll-interface to be used by clients of Matplot++ classes
        target_compile_options(${TARGET_NAME} PRIVATE /wd4251)
        target_compile_options(${TARGET_NAME} PRIVATE /wd2220)
    else()
        if (MSVC)
            target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
        else ()
            target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
        endif ()
    endif()
endfunction()

# @brief Maybe enable pedantic warnings for a target
function(maybe_target_pedantic_warnings TARGET_NAME)
    if (MATPLOTPP_BUILD_WITH_PEDANTIC_WARNINGS)
        target_pedantic_warnings(${TARGET_NAME})
    endif ()
endfunction ()

# @brief Sets pedantic compiler options for all targets
# - In a more serious project, we would do that per target
# - Setting it for all targets unfortunately affects
# external libraries, which often lead to some warnings.
macro(add_pedantic_warnings)
    if (MSVC)
        add_compile_options(/W4 /WX)
    else ()
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    endif ()
endmacro()

# @brief Maybe set pedantic compiler options for all targets
macro(maybe_add_pedantic_warnings)
    if (MATPLOTPP_BUILD_WITH_PEDANTIC_WARNINGS)
        add_pedantic_warnings()
    endif ()
endmacro()

# @brief Enable compile options for MSVC only
# - Adding special compile options to make your code work on MSVC
#   is unfortunately very common, so this pattern becomes useful.
function(target_msvc_compile_options TARGET_NAME DEFINITION)
    if (MSVC)
        target_compile_options(${TARGET_NAME} PUBLIC ${DEFINITION})
    endif ()
endfunction()

# Enable exceptions for the target
function(target_exception_options TARGET_NAME)
    # MSVC requires this flag if the code uses C++ exception handling
    target_msvc_compile_options(${TARGET_NAME} PRIVATE /EHsc)
endfunction()



# @brief Enable big object files for the target
function(target_bigobj_options TARGET_NAME)
    # MSVC requires this flag if the file has a lot of code
    target_msvc_compile_options(${TARGET_NAME} PRIVATE /bigobj)
endfunction()

# @brief Enable utf-8 for the target
function(target_utf8_options TARGET_NAME)
    # MSVC requires this flag if the file has a lot of code
    target_msvc_compile_options(${TARGET_NAME} PRIVATE /utf-8)
endfunction()

# @brief Disable minmax for target
function(target_disable_minmax TARGET_NAME)
    message("Checking if min exists for ${TARGET_NAME}")
    if (MSVC)
        # Another hack to check for min in Windows.h
        # http://www.suodenjoki.dk/us/archive/2010/min-max.htm
        include(CheckSymbolExists)
        message("Looking in WinDef.h")
        check_symbol_exists(min "WinDef.h" HAVE_WINDOWS_MINMAX)
        if (NOT HAVE_WINDOWS_MINMAX)
            message("Looking in Windows.h")
            check_symbol_exists(min "Windows.h" HAVE_WINDOWS_MINMAX)
        endif()
        if (HAVE_WINDOWS_MINMAX)
            target_compile_definitions(${TARGET_NAME} PUBLIC NOMINMAX)
        else()
            message("MINMAX not found")
        endif()
    else()
        message("Compiler is not MSVC")
    endif()
endfunction()

# @brief Disable minmax for target
# This forces the NOMINMAX definition without even looking for
# min in WinDef.h. This is necessary because the solution
# based on check_symbol_exists hasn't been enough.
function(target_nominmax_definition TARGET_NAME)
    target_compile_definitions(${TARGET_NAME} PUBLIC NOMINMAX)
endfunction()
