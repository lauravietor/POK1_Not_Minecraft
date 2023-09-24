cmake_minimum_required(VERSION 3.5)

project(POK_OpenGL_noQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(POK_OpenGL_noQt src/main.cpp
    include/main.h
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

include(GNUInstallDirs)
install(TARGETS POK_OpenGL_noQt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
