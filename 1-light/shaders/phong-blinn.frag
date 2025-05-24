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