#version 430

in vec2 varyingTexCoord;

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
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light0;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout (binding=0) uniform sampler2D samp;

void main(){
	vec3 texColor = (texture(samp, varyingTexCoord)).xyz;

	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	//vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(varyingHalfVector);
	
	float cosTheta = dot(L, N);
	//float cosPhi = dot(V, R);
	float cosPhi = dot(H, N);

	vec3 ambient = (globalAmbient*texColor*0.3 + light0.ambient*texColor*0.3).xyz;
	vec3 diffuse = (light0.diffuse * texColor * max(0.0f, cosTheta)).xyz;
	//vec3 specular = (light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess)).xyz;
	//vec3 specular = (light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0)).xyz;
	//vec3 specular = (light0.specular * pow(max(0.0f, cosPhi), material.shininess*3.0)).xyz;
	vec3 specular = (light0.specular*texColor * pow(max(0.0f, cosPhi), material.shininess*3.0)).xyz;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}