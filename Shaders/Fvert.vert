#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTan;
layout (location = 3) in vec3 aBiTan;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eyePos;
uniform int numOfpLights;




out vec2 uv;
//out vec3 tPointLightPositions[MAX_LIGHTS];
out vec3 tLightPosition;
out mat3 TBN;
out vec3 posWS;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);
	posWS = vec3(model*vec4(aPos,1.0));
	vec3 normal = vec3(model*vec4(aNormals,1.0));
	vec3 T = vec3(model * vec4(aTan,0.0));
	vec3 B = vec3(model * vec4(aBiTan,0.0));

	TBN = mat3(T, B, normal);



	uv = aUV;
//	for (int i = 0; i < numOfpLights; i ++)
//	{
//		tPointLightPositions[i] = TBN * pointLightPositions[i];
//	}
}