#version 410 core

vec3 getDirectionalLight(vec3, vec3);
vec3 getPointLight(vec3, vec3);

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;

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

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;

float specularFactor;

uniform bool useBlinn;
uniform bool useSpot;
uniform bool usePoint;
uniform bool useRim;

void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 result = vec3(0.0);
    result = getDirectionalLight(norm, viewDir);

    if(usePoint)
    {
        result = result + getPointLight(norm, viewDir); 
    }
    FragColor = vec4(result,1.0f);


}

vec3 getDirectionalLight(vec3 norm, vec3 viewDir)
{
    //ambient
   vec3 ambientColour = lightCol * objectCol * ambientFactor;
   //diffuse
   float diffuseFactor = dot(norm, -lightDir);
   diffuseFactor = max(diffuseFactor,0.0);
   vec3 diffuseColour = lightCol * objectCol * diffuseFactor;
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
   vec3 specularColour = lightCol * specularFactor * specularStrength;

   vec3 result = ambientColour + diffuseColour + specularColour; 
   return result;
}

vec3 getPointLight(vec3 norm, vec3 viewDir)
{
    //point light stuff
    float dist = length(pLight.position - posWS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));
    vec3 pLightDir = normalize(pLight.position - posWS);

    //ambient
    vec3 ambientColour = lightCol*objectCol*ambientFactor;
    ambientColour = ambientColour * attn;

    //diffuse
    float diffuseFactor = dot(norm, pLightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = lightCol * objectCol * diffuseFactor;
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
    vec3 specularColour = lightCol * specularFactor * specularStrength;
    specularColour = specularColour * attn;
    vec3 pointLightResult = ambientColour + diffuseColour + specularColour;
    return pointLightResult;
}