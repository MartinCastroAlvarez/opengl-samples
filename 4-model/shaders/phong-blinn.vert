#version 330 core
layout (location = 0) in vec3 aPos;      // Posición del vértice
layout (location = 1) in vec3 aNormal;   // Normal del vértice
layout (location = 2) in vec2 aTexCoord;  // Coordenadas de textura del vértice

uniform mat4 model;      // Matriz de modelo
uniform mat4 view;       // Matriz de vista
uniform mat4 projection; // Matriz de proyección

out vec3 FragPos;        // Posición del fragmento en espacio de vista
out vec3 Normal;         // Normal en espacio de vista
out vec2 TexCoord;       // Coordenadas de textura del fragmento

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = vec3(view * worldPos);
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPos;
} 