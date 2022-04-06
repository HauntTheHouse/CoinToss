#version 330 core

layout(location = 0) in vec3 fNormal;
layout(location = 1) in vec2 fTextureCoord;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
