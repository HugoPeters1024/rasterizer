inline static GLuint CompileShader(GLint type, const GLchar* const* source)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, source, NULL);
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
  static const char* vs_src;
  static const char* fs_src;

public:
  GLint vPos, uMvp, uCamera;
  GLuint program;
  Shader();

  void bind();
};

Shader::Shader()
{
  logDebug("Initializing shader");
  GLuint vs = CompileShader(GL_VERTEX_SHADER, &vs_src);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, &fs_src);

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  vPos = glGetAttribLocation(program, "vPos");
  uMvp = glGetUniformLocation(program, "uMvp");
  uCamera = glGetUniformLocation(program, "uCamera");

  glEnableVertexAttribArray(vPos);

  logDebug("a%i: \t vPos", vPos);
  logDebug("u%i: \t uMvp", uMvp);
  logDebug("u%i: \t uCamera", uCamera);

  logDebug("Done initializing shader");
}

void Shader::bind()
{
   glUseProgram(program);
}



const char* Shader::vs_src = R"(
#version 330 core
layout(location = 0) in vec3 vPos;

uniform mat4 uMvp;
uniform mat4 uCamera;

out vec3 pos;


void main() {
   gl_Position = uCamera * uMvp * vec4(vPos, 1);
   pos = vPos;
})";

const char* Shader::fs_src = R"(
#version 330 core
out vec4 color;

in vec3 pos;

void main() {
  color = vec4(1, 1, 1, 1); 
})";
