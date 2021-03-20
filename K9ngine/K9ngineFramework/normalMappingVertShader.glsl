#version 430

layout(location=0) in vec3 vertPos;
layout(location=1) in vec3 vertNorm;
layout(location=2) in vec3 vertTangent;
layout(location=3) in vec2 texCoord;

layout (binding=0) uniform sampler2D normalMap;

out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingNormal;
out vec3 varyingTangent;
out vec3 originalVertex;
out vec2 varyingTexCoord;
out vec3 varyingHalfVector;

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

void main(){
	originalVertex = vertPos;
	varyingTexCoord = texCoord;

	varyingVertPos = (mv_matrix*vec4(vertPos, 1.0)).xyz;
	varyingNormal = normalize(norm_matrix*vec4(vertNorm, 1.0)).xyz;
	varyingTangent = normalize(norm_matrix*vec4(vertTangent, 1.0)).xyz;
	varyingLightDir = normalize(light0.position - varyingVertPos); //light is in view dimention (done in c++)
	varyingHalfVector = normalize(varyingLightDir + normalize(-varyingVertPos)).xyz;

	gl_Position = proj_matrix*mv_matrix*vec4(vertPos, 1.0);
}