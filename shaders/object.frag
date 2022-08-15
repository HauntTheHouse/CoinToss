#version 330 core

in vec3 fFragPos;
in vec2 fTextureCoord;
in mat3 fTBN;

out vec4 FragColor;

uniform sampler2D uTextureDiffuse;
uniform sampler2D uTextureSpecular;
uniform sampler2D uTextureNormal;

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

	vec3 normal = vec3(texture(uTextureNormal, fTextureCoord));
	normal = normal * 2.0f - 1.0f;
	normal = normalize(fTBN * normal);

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
