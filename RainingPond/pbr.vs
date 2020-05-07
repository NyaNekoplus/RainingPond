#version 330 core
/*
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
*/

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
//out vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    vec3 aNormal = vec3(0, 0, 1);
    vec3 T = normalize(vec3(model * vec4(tangent, 1.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 1.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 1.0)));
    mat3 TBN = transpose(mat3(T,B,N));

    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    //Normal = mat3(model) * aNormal;   
    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * WorldPos;

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}