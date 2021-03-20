#version 430

in vec3 originalVertex;

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
	vec3 L = normalize(varyingLightDir);

	vec3 N;
	
	float a = 0.25;
	float b = 100.0;
	
	float x = originalVertex.x;
	float y = originalVertex.y;
	float z = originalVertex.z;

	N.x = varyingNormal.x + a * sin(b*x);
	N.y = varyingNormal.y + a * sin(b*y);
	N.z = varyingNormal.z + a * sin(b*z);
	//N.x = a * sin(b*x);
	//N.y = a * sin(b*y);
	//N.z = a * sin(b*z);
	N = normalize(N);
	
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