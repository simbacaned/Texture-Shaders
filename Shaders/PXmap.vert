
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 posWS;
out vec2 uv;
out mat3 TBN;

void main()
{  
	posWS = (model*vec4(aPos,1.0)).xyz;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	normal = (model * vec4(aNormals, 0.0)).xyz; 
	vec3 T = (model * vec4(tangent, 0.0)).xyz; 
	vec3 B = (model * vec4(bitangent, 0.0)).xyz; 
	uv = texCoords;
	TBN = mat3(T, B, normal);
}