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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Constantes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "Horus";

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
const glm::vec3 LIGHT_POSITION = glm::vec3(0.0f, 2.0f, 2.0f);  // Luz más arriba y adelante
const glm::vec3 LIGHT_COLOR = glm::vec3(0.7f, 0.7f, 0.7f);

// Constantes del color de fondo
const glm::vec4 CLEAR_COLOR = glm::vec4(0.15f, 0.15f, 0.18f, 1.0f);  // Gris oscuro

// Constantes de la cámara
const float CAMERA_HEIGHT = 2.0f;
const float CAMERA_RADIUS = 4.0f;

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

// Estructura para almacenar los datos de una malla
struct Mesh {
    unsigned int VAO, VBO;
    unsigned int numVertices;
};

// Función para procesar una malla de Assimp y crear buffers de OpenGL
Mesh processMesh(aiMesh* mesh) {
    std::vector<float> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Posición
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // Normal
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
        // UV
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }
    // Indices (solo para triángulos)
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    // Crear VAO/VBO/EBO
    Mesh result;
    glGenVertexArrays(1, &result.VAO);
    glGenBuffers(1, &result.VBO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindVertexArray(result.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    // Atributos: pos(3), normal(3), uv(2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    result.numVertices = indices.size();
    return result;
}

// Función para recorrer el nodo y procesar todas las mallas
void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, meshes);
    }
}

// Función para cargar texturas
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Filtros
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
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

    // Cargar el modelo con Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("../horus/Horus.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return -1;
    }
    std::vector<Mesh> meshes;
    processNode(scene->mRootNode, scene, meshes);

    // Cargar texturas
    std::cout << "Loading texture from: ../horus/Textures/Horus_Diffuse.png" << std::endl;
    unsigned int diffuseMap = loadTexture("../horus/Textures/Horus_Diffuse.png");
    if (diffuseMap == 0) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }
    std::cout << "Texture loaded successfully with ID: " << diffuseMap << std::endl;

    // Cargar texturas
    std::cout << "Loading specular map from: ../horus/Textures/Horus_Specular.png" << std::endl;
    unsigned int specularMap = loadTexture("../horus/Textures/Horus_Specular.png");
    if (specularMap == 0) {
        std::cerr << "Failed to load specular texture!" << std::endl;
        return -1;
    }
    std::cout << "Specular map loaded successfully with ID: " << specularMap << std::endl;

    // Cargar texturas
    std::cout << "Loading normal map from: ../horus/Textures/Horus_Normal.png" << std::endl;
    unsigned int normalMap = loadTexture("../horus/Textures/Horus_Normal.png");
    if (normalMap == 0) {
        std::cerr << "Failed to load normal texture!" << std::endl;
        return -1;
    }
    std::cout << "Normal map loaded successfully with ID: " << normalMap << std::endl;

    // Activar y vincular las texturas
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "specularMap"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 2);

    // Bucle principal de renderizado, mientras no se cierra la ventana.
    while (!glfwWindowShouldClose(window)) {

        // Maneja la entrada del teclado.
        onKeyPress(window);

        // Limpia el buffer de color y el buffer de profundidad.
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usa el programa de shader.
        glUseProgram(shaderProgram);

        // Cámara girando alrededor del centro
        float time = glfwGetTime();
        glm::vec3 cameraPos = glm::vec3(CAMERA_RADIUS * sin(time), CAMERA_HEIGHT, CAMERA_RADIUS * cos(time));
        glm::mat4 view = glm::lookAt(cameraPos, CAMERA_TARGET, CAMERA_UP);
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);

        // Aplicar la proyección.
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Aplicar la vista.
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Configurar la matriz de modelo y escalar el modelo
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));  // Centrar el modelo, porque no está centrado en model space.
        model = glm::scale(model, glm::vec3(0.008f));  // Achicar el modelo para que quepa en la pantalla
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Configurar la luz
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(LIGHT_POSITION));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(LIGHT_COLOR));

        // Luz direccional
        glm::vec3 dirLightDir = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));
        glm::vec3 dirLightColor = glm::vec3(0.25f, 0.25f, 0.25f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightDirection"), 1, glm::value_ptr(dirLightDir));
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightColor"), 1, glm::value_ptr(dirLightColor));

        // Asegurarse de que estamos en modo sólido
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Activar y vincular las texturas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalMap);

        // Dibuja cada una de las mallas del modelo
        for (const auto& mesh : meshes) {
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.numVertices, GL_UNSIGNED_INT, 0);
        }

        // Intercambia los buffers de color y profundidad.
        glfwSwapBuffers(window);

        // Espera a que ocurra un evento.
        glfwPollEvents();
    }

    // Limpia los recursos de la memoria.
    glDeleteProgram(shaderProgram);

    // Termina GLFW.
    glfwTerminate();
    return 0;
}
