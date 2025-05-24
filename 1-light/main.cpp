#include <GL/glew.h>        // Biblioteca para cargar extensiones de OpenGL
#include <GLFW/glfw3.h>     // Biblioteca para crear ventanas y contexto OpenGL
#include <glm/glm.hpp>      // Biblioteca matemática para gráficos
#include <glm/gtc/matrix_transform.hpp>  // Funciones de transformación
#include <glm/gtc/type_ptr.hpp>         // Conversión de tipos GLM a punteros
#include <iostream>
#include <vector>
#include <string>

// Vertex shader: Se ejecuta por cada vértice
// Transforma las posiciones y normales al espacio de vista
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;      // Posición del vértice
layout (location = 1) in vec3 aNormal;   // Normal del vértice

uniform mat4 model;      // Matriz de modelo
uniform mat4 view;       // Matriz de vista
uniform mat4 projection; // Matriz de proyección

out vec3 FragPos;        // Posición del fragmento en espacio de vista
out vec3 Normal;         // Normal en espacio de vista

void main()
{
    // Transformar la posición al espacio de vista
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    // Transformar la normal al espacio de vista (considerando la matriz normal)
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    // Calcular la posición final del vértice
    gl_Position = projection * vec4(FragPos, 1.0);
}
)";

// Fragment shader: Se ejecuta por cada fragmento (píxel)
// Implementa el modelo de iluminación Phong-Blinn en espacio de vista
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;      // Color de salida del fragmento

in vec3 FragPos;         // Posición del fragmento en espacio de vista
in vec3 Normal;          // Normal en espacio de vista

uniform vec3 lightPos;   // Posición de la luz en espacio de vista
uniform vec3 viewPos;    // Posición de la cámara en espacio de vista
uniform vec3 lightColor; // Color de la luz
uniform vec3 objectColor;// Color base del objeto

void main()
{
    // Componente ambiental: iluminación base constante
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Componente difusa: depende del ángulo entre la normal y la dirección de la luz
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Componente especular (Blinn-Phong): usa el vector medio para mejor calidad
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);  // En espacio de vista, la cámara está en el origen
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combinar todos los componentes y aplicar el color del objeto
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

// Prototipos de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Dimensiones de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configurar GLFW para usar OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Phong-Blinn Lighting in View Space", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Compilar y enlazar shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Crear y enlazar programa de shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Limpiar shaders individuales después de enlazarlos
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Datos de vértices del cubo (posiciones y normales)
    float vertices[] = {
        // posiciones          // normales
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        // ... Añadir más vértices para completar el cubo
    };

    // Crear y configurar VAO y VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar atributos de vértices
    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo de normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Limpiar el buffer de color y profundidad
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activar el programa de shaders
        glUseProgram(shaderProgram);

        // Configurar matrices de vista y proyección
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f),  // Posición de la cámara
            glm::vec3(0.0f, 0.0f, 0.0f),  // Punto al que mira
            glm::vec3(0.0f, 1.0f, 0.0f)   // Vector arriba
        );
        glm::mat4 model = glm::mat4(1.0f);

        // Pasar matrices al shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Configurar propiedades de la luz
        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.2f);

        // Transformar la posición de la luz al espacio de vista
        lightPos = glm::vec3(view * glm::vec4(lightPos, 1.0f));

        // Pasar variables uniformes al shader
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(objectColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 3.0f)));

        // Renderizar el cubo
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Intercambiar buffers y procesar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza: liberar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// Función para procesar entrada del teclado
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Función para manejar el cambio de tamaño de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 