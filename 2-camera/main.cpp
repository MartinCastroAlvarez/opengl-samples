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
#include <cmath>

// Constantes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "Look At in View Space";

// Constantes de OpenGL
const int GL_VERSION_MAJOR = 3;
const int GL_VERSION_MINOR = 3;

// Constantes de la cámara
const glm::vec3 CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float FOV = 45.0f; // Ángulo de visión
const float NEAR_PLANE = 0.1f;  // Distancia mínima de visualización
const float FAR_PLANE = 100.0f; // Distancia máxima de visualización

// Constantes de la luz
const glm::vec3 LIGHT_POSITION = glm::vec3(2.0f, 4.0f, 2.0f);
const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

// Colores de los objetos
const glm::vec3 CUBE_COLOR = glm::vec3(1.0f, 0.5f, 0.2f);  // Taronja
const glm::vec3 PYRAMID_COLOR = glm::vec3(0.0f, 1.0f, 0.0f);  // Verde
const glm::vec3 PRISM_COLOR = glm::vec3(0.0f, 0.0f, 1.0f);  // Azul

// Constantes del color de fondo
const glm::vec4 CLEAR_COLOR = glm::vec4(0.15f, 0.15f, 0.18f, 1.0f);  // Gris oscuro

// Constantes de la cámara
const float CAMERA_HEIGHT = 2.0f;
const float CAMERA_RADIUS = 4.0f;

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

const float PYRAMID[] = {
    // Base
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,

    // Caras laterales
     0.0f,  0.5f,  0.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.5f,  0.5f,
     0.0f,  0.5f,  0.0f,  0.5f,  0.5f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.0f,
     0.0f,  0.5f,  0.0f,  0.0f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.5f, -0.5f,
     0.0f,  0.5f,  0.0f, -0.5f,  0.5f,  0.0f,
    -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.0f,
    -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
};

const float PRISM[] = {
    // Cara trasera
    -0.5f, -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,

    // Cara delantera
    -0.5f, -1.0f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -1.0f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -1.0f,  0.5f,  0.0f,  0.0f,  1.0f,

    // Lados
    -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  1.0f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -1.0f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -1.0f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -1.0f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,
     0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  1.0f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -1.0f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -1.0f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -1.0f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,
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

// Agregar función personalizada LookAt
glm::mat4 myLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
    glm::vec3 f = glm::normalize(center - eye);         // forward
    glm::vec3 r = glm::normalize(glm::cross(f, up));    // right
    glm::vec3 u = glm::cross(r, f);                     // recalculated up

    glm::mat4 result(1.0f);
    result[0][0] = r.x;
    result[1][0] = r.y;
    result[2][0] = r.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;

    result[3][0] = -glm::dot(r, eye);
    result[3][1] = -glm::dot(u, eye);
    result[3][2] = glm::dot(f, eye); // ojo: forward con signo invertido

    return result;
}

// Define una estructura para almacenar la información de las distintas formas, como una clase.
struct ShapeInfo {
    const float* data;
    int count;
    glm::vec3 pos;
    glm::vec3 color;
};

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE), CUBE, GL_STATIC_DRAW);

    // Configura los atributos de los vértices.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bucle principal de renderizado, mientras no se cierra la ventana.
    while (!glfwWindowShouldClose(window)) {
        onKeyPress(window);
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Cámara girando alrededor del centro
        float time = glfwGetTime();
        glm::vec3 cameraPos = glm::vec3(CAMERA_RADIUS * sin(time), CAMERA_HEIGHT, CAMERA_RADIUS * cos(time));
        glm::mat4 view = myLookAt(cameraPos, CAMERA_TARGET, CAMERA_UP);
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Define las formas a dibujar.
        ShapeInfo shapes[3] = {
            {CUBE, sizeof(CUBE)/(6*sizeof(float)), glm::vec3(0.0f, 0.0f, 0.0f), CUBE_COLOR},
            {PYRAMID, sizeof(PYRAMID)/(6*sizeof(float)), glm::vec3(2.0f, 0.0f, 0.0f), PYRAMID_COLOR},
            {PRISM, sizeof(PRISM)/(6*sizeof(float)), glm::vec3(-2.0f, 0.0f, 0.0f), PRISM_COLOR}
        };

        // Dibuja cada una de las formas, utilizando el VBO y el VAO.
        for (int i = 0; i < 3; ++i) {

            // Vincular los vértices del objeto.
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, shapes[i].count * 6 * sizeof(float), shapes[i].data, GL_STATIC_DRAW);
            glm::mat4 model = glm::mat4(1.0f);

            // Aplicar traslación a cada objeto, para que no se superpongan.
            model = glm::translate(model, shapes[i].pos);

            // Aplicar la transformación del objeto.
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Aplicar la transformación de la luz.
            glm::vec3 lightPosView = glm::vec3(view * glm::vec4(LIGHT_POSITION, 1.0f));

            // Aplicar la posición de la luz.
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPosView));

            // Aplicar el color de la luz.
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(LIGHT_COLOR));

            // Aplicar el color del objeto.
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(shapes[i].color));

            // Aplicar la posición de la cámara.
            glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));


            // Vincula el VAO, que contiene los vértices y los atributos de los vértices.
            glBindVertexArray(VAO);

            // Dibuja el objeto.
            glDrawArrays(GL_TRIANGLES, 0, shapes[i].count);
        }

        // Intercambia los buffers de color y profundidad.
        glfwSwapBuffers(window);

        // Espera a que ocurra un evento.
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
