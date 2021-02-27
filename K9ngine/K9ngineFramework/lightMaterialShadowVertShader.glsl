#version 430

layout(location=0) in vec3 vertPos;
layout(location=1) in vec3 vertNorm;

layout (binding=0) uniform sampler2DShadow shadowTexture;

out vec4 shadowCoord;

out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;

struct PositionalLight{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material{
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
uniform mat4 shadowMVP2_matrix;

uniform float shadowWidth;
uniform vec2 windowSize;
uniform bool betterPerformance; //uses dithering insted of sampling lots of texels

void main(){
	varyingVertPos = (mv_matrix*vec4(vertPos, 1.0)).xyz;
	varyingLightDir = normalize(light0.position - varyingVertPos); //light is in view dimention (done in c++)
	varyingNormal = normalize(norm_matrix*vec4(vertNorm, 1.0)).xyz;
	shadowCoord = shadowMVP2_matrix*vec4(vertPos, 1.0);
	varyingHalfVector = normalize(varyingLightDir + normalize(-varyingVertPos)).xyz;

	gl_Position = proj_matrix*mv_matrix*vec4(vertPos, 1.0);
}