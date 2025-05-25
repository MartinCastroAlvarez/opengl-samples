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

    // Transformar la normal al espacio de vista
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    // Calcular la posición final del vértice
    gl_Position = projection * vec4(FragPos, 1.0);
} 