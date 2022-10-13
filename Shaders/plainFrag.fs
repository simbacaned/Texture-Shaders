#version 410 core

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 objectCol;
uniform vec3 cameraPos;

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;

void main()
{    
    vec3 norm = normalize(normal);
    //ambient
    vec3 ambientColour = lightCol * objectCol * ambientFactor;
    //diffuse
    float diffuseFactor = dot(norm, -lightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = lightCol * objectCol * diffuseFactor;
    //specular
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 reflectDir = reflect(lightDir, norm);
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    vec3 specularColour = lightCol * specularFactor * specularStrength;

    vec3 result = ambientColour + diffuseColour + specularColour;
    FragColor = vec4(result,1.0f);
}

