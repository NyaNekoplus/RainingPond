#version 330 core
layout(location = 0) in vec3 aPos;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main(){
	TexCoords = aPos;
	mat4 mView = mat4(mat3(view));
	vec4 clipPos = projection * mView * vec4(aPos, 1.0);
	gl_Position = clipPos.xyww;
}