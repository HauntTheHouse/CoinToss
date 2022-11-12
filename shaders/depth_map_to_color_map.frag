#version 330 core

out vec4 FragColor;
in vec2 fTexCoords;

uniform sampler2D uDepthMap;

void main()
{
    float depthValue = texture(uDepthMap, fTexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0f);
}
