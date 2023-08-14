#version 330 core


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D normalMap;
    sampler2D heightMap;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linears;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linears;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 uv;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[3];
uniform int numPointLights = 3;
uniform SpotLight spotLights[3];
uniform int numSpotLights = 3;
uniform DirectionalLight directionalLights[1];
uniform int numDirectionalLights = 1;

void main()
{
    // Ambient, Diffuse, and Specular lighting
    
    vec3 result = vec3(0.0);

    vec3 diffuseTexture = texture(material.diffuseTexture, uv).rgb;

    //-----PointLights-----//
    for (int i = 0; i < numPointLights; i++) {
        // Ambient lighting
        vec3 ambient = pointLights[i].ambient * material.ambient * diffuseTexture;

        // Diffuse lighting
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = pointLights[i].diffuse * diff * material.diffuse * diffuseTexture;

        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = pointLights[i].specular * spec * material.specular;

        // Attenuation
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linears * distance + pointLights[i].quadratic * distance * distance);

        // Add all the components together
        result += attenuation * (ambient + diffuse + specular);
    }

    //-----SpotLights-----// 
    for (int i = 0; i < numSpotLights; i++) {

        // diffuse 
        vec3 lightDir = normalize(spotLights[i].position - FragPos);
        float cosTheta = dot(lightDir, normalize(Normal));
        cosTheta = clamp(cosTheta, 0.0, 1.0);
        vec3 diffuse = spotLights[i].diffuse * (cosTheta * material.diffuse ) * diffuseTexture;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normalize(Normal));
        float cosPhi = dot(viewDir, reflectDir);
        cosPhi = clamp(cosPhi, 0.0, 1.0);
        vec3 specular = spotLights[i].specular * (pow(cosPhi, material.shininess) * material.specular);

        // attenuation
        float distance = length(spotLights[i].position - FragPos);
        float attenuation = 1.0 / (spotLights[i].constant + spotLights[i].linears * distance + spotLights[i].quadratic * (distance * distance));

        // spotlight
        vec3 lightToFragment = normalize(FragPos - spotLights[i].position);
        float spotEffect = dot(lightToFragment, normalize(spotLights[i].direction));
        float innerCutOff = cos(radians(spotLights[i].cutOff));
        float outerCutOff = cos(radians(spotLights[i].outerCutOff));
        float spotlightEffect = smoothstep(innerCutOff, outerCutOff, spotEffect);

        result += (spotlightEffect * attenuation * (diffuse + specular));
    }
    //-----DirectionalLights-----//
    for (int i = 0; i < numDirectionalLights; i++)
    {
        // Calculate lighting
        vec3 ambient = directionalLights[0].ambient * diffuseTexture;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-directionalLights[0].direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * directionalLights[0].diffuse *diffuseTexture;

        vec3 viewDir = normalize(-FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * directionalLights[0].specular;


        result += (ambient + diffuse + specular);
    }
    material.diffuseTexture;
    FragColor = vec4(result, 1.0);
}