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
#include <array>

// Constantes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "Multiple Cameras";

// Constantes de OpenGL
const int GL_VERSION_MAJOR = 3;
const int GL_VERSION_MINOR = 3;

// Constantes de las cámaras
const glm::vec3 PRIMARY_CAMERA_POS = glm::vec3(0.0f, 0.5f, 2.5f); // Más cerca y centrada en Y
const glm::vec3 PRIMARY_CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 PRIMARY_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float PRIMARY_FOV = 75.0f; // Ángulo de visión
const float PRIMARY_NEAR_PLANE = 0.1f;  // Distancia mínima de visualización
const float PRIMARY_FAR_PLANE = 100.0f; // Distancia máxima de visualización
const glm::vec3 SECONDARY_CAMERA_POS = glm::vec3(-0.5f, -2.0f, -2.0f);  // más alejado e inclinado
const glm::vec3 SECONDARY_CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 SECONDARY_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float SECONDARY_FOV = 75.0f; // Ángulo de visión
const float SECONDARY_NEAR_PLANE = 0.3f;  // Distancia mínima de visualización
const float SECONDARY_FAR_PLANE = 100.0f; // Distancia máxima de visualización

// Constantes de la luz
const glm::vec3 LIGHT_POSITION = glm::vec3(2.0f, 4.0f, 2.0f);
const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

// Colores de los objetos
const glm::vec3 CUBE_COLOR = glm::vec3(1.0f, 0.5f, 0.2f);  // Taronja
const glm::vec3 PYRAMID_COLOR = glm::vec3(0.0f, 1.0f, 0.0f);  // Verde
const glm::vec3 PRISM_COLOR = glm::vec3(0.0f, 0.0f, 1.0f);  // Azul

// Constantes del color de fondo
const glm::vec4 CLEAR_COLOR = glm::vec4(0.15f, 0.15f, 0.18f, 1.0f);  // Gris oscuro

// COnstantes de la linterna.
const float INNER_CUTOFF = glm::cos(glm::radians(35.0f));
const float OUTER_CUTOFF = glm::cos(glm::radians(45.0f));

// Constantes de la cámara
const float CAMERA_HEIGHT = 5.0f;
const float CAMERA_RADIUS = 14.0f;

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

    // Inicializa framebuffer de la camara secundaria
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    unsigned int fboTexture;
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Incompleto!" << std::endl;

    // Conecta el framebuffer a la ventana
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Establece el callback para el cambio de tamano de la ventana.
    glfwSetFramebufferSizeCallback(window, onResize);

    // Crea un quad para el framebuffer.
    // El quad es un cuadrado que se utiliza para dibujar el framebuffer flotante en la ventana.
    float quadVertices[] = {
        0.5f, -0.5f,       0.0f, 1.0f,   // top-left
        1.0f, -0.5f,       1.0f, 1.0f,   // top-right
        1.0f, -1.0f,       1.0f, 0.0f,   // bottom-right

        0.5f, -0.5f,       0.0f, 1.0f,   // top-left
        1.0f, -1.0f,       1.0f, 0.0f,   // bottom-right
        0.5f, -1.0f,       0.0f, 0.0f    // bottom-left
    };

    // Array de vértices para el borde blanco del quad
    float quadBorderVertices[] = {
        0.5f, -0.5f,  1.0f, -0.5f,
        1.0f, -0.5f,  1.0f, -1.0f,
        1.0f, -1.0f,  0.5f, -1.0f,
        0.5f, -1.0f,  0.5f, -0.5f
    };

    // Define el VAO y VBO de la camara secundaria.
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // VAO y VBO para el borde del quad
    unsigned int quadBorderVAO, quadBorderVBO;
    glGenVertexArrays(1, &quadBorderVAO);
    glGenBuffers(1, &quadBorderVBO);
    glBindVertexArray(quadBorderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadBorderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadBorderVertices), quadBorderVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Carga los shaders para el quad, de la camara secundaria.
    std::string screenVert = load("shaders/framebuffer.vert");
    std::string screenFrag = load("shaders/framebuffer.frag");
    unsigned int screenVShader = compile(screenVert.c_str(), GL_VERTEX_SHADER);
    unsigned int screenFShader = compile(screenFrag.c_str(), GL_FRAGMENT_SHADER);
    unsigned int screenShaderProgram = glCreateProgram();
    glAttachShader(screenShaderProgram, screenVShader);
    glAttachShader(screenShaderProgram, screenFShader);
    glLinkProgram(screenShaderProgram);
    glDeleteShader(screenVShader);
    glDeleteShader(screenFShader);

    // Carga los shaders para el borde blanco
    std::string borderVert = load("shaders/border.vert");
    std::string borderFrag = load("shaders/border.frag");
    unsigned int borderVShader = compile(borderVert.c_str(), GL_VERTEX_SHADER);
    unsigned int borderFShader = compile(borderFrag.c_str(), GL_FRAGMENT_SHADER);
    unsigned int borderShaderProgram = glCreateProgram();
    glAttachShader(borderShaderProgram, borderVShader);
    glAttachShader(borderShaderProgram, borderFShader);
    glLinkProgram(borderShaderProgram);
    glDeleteShader(borderVShader);
    glDeleteShader(borderFShader);

    // Define el VAO y VBO de la camara primaria
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE), CUBE, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Carga los shaders para los objetos, de la camara primaria.
    std::string vertexCode = load("shaders/phong-blinn.vert");
    std::string fragmentCode = load("shaders/phong-blinn.frag");
    unsigned int vertexShader = compile(vertexCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compile(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Bucle principal de renderizado, mientras no se cierra la ventana.
    while (!glfwWindowShouldClose(window)) {

        // Maneja la entrada del teclado.
        onKeyPress(window);

        // Obtiene el tiempo actual utiizando GLFW
        float time = glfwGetTime();

        // Obtiene el tamaño real del framebuffer
        int fbw, fbh;
        glfwGetFramebufferSize(window, &fbw, &fbh);

        // Define los 3 modelos a dibujar.
        ShapeInfo shapes[3] = {
            {CUBE, sizeof(CUBE)/(6*sizeof(float)), glm::vec3(0.0f, 0.0f, 0.0f), CUBE_COLOR},
            {PYRAMID, sizeof(PYRAMID)/(6*sizeof(float)), glm::vec3(2.0f, 0.0f, 0.0f), PYRAMID_COLOR},
            {PRISM, sizeof(PRISM)/(6*sizeof(float)), glm::vec3(-2.0f, 0.0f, 0.0f), PRISM_COLOR}
        };

        // Renderiza los objetos con la camara secundaria, en el framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fbw, fbh);
        glEnable(GL_DEPTH_TEST);
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calcula la matriz de vista y proyección de la camara secundaria
        glUseProgram(shaderProgram);
        glm::mat4 secView = glm::lookAt(SECONDARY_CAMERA_POS, SECONDARY_CAMERA_TARGET, SECONDARY_CAMERA_UP);
        glm::mat4 secProj = glm::perspective(glm::radians(SECONDARY_FOV), (float)fbw / (float)fbh, SECONDARY_NEAR_PLANE, SECONDARY_FAR_PLANE);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(secView));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(secProj));

        // Renderiza los objetos con la camara secundaria, en el framebuffer
        for (int i = 0; i < 3; ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, shapes[i].count * 6 * sizeof(float), shapes[i].data, GL_STATIC_DRAW);
            float phase = i * 1.5f;
            float speed = 1.0f + 0.5f * i;
            float jump = 0.5f * fabs(sin(time * speed + phase));
            glm::mat4 model = glm::translate(glm::mat4(1.0f), shapes[i].pos + glm::vec3(0.0f, jump, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(glm::vec3(0.0f)));
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, glm::value_ptr(glm::normalize(glm::vec3(secView * glm::vec4(SECONDARY_CAMERA_TARGET - SECONDARY_CAMERA_POS, 0.0)))));
            glUniform1f(glGetUniformLocation(shaderProgram, "innerCutoff"), INNER_CUTOFF);
            glUniform1f(glGetUniformLocation(shaderProgram, "outerCutoff"), OUTER_CUTOFF);
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(LIGHT_COLOR));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(shapes[i].color));
            glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(SECONDARY_CAMERA_POS));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, shapes[i].count);
        }

        // Renderiza los objetos con la camara primaria
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, fbw, fbh);
        glEnable(GL_DEPTH_TEST);
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calcula la matriz de vista y proyección de la camara primaria
        glUseProgram(shaderProgram);
        glm::mat4 primView = glm::lookAt(PRIMARY_CAMERA_POS, PRIMARY_CAMERA_TARGET, PRIMARY_CAMERA_UP);
        glm::mat4 primProj = glm::perspective(glm::radians(PRIMARY_FOV), (float)fbw / (float)fbh, PRIMARY_NEAR_PLANE, PRIMARY_FAR_PLANE);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(primView));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(primProj));

        // Renderiza los objetos con la camara primaria
        for (int i = 0; i < 3; ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, shapes[i].count * 6 * sizeof(float), shapes[i].data, GL_STATIC_DRAW);
            float phase = i * 1.5f;
            float speed = 1.0f + 0.5f * i;
            float jump = 0.5f * fabs(sin(time * speed + phase));
            glm::mat4 model = glm::translate(glm::mat4(1.0f), shapes[i].pos + glm::vec3(0.0f, jump, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(glm::vec3(0.0f)));
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, glm::value_ptr(glm::normalize(glm::vec3(primView * glm::vec4(PRIMARY_CAMERA_TARGET - PRIMARY_CAMERA_POS, 0.0)))));
            glUniform1f(glGetUniformLocation(shaderProgram, "innerCutoff"), INNER_CUTOFF);
            glUniform1f(glGetUniformLocation(shaderProgram, "outerCutoff"), OUTER_CUTOFF);
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(LIGHT_COLOR));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(shapes[i].color));
            glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(PRIMARY_CAMERA_POS));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, shapes[i].count);
        }

        // Dibuja el quad con la textura del framebuffer
        glViewport(0, 0, fbw, fbh);
        glUseProgram(screenShaderProgram);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Dibuja el borde blanco alrededor del quad
        glUseProgram(borderShaderProgram);
        glBindVertexArray(quadBorderVAO);
        glDrawArrays(GL_LINES, 0, 8);

        // Intercambia los buffers y actualiza la ventana
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
