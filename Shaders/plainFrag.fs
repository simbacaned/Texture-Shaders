#version 410 core

vec3 getDirectionalLight(vec3, vec3);

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

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 objectCol;
uniform vec3 cameraPos;
uniform pointLight pLight;

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;

void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 result = vec3(0.0);
    result = getDirectionalLight(norm, viewDir);
    //point light stuff
    float dist = (pLight.position - posWS);
    float attn = 1.0/(pLight.Kc + (pLight.Kl*dist) + (pLight.Ke*(dist*dist)));
    vec3 pLightDir = normalize(pLight.position - posWS);
    //ambient
    ambientColour = lightCol*objectCol*ambientFactor;
    ambientColour = ambientColour * attn;
    //diffuse
    float diffuseFactor = dot(norm, lightDir);
    diffuseFactor = max(diffuseFactor,0.0);
    vec3 diffuseColour = lightCol * objectCol * diffuseFactor;
    diffuseColour = diffuseColour * attn; 
    //specular
    vec3 reflectDir = reflect(lightDir, norm);
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    vec3 specularColour = lightCol * specularFactor * specularStrength;
    specularColour = specularColour * attn;
    spotLightResult = ambientColour + diffuseColour + specularColour;

    result = result + pointLightResult; 
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
   vec3 reflectDir = reflect(lightDir, norm);
   float specularFactor = dot(viewDir, reflectDir);
   specularFactor = max(specularFactor, 0.0);
   specularFactor = pow(specularFactor, shine);
   vec3 specularColour = lightCol * specularFactor * specularStrength;

   vec3 result = ambientColour + diffuseColour + specularColour; 
   return result;
}