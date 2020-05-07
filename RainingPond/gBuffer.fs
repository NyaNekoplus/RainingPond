#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 Position;
in vec2 TexCoord;

uniform sampler2D NormalMap;
uniform sampler2D DepthMap;
void main(){
    gPosition = Position;

}