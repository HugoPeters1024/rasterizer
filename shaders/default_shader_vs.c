#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 uMvp;
uniform mat4 uCamera;
uniform float uTexSize;

out vec3 pos;
out vec3 normal;
out vec2 uv;

void main() {
   vec4 worldPos = uMvp * vec4(vPos, 1);
   gl_Position = uCamera * worldPos; 
   pos = worldPos.xyz;
   normal = normalize( uMvp * vec4(vNormal, 0)).xyz;
   uv = vUV / uTexSize;
}

