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

struct pointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float Kc;
    float Kl;
    float Ke;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 1) out vec4 brightColor;

vec3 getDirectionalLight(vec3 normal, vec3 lightDir, vec3 viewDir, float shadow);
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal, vec3 viewDir, float shadow);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir);
float calcShadow(vec4 fragPosLightSpace);

out vec4 FragColor;

in vec3 tEyePos;
in vec3 posTS;
in vec3 tLightPosition;
#define MAX_LIGHTS 8
in vec3 tPointLightPositions[MAX_LIGHTS];
in vec2 uv;

uniform Light light;
uniform Material mat;
uniform float bloomBrightness;

//for shadow mapping
uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;



uniform bool useBlinn = true;
uniform bool useSpot = true;
uniform bool usePoint = true;
uniform bool useRim = true;
uniform bool useNorm = true;

uniform int numOfpLights;
uniform pointLight pLights[MAX_LIGHTS];

vec3 diffMapCol;
vec3 specMapCol;

void main()
{    
    //pxmapping
    vec3 TviewDir = normalize(tEyePos - posTS);
    vec2 texCoords = ParallaxMapping(uv, TviewDir);

    diffMapCol = texture(mat.diffuseTexture, texCoords).xyz;
    specMapCol = texture(mat.specularTexture, texCoords).xyz;

    vec3 normal = normalize(texture(mat.normalMap, texCoords).rgb * 2.0 - 1.0);
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(posTS, 1.0);
    float shadow = calcShadow(fragPosLightSpace);
    vec3 result = vec3(0.0);
    vec3 lightDir = normalize(-tLightPosition);
    normal = normal * 2.0 - 1.0;
    normal = normalize(normal);

    //    FragColor = vec4(normal,1.0);
    //    return;
    //result = result + getDirectionalLight(normal, lightDir, TviewDir, shadow);
    if(usePoint)
    {
        result = result + getPointLight(tPointLightPositions[0], pLights[0], normal, TviewDir, shadow);
        for (int i = 0; i < numOfpLights; i++)
        {
            //result = result + getPointLight(tPointLightPositions[i], pLights[i], normal, TviewDir, shadow);
        }
    }
    float brightness = max(max(result.x, result.y), result.z);
    if(brightness > bloomBrightness)
    {
        brightColor = FragColor;
    }
    else
    {
        brightColor = vec4(vec3(0.0), 1.0);
    }
    result = result/(result + vec3(1.0));
    FragColor = vec4(result, 1.0f);
    //FragColor = vec4(texture(mat.diffuseTexture, uv).xyz, 1.0);
}

vec3 getDirectionalLight(vec3 normal, vec3 lightDir, vec3 viewDir, float shadow)
{
    //ambient
    vec3 ambient = diffMapCol * mat.ambient * light.ambient;

    //diffuse
    float diffuseFactor = dot(normal, lightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = light.diffuse * diffMapCol * 0.1;

    //specular
    float specularFactor;
    if(useBlinn)
    {
        
        vec3 halfwayDir = normalize(tLightPosition + viewDir);
        specularFactor =  dot(normal, halfwayDir);
    }
    else
    {
        vec3 reflectDir = reflect(tLightPosition, normal);
        specularFactor = dot(viewDir, reflectDir);
    }
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, mat.shininess);
    vec3 specularColour = light.specular * specularFactor * specMapCol.r;

    vec3 result = ambient + diffuseColour + specularColour; 
    return result;
}

//point light 
vec3 getPointLight(vec3 position, pointLight pLight, vec3 normal, vec3 viewDir, float shadow)
{
    float dist = length(position - posTS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));

    //ambient
    vec3 ambient = diffMapCol * mat.ambient * pLight.ambient;

    //diffuse
    vec3 pLightDir = normalize(position - posTS);
    float diffuseFactor = max(dot(normal, posTS),0.0);
    vec3 diffuseColour = diffMapCol * diffuseFactor * pLight.diffuse ;
    diffuseColour = diffMapCol + attn; 

    //specular
    {
//    float specularFactor;
//    if(useBlinn)
//    {
//        vec3 halfwayDir = normalize(pLightDir + viewDir);
//        specularFactor =  dot(normal, halfwayDir);
//    }
//    else
//    {
//        vec3 reflectDir = reflect(-pLightDir, normal);
//        specularFactor = dot(viewDir, reflectDir);
//    }
//    specularFactor = max(specularFactor, 0.0);
//    specularFactor = pow(specularFactor, mat.shininess);
//    float specularStrength = 0.2;
//    vec3 specularColour = pLight.specular * specularFactor * specularStrength * specMapCol.r;
//    specularColour = specularColour * attn;
}
    vec3 pointLightResult =  diffuseColour ;
    return pointLightResult;
} 
//+ (1.0 - shadow) * (diffuseColour + specularColour)

vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir)
{
    float height = texture(mat.heightMap, texCoords).r;
    return texCoords - (viewdir.xy / viewdir.z * height * 0.000000001);
};

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