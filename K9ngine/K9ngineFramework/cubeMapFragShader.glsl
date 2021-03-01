#version 430

layout (binding=0) uniform samplerCube samp;

in vec3 varyingTexCoord;

out vec4 fragColor;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;

void main(){
	fragColor = texture(samp, varyingTexCoord);
}