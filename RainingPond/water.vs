#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec4 ScreenTexCoord;
out vec3 Normal;
out vec3 WorldPos;

uniform sampler2D HeightMap;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 ComputeScreenPos(vec4 pos, float flip){
	vec4 o = pos * 0.5;
	o.xy = vec2(o.x, o.y*1.0) + o.w;
	o.zw = pos.zw;
	return o;
}
vec4 ComputeScreenPosXZ(vec4 pos, float flip){
	vec4 o = pos * 0.5;
	o.xz = vec2(o.x, o.z*1.0) + o.w;
	o.yw = pos.yw;
	return o;
}

void main(){
	

	float Depth = texture(HeightMap, aTexCoord).g;	
	vec3 tN = vec3(0, 1, 0);
	vec3 shiftedVertex = aPos + (tN * Depth * 0.0);

	//vec4 p = projection * view * model * vec4(shiftedVertex, 1.0);
	vec4 p = projection * view * model * vec4(aPos, 1.0);

	vec4 Tex = ComputeScreenPos(p, 1.0);
	vec4 TexXZ = ComputeScreenPosXZ(p, 1.0);

	//shiftedVertex = aPos + (normal * Depth * 0.01);

	ScreenTexCoord = Tex;

	//Normal = mat3(transpose(inverse(model))) * tN;
	Normal = tN;
	WorldPos = vec3(model * vec4(shiftedVertex, 1.0));
	gl_Position = projection * view * model * vec4(shiftedVertex, 1.0);
}