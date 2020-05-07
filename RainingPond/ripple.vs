#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec4 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int RippleNum;
uniform vec3 RipplePosition[256];

uniform float PeriodOfRain;
void main(){
	vec4 pos = model * vec4(Position, 1.0);

	float h = 0.0;
	float frequency = 1;
	float RippleSpeed = 0.60;
	
	for(int i = 0; i < RippleNum; i++){
	
		float d = distance(pos.xz , RipplePosition[i].xz);
		if(PeriodOfRain > 0.0){
			float radius = PeriodOfRain * RippleSpeed;
			float ha = PeriodOfRain * 3000 + 1;
			float da = (d - radius) * 0.5;
			float e = sin(frequency * d - radius) / (ha * (da * da + 1));
			h = h + e * 0.5;
		}
	}
	//h /= RippleNum;
	WorldPos = model * vec4(Position.x, h, Position.z, 1.0);
	gl_Position = projection * view * model * vec4(Position.x, h, Position.z, 1.0);
}