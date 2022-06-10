#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 fColor;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);
	fColor = aColor;
}
