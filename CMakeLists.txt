cmake_minimum_required(VERSION 3.10)

set(APP_NAME "renderer")

project(${APP_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/")
set(APP_DIR "${SRC_DIR}/app/")
set(ENGINE_DIR "${SRC_DIR}/engine/")

add_compile_definitions(DATA_DIRECTORY="${DATA_DIR}")

find_library(GLFW_LIBRARY
    NAMES glfw3 glfw3dll
    PATHS "${ENGINE_DIR}/contrib/lib"
    NO_DEFAULT_PATH
)

file(GLOB APP_SOURCES 
    "${APP_DIR}/*.cpp"
)

file(GLOB ENGINE_SOURCES 
    "${ENGINE_DIR}/*.cpp"
    "${ENGINE_DIR}/contrib/src/*.cpp"
    "${ENGINE_DIR}/contrib/src/*.c"
)

add_executable(${APP_NAME} ${SRC_DIR}/main.cpp ${APP_SOURCES} ${ENGINE_SOURCES})

target_include_directories(${APP_NAME}
    PRIVATE
        "${SRC_DIR}"
        "${ENGINE_DIR}/contrib/include"
)

if(GLFW_LIBRARY)
    target_link_libraries(${APP_NAME} ${GLFW_LIBRARY})
else()
    message(FATAL_ERROR "GLFW not found")
endif()