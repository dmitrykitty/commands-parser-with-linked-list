project(system_tests)

# Disabling include-what-you-use for GTest
set_target_properties(gtest PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
set_target_properties(gtest_main PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")

option(SYSTEM_TESTS_WITH_BOOST "set ON to use boost otherwise use tiny process library" ON)


file(GLOB INPUT_OUTPUT_FILES inputOutputData/*.txt)
message(" + Znaleziono pliki wejścia/wyjścia do testów: ${INPUT_OUTPUT_FILES}")

add_executable(${PROJECT_NAME} executableTests.cpp
    runningexternalprogram.h runningexternalprogram.cpp
    ${INPUT_OUTPUT_FILES})
add_dependencies(${PROJECT_NAME} ${MAIN_PROJECT_NAME})


if(SYSTEM_TESTS_WITH_BOOST)
    set(BOOST_ROOT "./libs/boost_1_81/") # either set it here or from the command line
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_NO_SYSTEM_PATHS TRUE)

    if (Boost_NO_SYSTEM_PATHS)
      set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost_1_81/")
      set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
      message("Using Boost: ${BOOST_INCLUDE_DIRS}")
      set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
    else()
        find_package(Boost REQUIRED) # header only libraries must not be added here
    endif (Boost_NO_SYSTEM_PATHS)

    target_include_directories(${PROJECT_NAME} PRIVATE ${BOOST_INCLUDE_DIRS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE SYSTEM_TESTS_WITH_BOOST="${SYSTEM_TESTS_WITH_BOOST}")
    target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)
    message("Using Boost library (to run process)")
else()
    add_subdirectory(./libs/tiny-process-library)
    set(BUILD_TESTING OFF)
    target_link_libraries(${PROJECT_NAME} PRIVATE tiny-process-library::tiny-process-library gtest gtest_main)
    message("Using Tiny Process Library")
endif(SYSTEM_TESTS_WITH_BOOST)

# target_link_libraries(${PROJECT_NAME} gtest gtest_main)

target_compile_definitions(${PROJECT_NAME} PRIVATE COMMANDS_PARSER_PATH="${PROJECT_BINARY_PATH}")
target_compile_definitions(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

############ KOMENDA: make run_system_tests
add_custom_target(run_${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
