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
    vec3 position;
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

vec3 getDirectionalLight(vec3 normal, vec3 lightDir, vec3 viewDir, float shadow);
vec3 getPointLight(vec3 position, pointLight pLight, vec3 norm, vec3 viewDir, float shadow);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);
float calcShadow(vec4 fragPosLightSpace);

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 brightColor;

uniform float bloomBrightness;
uniform float PXscale;
uniform Light light;
uniform Material mat;

//for shadow mapping
uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;

in vec3 normal;
in vec3 tEyePos;
#define MAX_LIGHTS 8
in vec3 tPointLightPositions[MAX_LIGHTS];
in vec3 posTS;
in vec2 uv;

uniform bool useBlinn = true;
uniform bool useSpot = true;
uniform bool usePoint = true;
uniform bool useRim = true;
uniform bool useNorm = true;

uniform int numOfpLights;
uniform pointLight pLights[MAX_LIGHTS];

void main()
{    
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(posTS, 1.0);
    float shadow = calcShadow(fragPosLightSpace);
    vec3 result = vec3(0.0);
    vec3 lightDir = normalize(-light.position);
    vec3 viewDir = normalize(tEyePos - posTS);
    vec2 texCoords = ParallaxMapping(uv, viewDir);
    vec3 normal = texture(mat.normalMap, uv).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(normal);
//    FragColor = vec4(normal,1.0);
//    return;
    result = result + getDirectionalLight(normal, lightDir, viewDir, shadow);
    if(usePoint)
    {
        for (int i = 0; i < numOfpLights; i++)
        {
            result = result + getPointLight(tPointLightPositions[i], pLights[i], normal, viewDir, shadow);
        }
    }
    FragColor = vec4(result, 1.0f);

    float brightness = max(max(result.x, result.y), result.z);
    if(brightness > bloomBrightness)
    {
        brightColor = FragColor;
    }
    else
    {
        brightColor = vec4(vec3(0.0), 1.0);
    }
}

vec3 getDirectionalLight(vec3 normal, vec3 lightDir, vec3 viewDir, float shadow)
{
    vec2 texCoords = ParallaxMapping(uv, viewDir);
    vec3 ambient = texture(mat.diffuseTexture, uv).rgb * mat.ambient * light.ambient;
    float dif = max(dot(normal, lightDir),0.0);
    vec3 diffuse = dif * texture(mat.diffuseTexture, uv).xyz * light.diffuse;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec =  pow(max(dot(normal, halfwayDir), 0.0), 2.0);
    vec3 specular = light.specular * spec * vec3(texture(mat.specularTexture, uv).r);
    return ambient + diffuse + specular; 
}

vec3 getPointLight(vec3 position, pointLight pLight, vec3 norm, vec3 viewDir, float shadow)
{
     vec3 diffMapCol = texture(mat.diffuseTexture, uv).xyz;
    //point light stuff
    float dist = length(position - posTS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));
    vec3 pLightDir = normalize(position - posTS);

    //ambient
    vec3 ambient = texture(mat.diffuseTexture, uv).rgb * mat.ambient * pLight.ambient;

    //diffuse
    float diffuseFactor = dot(norm, pLightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = diffMapCol * pLight.diffuse * 0.1;
    diffuseColour = diffuseColour * attn; 

    float specularFactor;
    //specular
    if(useBlinn)
    {
        vec3 halfwayDir = normalize(pLightDir + viewDir);
        specularFactor =  dot(norm, halfwayDir);
    }
    else
    {
        vec3 reflectDir = reflect(-pLightDir, norm);
        specularFactor = dot(viewDir, reflectDir);
    }
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, mat.shininess);
    float specularStrength = 0.2;
    vec3 specularColour = pLight.specular * specularFactor * specularStrength * vec3(texture(mat.specularTexture, uv).r);
    specularColour = specularColour * attn;
    vec3 pointLightResult = ambient + diffuseColour ;
    return pointLightResult;
} 
//+ (1.0 - shadow) * (diffuseColour + specularColour)

float calcShadow(vec4 fragPosLightSpace)
{
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    // perform perspective divide values in range (-1,1)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transofrm to (0,1) range
    projCoords = projCoords * 0.5 + 0.5;
    // sample from the shadow map
    float closestDepth = texture(depthMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether the current frag pos is in shadow
    float shadow = 0.0;
    float bias = 0.005;
    for(int i = -3; i < 3; i++)
    {
        for(int j = -3; j < 3; j++)
        {
            float pcf = texture(depthMap, projCoords.xy + vec2(i, j) * texelSize).r;
            if(currentDepth - bias > pcf)
            {
                shadow += 1;
            }
        }
    }
    shadow = shadow/36;
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    return shadow;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir){
    float height = texture(mat.heightMap, texCoords).r;
    return texCoords - (viewdir.xy / viewdir.z * height * 0.00001);
};