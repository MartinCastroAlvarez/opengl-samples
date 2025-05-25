#version 330 core
out vec4 FragColor;      // Color de salida del fragmento

in vec3 FragPos;         // Posición del fragmento en espacio de vista
in vec3 Normal;          // Normal en espacio de vista

uniform vec3 lightPos;   // Posición de la luz en espacio de vista (cámara)
uniform vec3 lightDir;   // Dirección de la linterna en espacio de vista
uniform float innerCutoff; // Ángulo interno (coseno)
uniform float outerCutoff; // Ángulo externo (coseno)

uniform vec3 viewPos;    // Posición de la cámara en espacio de vista
uniform vec3 lightColor; // Color de la luz
uniform vec3 objectColor;// Color base del objeto

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    // Spotlight: calcular el ángulo entre la dirección de la luz y el vector hacia el fragmento
    float theta = dot(normalize(-lightDirection), normalize(lightDir));
    float epsilon = innerCutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    // Componente difusa
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor * intensity;

    // Componente especular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * intensity;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 