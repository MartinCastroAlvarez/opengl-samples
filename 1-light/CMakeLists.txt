# Versión mínima requerida de CMake.
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto.
project(PhongBlinnViewSpace)

# Configurar el estándar de C++ a usar.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencieas

# OpenGL: API principal para gráficos.
find_package(OpenGL REQUIRED)

# GLEW: Gestor de extensiones de OpenGL.
find_package(GLEW REQUIRED)

# GLFW: Biblioteca para crear ventanas y manejador de contexto OpenGL.
find_package(glfw3 REQUIRED)

# GLM: Biblioteca matemática para gráficos.
find_package(glm REQUIRED)

# Define el nombre del archivo principal del programa.
add_executable(PhongBlinnViewSpace main.cpp)

# Enlaza las bibliotecas necesarias al ejecutable.
target_link_libraries(PhongBlinnViewSpace
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

# Copia los shaders al directorio de build
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 