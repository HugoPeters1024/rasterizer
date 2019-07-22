#include <array>
#include <string>

#include "keyboard.h"
#include "camera.h"

#define NUM_LIGHTS 10

inline static GLuint CompileShader(GLint type, std::string &source)
{
  // Preprocess macro's

  GLuint shader = glCreateShader(type);
  const GLchar* c_str = source.c_str();
  glShaderSource(shader, 1, &c_str, NULL);
  glCompileShader(shader);

  // Check the compilation of the shader 
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    GLchar* errorLog = (GLchar*)malloc(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);

    printf("%s", errorLog);

    free(errorLog);
    return -1;
  }

  return shader;
};

class Shader
{
private:
  static std::string vs_src;
  static std::string fs_src;

public:
  GLint vPos, vNormal, uMvp, uCamera, uCamPos;
  std::array<GLint, 10> uLightsCol;
  std::array<GLint, 10> uLightsPos;
  GLuint program;
  Shader();

  void bind(const Camera* camera, mat4x4 &mvp);
};

Shader::Shader()
{
  logDebug("Initializing shader");
  GLuint vs = CompileShader(GL_VERTEX_SHADER, vs_src);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fs_src);

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  vPos = glGetAttribLocation(program, "vPos");
  vNormal = glGetAttribLocation(program, "vNormal");
  uMvp = glGetUniformLocation(program, "uMvp");
  uCamera = glGetUniformLocation(program, "uCamera");
  uCamPos = glGetUniformLocation(program, "uCamPos");

  for(int i=0; i<uLightsPos.size(); i++) {
    char name[12 + i / 10];
    snprintf(name, sizeof(name), "lights_p[%i]", i);
    uLightsPos[i] = glGetUniformLocation(program, name);

    snprintf(name, sizeof(name), "lights_c[%i]", i);
    uLightsCol[i] = glGetUniformLocation(program, name);
  }


  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vNormal);

  logDebug("Done initializing shader");
}

void Shader::bind(const Camera* camera, mat4x4 &mvp)
{
   mat4x4 m_camera;
   camera->getMatrix(m_camera);

   glUseProgram(program);

   glUniform3f(uLightsPos[0], 0, 15, 4);
   glUniform3f(uLightsCol[0], 200, 200, 300);

   glUniform3f(uCamPos, camera->pos[0], camera->pos[1], camera->pos[2]);
   glUniformMatrix4fv(uCamera, 1, GL_FALSE, (const GLfloat*)m_camera);
   glUniformMatrix4fv(uMvp, 1, GL_FALSE, (const GLfloat*)mvp);
}



std::string Shader::vs_src = R"(
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 uMvp;
uniform mat4 uCamera;

out vec3 pos;
out vec3 normal;


void main() {
   vec4 worldPos = uMvp * vec4(vPos, 1);
   gl_Position = uCamera * worldPos; 
   pos = worldPos.xyz;
   normal = normalize( uMvp * vec4(vNormal, 0)).xyz;
})";

std::string Shader::fs_src = R"(
#version 330 core
out vec4 color;

in vec3 pos;
in vec3 normal;

uniform mat4 uCamera;
uniform vec3 uCamPos;

//#define NUM_LIGHTS [NUM_LIGHTS];

// Array of structs is not supported
uniform vec3 lights_p[10];
uniform vec3 lights_c[10];

void main() {
  // ambient component
  vec3 materialCol = vec3(1.0f);
  vec3 fColor = 0.15f * materialCol;
  vec3 cameraPos = (uCamera * vec4(0, 0, -1, 1)).xyz;


  for(int i=0; i<10; i++) {
    vec3 light_p = lights_p[i];
    vec3 light_c = lights_c[i];

    vec3 lightVec = light_p - pos;

    float dist = length(lightVec);
    vec3 lightDir = lightVec / dist;
    float attenuation = 1.0f / (dist * dist);

    float vis = clamp(dot(lightDir, normal), 0, 1);
    vec3 diffuse = vis * light_c * attenuation;

    vec3 E = normalize(uCamPos - pos);
    vec3 R = reflect(-lightDir, normal);
    float cosAlpha = clamp(dot(E, R), 0, 1); 
    vec3 specular = materialCol * light_c * pow(cosAlpha, 10) * attenuation;

    fColor += diffuse + specular;
  }

  color = vec4(fColor, 1.0f);
})";
