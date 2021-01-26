#version 430

layout(location=0) in vec3 vertPos;

uniform mat4 shadowMVP_matrix;

void main(){
	gl_Position = shadowMVP_matrix*vec4(vertPos, 1.0);
}