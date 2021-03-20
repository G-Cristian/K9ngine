#version 430

layout (binding=0) uniform sampler2D normalMap;

in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec3 originalVertex;
in vec2 varyingTexCoord;
in vec3 varyingHalfVector;

out vec4 fragColor;

struct PositionalLight{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light0;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

vec3 calcNewNormal() {
	vec3 normal = normalize(varyingNormal);
	vec3 tangent = normalize(varyingTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 retrievedNormal = texture(normalMap, varyingTexCoord).xyz;
	retrievedNormal = retrievedNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);

	return newNormal;
}

void main(){
	vec3 L = normalize(varyingLightDir);

	vec3 N = calcNewNormal();
	
	vec3 V = normalize(-varyingVertPos);

	vec3 H = normalize(varyingHalfVector);
	
	float cosTheta = dot(L, N);
	float cosPhi = dot(H, N);



	vec3 ambient = (globalAmbient*material.ambient + light0.ambient*material.ambient).xyz;
	vec3 diffuse = (light0.diffuse * material.diffuse * max(0.0f, cosTheta)).xyz;
	//vec3 specular = (light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess)).xyz;
	vec3 specular = (light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0)).xyz;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}