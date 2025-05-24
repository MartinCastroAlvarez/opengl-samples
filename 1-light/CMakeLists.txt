# Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(PhongBlinnViewSpace)

# Configurar el estándar de C++ a usar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar y configurar las dependencias necesarias
# OpenGL: API principal para gráficos
find_package(OpenGL REQUIRED)

# GLEW: Gestor de extensiones de OpenGL
find_package(GLEW REQUIRED)

# GLFW: Biblioteca para crear ventanas y contexto OpenGL
find_package(glfw3 REQUIRED)

# GLM: Biblioteca matemática para gráficos
find_package(glm REQUIRED)

# Crear el ejecutable principal
add_executable(PhongBlinnViewSpace main.cpp)

# Enlazar las bibliotecas necesarias al ejecutable
target_link_libraries(PhongBlinnViewSpace
    OpenGL::GL        # OpenGL core
    GLEW::GLEW       # Extensiones de OpenGL
    glfw             # Gestión de ventanas
    glm::glm         # Matemáticas para gráficos
) 