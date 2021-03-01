#version 430

layout(location=0) in vec3 vertPos;

layout (binding=0) uniform samplerCube samp;

out vec3 varyingTexCoord;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;

void main(){
	varyingTexCoord = vertPos;
	mat4 vrot_matrix = mat4(mat3(v_matrix)); //removes translation from view matrix

	gl_Position = proj_matrix*vrot_matrix*vec4(vertPos, 1.0);
}