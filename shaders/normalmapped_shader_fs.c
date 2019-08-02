#version 330 core
out vec4 color;

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;

uniform mat4 uCamera;
uniform vec3 uCamPos;

uniform sampler2D tex;
uniform sampler2D n_tex;

#define NUM_LIGHTS 10

// Array of structs is not supported
uniform vec3 lights_p[NUM_LIGHTS]; 
uniform vec3 lights_c[NUM_LIGHTS];

void main() {
  // read normals from map
  mat3 TBN = inverse(mat3(tangent, bitangent, normal));
  vec3 tnormal = normalize((texture(n_tex, uv).xyz * 2 - 1) * TBN);

  // ambient component
  vec3 materialCol = texture(tex, uv).xyz;
  vec3 fColor = 0.15f * materialCol;
  vec3 cameraPos = (uCamera * vec4(0, 0, -1, 1)).xyz;

  for(int i=0; i<NUM_LIGHTS; i++) {
    vec3 light_p = lights_p[i];
    vec3 light_c = lights_c[i];

    vec3 lightVec = light_p - pos;

    float dist = length(lightVec);
    vec3 lightDir = lightVec / dist;
    float attenuation = 1.0f / (dist * dist);

    float vis = max(dot(lightDir, tnormal), 0);
    vec3 diffuse = vis * light_c * attenuation;

    vec3 E = normalize(uCamPos - pos);
    vec3 R = reflect(-lightDir, tnormal);
    float cosAlpha = max(dot(E, R), 0); 
    vec3 specular = materialCol * light_c * pow(cosAlpha, 100) * attenuation;

    fColor += diffuse + specular;
  }

  color = vec4(fColor, 1.0f);
}
