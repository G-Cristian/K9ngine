#version 430

layout (binding=0) uniform sampler2DShadow shadowTexture;

in vec4 shadowCoord;

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;

out vec4 fragColor;

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

void main(){
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	//vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(varyingHalfVector);
	
	float notInShadow = textureProj(shadowTexture, shadowCoord);

	fragColor = globalAmbient*material.ambient + light0.ambient*material.ambient;
	
	if(notInShadow != 0.0){
		float cosTheta = dot(L, N);
		//float cosPhi = dot(V, R);
		float cosPhi = dot(H, N);

		vec4 diffuse = light0.diffuse * material.diffuse * max(0.0f, cosTheta);
		//vec4 specular = light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess);
		//vec4 specular = light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);
		//vec4 specular = light0.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);
		vec4 specular = light0.specular*material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);

		fragColor += diffuse + specular;
	}
}