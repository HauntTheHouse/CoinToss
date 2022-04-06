#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;

layout(location = 0) out vec3 fNormal;
layout(location = 1) out vec2 fTextureCoord;

void main()
{
	gl_Position = vec4(aPosition, 1.0f);
	fNormal = aNormal;
	fTextureCoord = aTextureCoord;
}
