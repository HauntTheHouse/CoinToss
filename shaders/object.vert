#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 fFragPos;
out vec2 fTextureCoord;
out mat3 fTBN;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vec4 modelSpasePos = uModel * vec4(aPosition, 1.0f);
	gl_Position = uProjection * uView * modelSpasePos;

	fFragPos = vec3(modelSpasePos);
	fTextureCoord = aTextureCoord;

	// apply mat3(transpose(inverse(uModel))) when using non-uniform scaling
	vec3 T = normalize(vec3(uModel * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(uModel * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(uModel * vec4(aNormal,    0.0)));
    fTBN = mat3(T, B, N);
}
