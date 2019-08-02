#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBiTangent;

uniform mat4 uMvp;
uniform mat4 uCamera;
uniform float uTexSize;

out vec3 pos;
out vec3 normal;
out vec2 uv;
out vec3 tangent;
out vec3 bitangent;

void main() {
   vec4 worldPos = uMvp * vec4(vPos, 1);
   gl_Position = uCamera * worldPos; 
   pos = worldPos.xyz;
   normal = normalize(uMvp * vec4(vNormal, 0)).xyz;
   uv = vUV / uTexSize;
   tangent = normalize(uMvp * vec4(vTangent, 0)).xyz;
   bitangent = normalize(uMvp * vec4(vBiTangent, 0)).xyz;
}

