#version 330 core
out vec4 FragColor;

in vec4 ScreenTexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform vec3 ViewPos;
uniform sampler2D RippleTex;
uniform sampler2D ReflectMap;
uniform samplerCube RefractMap;


vec3 fresnel(float costheta, vec3 r){
    return r + (1.0-r)*pow(1.0 - costheta, 5.0);
}
void main(){
    vec2 ScreenClipTexCoord = ScreenTexCoord.xy / ScreenTexCoord.z;
    vec4 RipPos = texture(RippleTex, ScreenClipTexCoord);
    vec4 right  = texture(RippleTex, ScreenClipTexCoord + vec2(1.0/800.0, 0));
    vec4 left   = texture(RippleTex, ScreenClipTexCoord - vec2(1.0/800.0, 0));
    vec4 down   = texture(RippleTex, ScreenClipTexCoord - vec2(0, 1.0/600.0));
    vec4 up		= texture(RippleTex, ScreenClipTexCoord + vec2(0, 1.0/600.0));
    vec3 normal = normalize(cross(right.xyz - left.xyz, up.xyz - down.xyz));
    
    float ratio = 1.00 / 1.52;
    vec3 ViewDir = normalize(ViewPos - WorldPos);
    vec3 I = normalize(WorldPos - ViewPos);
    vec3 R = refract(I, normalize(normal), ratio);
    vec3 R2 = reflect(I, normal);

    float deltaH = ScreenTexCoord.y + 100;
    float theta = dot(R2, I);
    float len = deltaH / theta;
    vec4 hitPos = vec4(ScreenTexCoord.xyz + R2 * len, 1.0);
    vec2 coord = vec2(hitPos.x / 800.0, hitPos.z / 600.0) + vec2(0.5);
    
    //vec3 reflectColor = texture(ReflectMap, ScreenClipTexCoord).rgb;
    vec3 reflectColor = texture(ReflectMap, coord).rgb;
    vec3 refractColor = texture(RefractMap, R).rgb;
    //vec3 refractColor = texture(RefractMap, vec3(hitPos.x / 800.0, hitPos.y, 1.0 - hitPos.z / 600.0)).rgb;
    vec3 F0 = vec3(0.66);
    vec3 k = fresnel(max(dot(ViewDir, normalize(normal)), 0.0), F0);
    //vec3 Color = (1.0-0.5)*refractColor + reflectColor * 0.5;
    //vec3 Color = mix(refractColor, reflectColor, k);
    vec3 Color = mix(reflectColor, refractColor, k);

    // HDR tonemap and gamma correct
    Color = Color / (Color + vec3(1.0));
    Color = pow(Color, vec3(1.0/2.2));
    FragColor = vec4(Color, 1.0);
    //FragColor = vec4(0.0, 0.3, 1.0, 0.0);
}