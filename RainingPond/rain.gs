#version 330 core

layout (points) in;
layout (points, max_vertices = 2) out;

void circle(vec4 position){ //µãµÄÎ»ÖÃ
    for(int i=0; i < 100; i++){
        
    }
}

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_PointSize = 10;

    EndPrimitive();
}