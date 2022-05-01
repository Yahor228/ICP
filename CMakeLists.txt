# CMakeList.txt: проект CMake для ICP; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("ICP" CXX)
set(CMAKE_CXX_STANDARD 23)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (ICP ${hdrs} ${srcs})

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")

target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)

include (WINDEPLOYQT.cmake)
if(WIN32)
    WINDEPLOYQT("${PROJECT_NAME}")
endif(WIN32)


# TODO: Добавьте тесты и целевые объекты, если это необходимо.
