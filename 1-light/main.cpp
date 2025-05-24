#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Constantes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "Phong-Blinn Lighting in View Space";

// Constantes de OpenGL
const int GL_VERSION_MAJOR = 3;
const int GL_VERSION_MINOR = 3;

// Constantes de la cámara
const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float FOV = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

// Constantes de la luz
const glm::vec3 LIGHT_POSITION = glm::vec3(1.2f, 1.0f, 2.0f);
const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 OBJECT_COLOR = glm::vec3(1.0f, 0.5f, 0.2f);  // Taronja

// Constantes de la rotación
const float ROTATION_SPEED = 0.5f;
const glm::vec3 ROTATION_AXIS = glm::vec3(0.5f, 1.0f, 0.0f);

// Constantes del color de fondo
const glm::vec4 CLEAR_COLOR = glm::vec4(0.2f, 0.48f, 0.72f, 1.0f);  // #337ab7 (Azul)

// Constantes de geometría
const float CUBE[] = {
    // Cara trasera
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    // Cara delantera
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    // Cara izquierda
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    // Cara derecha
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    // Cara inferior
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    // Cara superior
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
};

// Un cono simple con base circular y punta
const float CONE[] = {
    // Base del cono (triángulos que forman un círculo)
    0.0f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  // Centro de la base
    0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  // Punto en el borde
    0.0f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  // Otro punto en el borde

    // Lados del cono (triángulos que van desde la base hasta la punta)
    0.0f, 0.5f, 0.0f,  0.0f, 0.5f, 0.5f,    // Punta del cono
    0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  // Punto en la base
    0.0f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  // Otro punto en la base
};

// Un cilindro simple con tapas circulares
const float CYLINDER[] = {
    // Tapa superior
    0.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    // Centro de la tapa superior
    0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    // Punto en el borde
    0.0f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,    // Otro punto en el borde

    // Tapa inferior
    0.0f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  // Centro de la tapa inferior
    0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  // Punto en el borde
    0.0f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  // Otro punto en el borde

    // Lado del cilindro (rectángulo)
    0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,    // Esquina superior derecha
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Esquina inferior derecha
    0.0f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,    // Esquina superior izquierda
    0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,   // Esquina inferior izquierda
};

// Función para cargar el código de un shader desde un archivo.
std::string load(const char* filePath) {
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
    }
    return shaderCode;
}

// Función para compilar un shader, porque el compilador de OpenGL no lo hace automáticamente.
unsigned int compile(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Inicialización de funciones.
void onResize(GLFWwindow* window, int width, int height);
void onKeyPress(GLFWwindow* window);

// Maneja la entrada del teclado.
void onKeyPress(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Cuando cambia el tamano de la ventana, se actualiza el viewport.
void onResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 

int main() {

    // Inicialización de GLFW, que se encarga de crear la ventana y el contexto OpenGL.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configura GLFW para usar OpenGL 3.3 Core Profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crea una ventana con el tamano especificado y el titulo especificado.
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Establece la ventana como la ventana actual.
    glfwMakeContextCurrent(window);

    // Activar el test de profundidad
    glEnable(GL_DEPTH_TEST);

    // Inicializa GLEW.
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Establece el callback para el cambio de tamano de la ventana.
    glfwSetFramebufferSizeCallback(window, onResize);

    // Carga el archivo .vert contiene el código del shader de vértices que se encarga de transformar los puntos al view space.
    std::string vertexCode = load("shaders/phong-blinn.vert");
    unsigned int vertexShader = compile(vertexCode.c_str(), GL_VERTEX_SHADER);

    // Carga el archivo .frag contiene el código del shader de fragmentos que se encarga de pintar el objeto.
    std::string fragmentCode = load("shaders/phong-blinn.frag");
    unsigned int fragmentShader = compile(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // Enlaza el programa con los shadres.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Limpia los shaders de la memoria luego de enlazarse al programa.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Crea un VAO y un VBO, que son objetos que se utilizan para almacenar los datos de los vértices.
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Define el objeto a renderizar.
    const float* vertices = CUBE;
    // const float* vertices = CONE;
    // const float* vertices = CYLINDER;

    // Vincula el VBO al VAO.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Usar el tamaño correcto según la figura seleccionada
    int verticesCount = 0;
    if (vertices == CUBE) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE), CUBE, GL_STATIC_DRAW);
        verticesCount = sizeof(CUBE) / (6 * sizeof(float));
    } else if (vertices == CONE) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(CONE), CONE, GL_STATIC_DRAW);
        verticesCount = sizeof(CONE) / (6 * sizeof(float));
    } else if (vertices == CYLINDER) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(CYLINDER), CYLINDER, GL_STATIC_DRAW);
        verticesCount = sizeof(CYLINDER) / (6 * sizeof(float));
    } 

    // Configura los atributos de los vértices.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bucle principal de renderizado, mientras no se cierra la ventana.
    while (!glfwWindowShouldClose(window)) {

        // Cuando se presiona una tecla, se maneja la entrada del teclado.
        onKeyPress(window);

        // Limpia el buffer de color y profundidad.
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activa el programa de shaders.
        glUseProgram(shaderProgram);

        // Configura las matrices de vista y proyección.
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
        glm::mat4 view = glm::lookAt(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP);

        // Rotar el cubo usando el tiempo para animación continua
        float time = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, time * ROTATION_SPEED, ROTATION_AXIS);

        // Pasa las matrices al shader.
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Configura las propiedades de la luz.
        glm::vec3 lightPos = glm::vec3(view * glm::vec4(LIGHT_POSITION, 1.0f));

        // Pasa las variables uniformes al shader.
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(LIGHT_COLOR));
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(OBJECT_COLOR));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(CAMERA_POSITION));

        // Renderiza el objeto.
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);

        // Intercambia los buffers de color y profundidad.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpia los recursos de la memoria.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Termina GLFW.
    glfwTerminate();
    return 0;
}
