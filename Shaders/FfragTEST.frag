#version 410 core

struct Material
{
    vec3 ambient;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D normalMap;
    sampler2D heightMap;
    float shininess;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct pointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float Kc;
    float Kl;
    float Ke;
};

layout (location = 1) out vec4 brightColor;

vec3 getDirectionalLight(vec3 normal, vec3 viewDir, vec3 lightDir);
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal, vec3 viewDir);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir);
float getRimLight(vec3 norm, vec3 viewDir);

out vec4 FragColor;

in vec3 posWS;
#define MAX_LIGHTS 8
in vec2 uv;
in mat3 TBN;

uniform Material mat;

uniform vec3 lightPosition;
uniform vec3 eyePos;
uniform Light light;
uniform int numOfpLights;
uniform pointLight pLights[MAX_LIGHTS];
#define MAX_LIGHTS 8
uniform vec3 pointLightPositions[MAX_LIGHTS];

vec3 diffMapCol;
vec3 specMapCol;

uniform bool useBlinn = true;
uniform bool useSpot = true;
uniform bool usePoint = true;
uniform bool useNorm = false;
uniform bool useRim = true;

void main()
{    
    //pxmapping
    vec3 viewDir = normalize(eyePos - posWS);
   // vec2 texCoords = ParallaxMapping(uv, viewDir);

    diffMapCol = texture(mat.diffuseTexture, uv).xyz;
    specMapCol = texture(mat.specularTexture, uv).xyz;

    vec3 normalMap = texture(mat.normalMap, uv).rgb * 2.0 - 1.0;

    if(useNorm)
    {
        diffMapCol = normalMap;
        specMapCol = normalMap;
    }

    vec3 normal = normalize(TBN * normalMap);
    vec3 result = vec3(0.0);
    vec3 lightDir = normalize(lightPosition - posWS);

    result = result + getDirectionalLight(normal, viewDir, lightDir);
    if(usePoint)
    {
        for (int i = 0; i < numOfpLights; i ++)
        {
            result = result + getPointLight(pointLightPositions[i], pLights[i], normal, viewDir);
        }
    }
    if(useRim)
    {
        result = result + getRimLight(normal, viewDir);
    }
    result = result/(result + vec3(1.0));
    FragColor = vec4(result, 1.0f);

}

vec3 getDirectionalLight(vec3 normal, vec3 viewDir, vec3 lightDir)
{
    //ambient
    vec3 ambient = diffMapCol * mat.ambient * light.ambient;

    //diffuse
    float diffuseFactor = dot(normal, lightPosition);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = light.diffuse * diffMapCol * diffuseFactor;

    //specular
    float specularFactor;
    if(useBlinn)
    {
        
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 8.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, mat.shininess);
    vec3 specularColour = light.specular * specularFactor * specMapCol;

    vec3 result = ambient + diffuseColour + specularColour; 
    return result;
}

//-----PointLights-----//
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal, vec3 viewDir)
{
    vec3 diffuseTexture = texture(mat.diffuseTexture, uv).rgb;
    // Ambient lighting
    vec3 ambient = pLight.ambient * mat.ambient * diffuseTexture;

    // Diffuse lighting
    vec3 lightDir = normalize(position - posWS);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pLight.diffuse * diff  * diffuseTexture;

    // Specular lighting
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
//    vec3 specular = pLight.specular * spec * specMapCol;
    //specular
    float specularFactor;
    if(useBlinn)
    {
        
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    // Attenuation
    float distance = length(position - posWS);
    float attenuation = 1.0 / (pLight.Kc + pLight.Kl * distance + pLight.Ke * distance * distance);

    // Add all the components together
    return attenuation * (ambient + diffuse * specularFactor);
} 

//Rim Light
float getRimLight(vec3 norm, vec3 viewDir)
{
    float S = 2.0f;
    float B = 0.2f;
    float bLight = B * (1.0f - dot(norm, viewDir));
    bLight = pow(bLight, S);
    return bLight;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir)
{
    float height = texture(mat.heightMap, texCoords).r;
    return texCoords - (viewdir.xy / viewdir.z * height * 0.000000001);
};