cmake_minimum_required(VERSION 3.5)

project(POK_OpenGL_noQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(POK_OpenGL_noQt src/main.cpp
    include/main.h
    include/shaderprogram.h src/shaderprogram.cpp
    assets/shaders/vertex_color.frag
    assets/shaders/do_nothing.vert
    assets/shaders/upside_down.vert
    assets/shaders/move_circle.vert
    assets/shaders/color_pos.vert
    include/stb_image.h
    src/std_image.cpp
    assets/shaders/texture.vert
    assets/shaders/texture.frag
    assets/shaders/transform4x4.vert
    assets/shaders/transform4x4.vert
    assets/shaders/model_view_proj_matrices.vert
)

add_custom_command(
    TARGET POK_OpenGL_noQt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
    )

target_include_directories(POK_OpenGL_noQt PRIVATE include/)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (OPENGL_FOUND)
    target_include_directories(POK_OpenGL_noQt PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(POK_OpenGL_noQt ${OPENGL_gl_LIBRARY})
endif()

find_package(glfw3 REQUIRED)
target_link_libraries(POK_OpenGL_noQt glfw)

add_subdirectory(external/glad/)
target_link_libraries(POK_OpenGL_noQt glad)

add_subdirectory(external/glm/)
target_link_libraries(POK_OpenGL_noQt glm)

include(GNUInstallDirs)
install(TARGETS POK_OpenGL_noQt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
