#version 410 core

out vec4 FragColor;
uniform sampler2D image;
in vec2 uv;
uniform bool useGray = false;

void main()
{
    vec3 result = texture(image, uv).rgb;
    result = result / (result + vec3(1.0f));
    if(useGray)
    {
        FragColor = vec4(vec3(1 - (result.x + result.y + result.z))/3,1.0f);
    }
    else
    {
        FragColor = vec4(result, 1.0);
    }
}
