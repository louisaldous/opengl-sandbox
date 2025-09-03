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

set( 
    APP_SOURCES
    "${APP_DIR}/App.cpp"
    "${APP_DIR}/Player.cpp"
)

set(
    ENGINE_SOURCES 
    "${ENGINE_DIR}/Camera.cpp"
    "${ENGINE_DIR}/InputManager.cpp"
    "${ENGINE_DIR}/Renderer.cpp"
    "${ENGINE_DIR}/Shader.cpp"
    "${ENGINE_DIR}/contrib/src/stb_impl.cpp"
    "${ENGINE_DIR}/contrib/src/glad.c"
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