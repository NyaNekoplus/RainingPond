#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Tangent;
layout (location = 3) in vec3 Bitangent;

out vec2 fTexCoord;
out vec3 WorldPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out mat3 aTBN;

uniform sampler2D HeightMap;
uniform float HeightScale;
uniform vec3 LightPos;
uniform vec3 ViewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vec3 normal = vec3(0, 1, 0);
	vec3 T = normalize(vec3(model * vec4(Tangent, 1.0)));
	//vec3 B = normalize(vec3(model * vec4(Bitangent, 1.0)));
	vec3 N = normalize(vec3(model * vec4(normal, 1.0)));
	vec3 B = normalize(cross(T, N));
	mat3 TBN = transpose(mat3(T,B,N));
	aTBN = mat3(T,B,N);

	fTexCoord = TexCoord;
	float Depth = texture(HeightMap, fTexCoord).r;				//vec3
	
	//float df = 0.30*Depth.x + 0.59*Depth.y + 0.11*Depth.z;	//translate to grey
	
	vec3 tN = vec3(0,1,0);
	vec3 shiftedVertex = Pos + (tN * Depth * HeightScale);
	WorldPos = vec3(model * vec4(shiftedVertex, 1.0));

	
	/*TangentLightPos =  LightPos;
    TangentViewPos =  ViewPos;
    TangentFragPos =  WorldPos;*/
    TangentLightPos = TBN * LightPos;
    TangentViewPos = TBN * ViewPos;
    TangentFragPos = TBN * WorldPos;

	gl_Position = projection * view * model * vec4(shiftedVertex, 1.0);
}