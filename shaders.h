#ifndef SHADERS_H
#define SHADERS_H
#include <array>
#include <string>
#include <sstream>
#include <fstream>

#include "logger.h"
#include "keyboard.h"
#include "camera.h"
#include "exceptions.h"
#include "light.h"

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

inline static GLuint CompileShaderF(GLuint type, const char* filename)
{
  logDebug("reading shader source from %s", filename);
  std::ifstream t(filename);
  if (!t)
    throw ShaderMissingException(filename);
  std::stringstream buf;
  buf << t.rdbuf();
  std::string src = buf.str();
  return CompileShader(type, src);
}

inline static GLuint GenerateProgram(GLuint vs, GLuint fs)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (!isLinked)
  {
    GLint maxLength = 0;  
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar* errorLog = (GLchar*)malloc(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);

    logError("Shader linker error: %s", errorLog);

    glDeleteProgram(program);
    exit(5);
  }
  return program;
}

class DefaultShader
{
private:
  const LightSet* lightset;
  GLint vPos, vNormal, vUV, uMvp, uCamera, uCamPos, uTexSize;
  std::array<GLint, 10> uLightsCol;
  std::array<GLint, 10> uLightsPos;
  GLuint program;
public:
  DefaultShader(const LightSet* lights);

  void prepare(GLuint vbo, GLuint nbo, GLuint uvo) const;
  void bind(const Camera* camera, Matrix4 mvp, GLuint tex, float texSize = 1) const;
};

DefaultShader::DefaultShader(const LightSet* lights) : lightset(lights)
{
  logDebug("Initializing shader");
  GLuint vs = CompileShaderF(GL_VERTEX_SHADER, "shaders/default_shader_vs.c");
  GLuint fs = CompileShaderF(GL_FRAGMENT_SHADER, "shaders/default_shader_fs.c");
  program = GenerateProgram(vs, fs);


  vPos = glGetAttribLocation(program, "vPos");
  vNormal = glGetAttribLocation(program, "vNormal");
  vUV = glGetAttribLocation(program, "vUV");
  uMvp = glGetUniformLocation(program, "uMvp");
  uCamera = glGetUniformLocation(program, "uCamera");
  uCamPos = glGetUniformLocation(program, "uCamPos");
  uTexSize = glGetUniformLocation(program, "uTexSize");

  for(int i=0; i<uLightsPos.size(); i++) {
    char name[12 + i / 10];
    snprintf(name, sizeof(name), "lights_p[%i]", i);
    uLightsPos[i] = glGetUniformLocation(program, name);

    snprintf(name, sizeof(name), "lights_c[%i]", i);
    uLightsCol[i] = glGetUniformLocation(program, name);
  }

  logDebug("Done initializing shader");
}

void DefaultShader::prepare(GLuint vbo, GLuint nbo, GLuint uvo) const
{
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, uvo);
   glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));

   glEnableVertexAttribArray(vPos);
   glEnableVertexAttribArray(vNormal);
   glEnableVertexAttribArray(vUV);
}

void DefaultShader::bind(const Camera* camera, Matrix4 mvp, GLuint tex, float texSize) const
{
   mat4x4 m_camera, u_mvp;
   camera->getMatrix().unpack(m_camera);
   mvp.unpack(u_mvp);

   glUseProgram(program);
   lightset->sendToShader(uLightsPos, uLightsCol);


   glUniform1f(uTexSize, texSize);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex);

   glUniform3f(uCamPos, camera->pos.x, camera->pos.y, camera->pos.z);
   glUniformMatrix4fv(uCamera, 1, GL_FALSE, (const GLfloat*)m_camera);
   glUniformMatrix4fv(uMvp, 1, GL_FALSE, (const GLfloat*)u_mvp);
}

class NormalMappedShader
{
private:
  GLint vPos, vNormal, vUV, vTangent, vBiTangent, uMvp, uCamera, uCamPos, uTex, uNormalTex, uTexSize;
  std::array<GLint, 10> uLightsCol;
  std::array<GLint, 10> uLightsPos;
  GLuint program;
  const LightSet* lights;
public:
  NormalMappedShader(const LightSet* lights);

  void prepare(GLuint vbo, GLuint nbo, GLuint uvo, GLuint tbo, GLuint btbo) const;
  void bind(const Camera* camera, Matrix4 mvp, GLuint tex, GLuint n_tex, float texSize=1) const;
};

NormalMappedShader::NormalMappedShader(const LightSet* lights) : lights(lights)
{
  logDebug("Initializing shader");

  GLuint vs = CompileShaderF(GL_VERTEX_SHADER, "shaders/normalmapped_shader_vs.c");
  GLuint fs = CompileShaderF(GL_FRAGMENT_SHADER, "shaders/normalmapped_shader_fs.c");
  program = GenerateProgram(vs, fs);

  vPos = glGetAttribLocation(program, "vPos");
  vNormal = glGetAttribLocation(program, "vNormal");
  vUV = glGetAttribLocation(program, "vUV");
  vTangent = glGetAttribLocation(program, "vTangent");
  vBiTangent = glGetAttribLocation(program, "vBiTangent");
  uMvp = glGetUniformLocation(program, "uMvp");
  uCamera = glGetUniformLocation(program, "uCamera");
  uCamPos = glGetUniformLocation(program, "uCamPos");
  uTexSize = glGetUniformLocation(program, "uTexSize");

  uTex = glGetUniformLocation(program, "tex");
  uNormalTex = glGetUniformLocation(program, "n_tex");

  for(int i=0; i<uLightsPos.size(); i++) {
    char name[12 + i / 10];
    snprintf(name, sizeof(name), "lights_p[%i]", i);
    uLightsPos[i] = glGetUniformLocation(program, name);

    snprintf(name, sizeof(name), "lights_c[%i]", i);
    uLightsCol[i] = glGetUniformLocation(program, name);
  }

  logDebug("Done initializing shader");
}


void NormalMappedShader::prepare(GLuint vbo, GLuint nbo, GLuint uvo, GLuint tbo, GLuint btbo) const
{
   logError("debugging: %i %i", vTangent, vBiTangent);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, uvo);
   glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, tbo);
   glVertexAttribPointer(vTangent, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
   glBindBuffer(GL_ARRAY_BUFFER, btbo);
   glVertexAttribPointer(vBiTangent, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));

   glEnableVertexAttribArray(vPos);
   glEnableVertexAttribArray(vNormal);
   glEnableVertexAttribArray(vUV);
   glEnableVertexAttribArray(vTangent);
   glEnableVertexAttribArray(vBiTangent);
}

void NormalMappedShader::bind(const Camera* camera, Matrix4 mvp, GLuint tex, GLuint n_tex, float texSize) const
{
   mat4x4 m_camera, u_mvp;
   camera->getMatrix().unpack(m_camera);
   mvp.unpack(u_mvp);

   glUseProgram(program);
   lights->sendToShader(uLightsPos, uLightsCol);

   glUniform1f(uTexSize, texSize);
   glUniform1i(uTex, 0);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex);
   glUniform1i(uNormalTex, 1);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, n_tex);

   glUniform3f(uCamPos, camera->pos.x, camera->pos.y, camera->pos.z);
   glUniformMatrix4fv(uCamera, 1, GL_FALSE, (const GLfloat*)m_camera);
   glUniformMatrix4fv(uMvp, 1, GL_FALSE, (const GLfloat*)u_mvp);
}

#endif
