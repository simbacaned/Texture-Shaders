#version 410 core

vec3 getDirectionalLight(vec3 norm, vec3 viewDir);
vec3 getPointLight(vec3 norm, vec3 viewDir);
vec3 getSpotLight(vec3 norm, vec3 viewDir);
float getRimLight(vec3 norm, vec3 viewDir);

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;
in vec2 uv;

struct pointLight
{
    vec3 position;
    vec3 colour;
    float Kc;
    float Kl;
    float Ke;
};

struct spotLight
{
    vec3 position;
    vec3 direction;
    vec3 colour;
    float Kc;
    float Kl;
    float Ke;

    float innerRad;
    float outerRad;
};

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 objectCol;
uniform vec3 cameraPos;
uniform pointLight pLight;
uniform spotLight sLight;

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;

float specularFactor;

uniform bool useBlinn = true;
uniform bool useSpot = true;
uniform bool usePoint = true;
uniform bool useRim = true;

uniform sampler2D diffuseTexture;

vec3 ambientColour;
vec3 diffuseColour;
vec3 specularColour;


void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 result = vec3(0.0);
    result =  getDirectionalLight(norm, viewDir);

    if(usePoint)
    {
        result = result + getPointLight(norm, viewDir); 
    }

    if(useSpot)
    {
        result = result + getSpotLight(norm, viewDir);
    }

    if(useRim)
    {
        result = result + getRimLight(norm, viewDir);
    }
    FragColor = vec4(result,1.0f);


}

vec3 getDirectionalLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapCol = texture(diffuseTexture, uv).xyz;
    //ambient
   ambientColour = lightCol * diffMapCol * ambientFactor;
   //diffuse
   float diffuseFactor = dot(norm, -lightDir);
   diffuseFactor = max(diffuseFactor,0.0);
   diffuseColour = lightCol * diffMapCol * diffuseFactor;
   //specular
   if(useBlinn)
   {
       vec3 halfwayDir = normalize(lightDir + viewDir);
       specularFactor =  dot(norm, halfwayDir);
   }
   else
   {
       vec3 reflectDir = reflect(lightDir, norm);
       float specularFactor = dot(viewDir, reflectDir);
   }
   specularFactor = max(specularFactor, 0.0);
   specularFactor = pow(specularFactor, shine);
   specularColour = lightCol * specularFactor * specularStrength;

   vec3 result = ambientColour + diffuseColour + specularColour; 
   return result;
}

vec3 getPointLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapCol = texture(diffuseTexture, uv).xyz;
    //point light stuff
    float dist = length(pLight.position - posWS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));
    vec3 pLightDir = normalize(pLight.position - posWS);

    //ambient
    ambientColour = lightCol*diffMapCol*ambientFactor;
    ambientColour = ambientColour * attn;

    //diffuse
    float diffuseFactor = dot(norm, pLightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    diffuseColour = lightCol * diffMapCol * diffuseFactor;
    diffuseColour = diffuseColour * attn; 

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
    specularFactor = pow(specularFactor, shine);
    specularColour = lightCol * specularFactor * specularStrength;
    specularColour = specularColour * attn;
    vec3 pointLightResult = ambientColour + diffuseColour + specularColour;
    return pointLightResult;
}

vec3 getSpotLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapCol = texture(diffuseTexture, uv).xyz;
    float dist = length(sLight.position -posWS);
    float attn = 1.0/(sLight.Kl*dist) + (sLight.Ke*(dist*dist));
    vec3 sLightDir = normalize(sLight.position - posWS);

    //diffuse
    float diffuseFactor = dot(norm, sLightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    diffuseColour = sLight.colour * objectCol * diffuseFactor;
    diffuseColour = diffMapCol * attn;

    //specular
    vec3 reflectDir = reflect(sLightDir, norm);
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    specularColour = sLight.colour * specularFactor * specularStrength;
    specularColour = diffMapCol*attn;

    float theta = dot(-sLightDir, normalize(sLight.direction));
    float denom = (sLight.innerRad - sLight.outerRad);
    float illum = (theta - sLight.outerRad);
    illum = clamp(illum, 0.0,1.0);

    //diffuseColour = diffuseColour * illum;
    specularColour = specularColour * illum;
    vec3 spotLightResult = diffuseColour + specularColour;
    return spotLightResult;
}

float getRimLight(vec3 norm, vec3 viewDir)
{
    float S = 1.0f;
    float B = 0.2f;
    float bLight = B * (1.0f - dot(norm, viewDir));
    bLight = pow(bLight, S);
    return bLight;
}