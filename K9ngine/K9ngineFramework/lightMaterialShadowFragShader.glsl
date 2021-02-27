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

uniform float shadowWidth;
uniform vec2 windowSize;
uniform bool betterPerformance; //uses dithering insted of sampling lots of texels

float lookup(float ox, float oy){
	return textureProj(shadowTexture, shadowCoord + vec4(ox * 1.0/windowSize.x * shadowCoord.w,
														 oy * 1.0/windowSize.y * shadowCoord.w,
														 -0.01, 0.0));
}

void main(){
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	//vec3 R = normalize(reflect(-L, N));
	vec3 H = normalize(varyingHalfVector);
	
	float shadowFactor = 0.0;
	
	if(betterPerformance){
		vec2 offset = mod(floor(gl_FragCoord.xy), 2.0) * shadowWidth;
		shadowFactor += lookup(-1.5 * shadowWidth + offset.x, 1.5 * shadowWidth - offset.y);
		shadowFactor += lookup(-1.5 * shadowWidth + offset.x, -0.5 * shadowWidth - offset.y);
		shadowFactor += lookup(0.5 * shadowWidth + offset.x, 1.5 * shadowWidth - offset.y);
		shadowFactor += lookup(0.5 * shadowWidth + offset.x, -0.5 * shadowWidth - offset.y);
		shadowFactor = shadowFactor / 4.0;
	}
	else{
		float endp = shadowWidth * 3.0 + shadowWidth / 2.0;
		for(float m = -endp; m <= endp; m=m+shadowWidth){
			for(float n = -endp; n <= endp; n=n+shadowWidth){
				shadowFactor += lookup(m,n);
			}
		}

		shadowFactor = shadowFactor / 64.0;
	}

	vec4 shadowColor = globalAmbient*material.ambient + light0.ambient*material.ambient;
	
	float cosTheta = dot(L, N);
	vec4 diffuse = light0.diffuse * material.diffuse * max(0.0f, cosTheta);
	
	float cosPhi = dot(H, N);
	//vec4 specular = light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess);
	//vec4 specular = light0.specular * material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);
	//vec4 specular = light0.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);
	vec4 specular = light0.specular*material.specular * pow(max(0.0f, cosPhi), material.shininess*3.0);

	vec4 lightedColor = diffuse + specular;

	fragColor = vec4((shadowColor.xyz + shadowFactor*(lightedColor.xyz)), 1.0);
}