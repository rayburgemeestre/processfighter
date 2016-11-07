cmake_minimum_required(VERSION 2.6.2)
project(processfighter)

include("cmake/gtestmockscript.cmake")


set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(Threads)
#find_package(OpenMP)
#SET(PLATFORM_SPECIFIC_LIBS "-lpthread")

find_package(SFML 1 COMPONENTS system window graphics network audio REQUIRED)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if (NOT DEBUG)
    set (COMPILE_FLAGS "-O3 -Wall -funsigned-char -std=c++14 -fno-omit-frame-pointer -pedantic -mssse3")
else()
    set (COMPILE_FLAGS "-O0 -g -fno-inline -Wall -funsigned-char -std=c++14 -fno-omit-frame-pointer -pedantic")
endif()

add_definitions(${COMPILE_FLAGS})

include_directories("./include/" "./")
file(GLOB_RECURSE processfighter_SOURCES "src/**.cpp" "include/**.h*")
add_executable(processfighter ${processfighter_SOURCES})

target_link_libraries(processfighter ${SFML_LIBRARIES})
target_link_libraries(processfighter ${PROJECT_SOURCE_DIR}/libs/tiny-process-library/build/libtiny-process-library.a)
target_link_libraries(processfighter ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS processfighter DESTINATION bin)

file(GLOB_RECURSE processfighter_test_SOURCES "")
file(GLOB TEST_FILES "test/*.cpp" "src/game_state.cpp")
# Don't include Main.cpp from src
add_executable(processfighter_tests ${MAIN_SRC} ${TEST_FILES} ${processfighter_test_SOURCES})
# Set link libraries (order matters)
target_link_libraries(processfighter_tests libgtest)
target_link_libraries(processfighter_tests libgmock)
target_link_libraries(processfighter_tests ${SFML_LIBRARIES})
target_link_libraries(processfighter_tests ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME processfighter COMMAND processfighter_tests --unittests)
