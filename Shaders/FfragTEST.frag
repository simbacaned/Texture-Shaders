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

vec3 getDirectionalLight(vec3 normal, vec3 viewDir);
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir);

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

void main()
{    
    //pxmapping
    vec3 viewDir = normalize(eyePos - posWS);
   // vec2 texCoords = ParallaxMapping(uv, viewDir);

    diffMapCol = texture(mat.diffuseTexture, uv).xyz;
    specMapCol = texture(mat.specularTexture, uv).xyz;

    vec3 normal = texture(mat.normalMap, uv).rgb * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 result = vec3(0.0);
    vec3 lightDir = lightPosition;

    result = result + getDirectionalLight(normal, viewDir);
    if(useSpot)
    {
        for (int i = 0; i < numOfpLights; i ++)
        {
            result = result + getPointLight(pointLightPositions[i], pLights[i], normal);
        }
    }
    result = result/(result + vec3(1.0));
    FragColor = vec4(result, 1.0f);
}

vec3 getDirectionalLight(vec3 normal, vec3 viewDir)
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
        
        vec3 halfwayDir = normalize(lightPosition + viewDir);
        specularFactor =  dot(normal, halfwayDir);
    }
    else
    {
        vec3 reflectDir = reflect(lightPosition, normal);
        specularFactor = dot(viewDir, reflectDir);
    }
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, mat.shininess);
    vec3 specularColour = light.specular * specularFactor * specMapCol;

    vec3 result = ambient + diffuseColour; 
    return result;
}

//point light 
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal)
{
    float dist = length(position - posWS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));
    
    //diffuse
    vec3 lightDir = normalize(position - posWS);
    float diffuseFactor = dot(normal, lightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = diffMapCol * pLight.diffuse * attn; 
    diffuseColour *= diffuseFactor;

    vec3 pointLightResult =  diffuseColour ;
    return pointLightResult;
} 

vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir)
{
    float height = texture(mat.heightMap, texCoords).r;
    return texCoords - (viewdir.xy / viewdir.z * height * 0.000000001);
};