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
  GLint vPos, vNormal, uMvp, uCamera;
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
  vNormal = glGetAttribLocation(program, "vNormal");
  uMvp = glGetUniformLocation(program, "uMvp");
  uCamera = glGetUniformLocation(program, "uCamera");

  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vNormal);

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
layout(location = 1) in vec3 vNormal;

uniform mat4 uMvp;
uniform mat4 uCamera;

out vec3 pos;
out vec3 normal;


void main() {
   gl_Position = uCamera * uMvp * vec4(vPos, 1);
   pos = gl_Position.xyz;
   normal = normalize(uCamera * uMvp * vec4(vNormal, 0)).xyz;
})";

const char* Shader::fs_src = R"(
#version 330 core
out vec4 color;

in vec3 pos;
in vec3 normal;

void main() {
  vec3 lightCol = vec3(2, 1, 5) * 15;
  vec3 materialCol = vec3(1, 1, 1);


  vec3 light = vec3(0, 5, 0);
  vec3 lightVec = light - pos;
  float dist = length(lightVec);
  vec3 lightDir = lightVec / dist;

  vec3 ambient = 0.2f * materialCol;

  vec3 diffuse = max(dot(lightDir, normal), 0) * lightCol / (dist * dist);
  vec3 fColor = ambient + diffuse;
  color = vec4(fColor, 1);
})";
