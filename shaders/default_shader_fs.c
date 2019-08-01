#version 330 core
out vec4 color;

in vec3 pos;
in vec3 normal;
in vec2 uv;

uniform mat4 uCamera;
uniform vec3 uCamPos;

uniform sampler2D tex;

#define NUM_LIGHTS 10

// Array of structs is not supported
uniform vec3 lights_p[NUM_LIGHTS]; 
uniform vec3 lights_c[NUM_LIGHTS];

void main() {
  vec3 materialCol = texture(tex, uv).xyz;
  // ambient component
  vec3 fColor = 0.15f * materialCol;
  vec3 cameraPos = (uCamera * vec4(0, 0, -1, 1)).xyz;

  for(int i=0; i<NUM_LIGHTS; i++) {
    vec3 light_p = lights_p[i];
    vec3 light_c = lights_c[i];

    vec3 lightVec = light_p - pos;

    float dist = length(lightVec);
    vec3 lightDir = lightVec / dist;
    float attenuation = 1.0f / (dist * dist);

    float vis = max(dot(lightDir, normal), 0);
    vec3 diffuse = vis * light_c * attenuation;

    vec3 E = normalize(uCamPos - pos);
    vec3 R = reflect(-lightDir, normal);
    float cosAlpha = clamp(dot(E, R), 0, 1); 
    vec3 specular = materialCol * light_c * pow(cosAlpha, 100) * attenuation;

    fColor += diffuse + specular;
  }

  color = vec4(fColor, 1.0f);
}
