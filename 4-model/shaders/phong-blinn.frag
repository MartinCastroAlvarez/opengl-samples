#version 330 core
out vec4 FragColor;      // Color de salida del fragmento

in vec3 FragPos;         // Posición del fragmento en espacio de vista
in vec3 Normal;          // Normal en espacio de vista
in vec2 TexCoord;        // Coordenadas de textura del fragmento

uniform vec3 lightPos;   // Posición de la luz en espacio de vista (cámara)
uniform vec3 viewPos;    // Posición de la cámara en espacio de vista
uniform vec3 lightColor; // Color de la luz
uniform sampler2D diffuseMap; // Textura de difuso del objeto
uniform sampler2D specularMap;
uniform vec3 dirLightDirection;
uniform vec3 dirLightColor;
uniform sampler2D normalMap;

void main()
{
    vec3 ambient = 0.3 * lightColor;

    // Cargar normal desde textura en espacio tangente (aproximado sin matriz TBN)
    vec3 tangentNormal = texture(normalMap, TexCoord).rgb;
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
    vec3 norm = normalize(tangentNormal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;

    // Luz direccional
    vec3 dirLightDir = normalize(-dirLightDirection);
    float dirDiff = max(dot(norm, dirLightDir), 0.0);
    vec3 dirDiffuse = dirDiff * dirLightColor;
    vec3 dirHalfway = normalize(dirLightDir + viewDir);
    float dirSpec = pow(max(dot(norm, dirHalfway), 0.0), 32);
    vec3 dirSpecular = 0.5 * dirSpec * dirLightColor;

    // Texturas
    vec3 texColor = texture(diffuseMap, TexCoord).rgb;
    vec3 specColor = texture(specularMap, TexCoord).rgb;
    specular *= specColor;
    dirSpecular *= specColor;

    vec3 result = (ambient + diffuse + specular + dirDiffuse + dirSpecular) * texColor;
    FragColor = vec4(result, 1.0);
} 