#version 330 core

layout(location = 0) in vec3 fFragPos;
layout(location = 1) in vec3 fNormal;
layout(location = 2) in vec2 fTextureCoord;

out vec4 FragColor;

uniform sampler2D uTextureDiffuse;
uniform sampler2D uTextureSpecular;

uniform vec3 uCameraPosition;

uniform vec3 uLightColor;
uniform vec3 uLightDir;

uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uSpecularExponent;

void main()
{
	vec3 fragColor = vec3(texture(uTextureDiffuse, fTextureCoord));
	vec3 specColor = vec3(texture(uTextureSpecular, fTextureCoord));

	vec3 normal = normalize(fNormal);

	vec3 ambient = uLightColor * uAmbientColor;

	float diffuseSignificance = max(dot(normal, -uLightDir), 0.0f);
	vec3 diffuse = uLightColor * diffuseSignificance * uDiffuseColor;

	vec3 reflectDir = reflect(uLightDir, normal);
	vec3 cameraDir = normalize(uCameraPosition - fFragPos);
	float specularSignificance = pow(max(dot(reflectDir, cameraDir), 0.0f), uSpecularExponent);
	vec3 specular = uLightColor * specularSignificance * uSpecularColor;

	vec3 resultColor = fragColor * (ambient + diffuse + specular);
	FragColor = vec4(resultColor, 1.0f);
}
