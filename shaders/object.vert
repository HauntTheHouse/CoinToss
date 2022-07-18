#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;

layout(location = 0) out vec3 fFragPos;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec2 fTextureCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vec4 modelSpasePos = uModel * vec4(aPosition, 1.0f);
	gl_Position = uProjection * uView * modelSpasePos;

	fFragPos = vec3(modelSpasePos);

	fNormal = mat3(transpose(inverse(uModel))) * aNormal;
	fTextureCoord = aTextureCoord;
}
