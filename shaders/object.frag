#version 330 core

layout(location = 0) in vec3 fNormal;
layout(location = 1) in vec2 fTextureCoord;

out vec4 FragColor;

uniform sampler2D uTextureDiffuse;

void main()
{
	FragColor = texture(uTextureDiffuse, fTextureCoord);
}
