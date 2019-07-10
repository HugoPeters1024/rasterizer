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
  GLint vPos, vNormal, uMvp, uCamera, uCameraPos;
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
  uCameraPos = glGetUniformLocation(program, "uCameraPos");

  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vNormal);

  logDebug("a%i: \t vPos", vPos);
  logDebug("u%i: \t uMvp", uMvp);
  logDebug("u%i: \t uCamera", uCamera);
  logDebug("u%i: \t uCameraPos", uCameraPos);

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
   vec4 worldPos = uMvp * vec4(vPos, 1);
   gl_Position = uCamera * worldPos; 
   pos = worldPos.xyz;
   normal = normalize( uMvp * vec4(vNormal, 0)).xyz;
})";

const char* Shader::fs_src = R"(
#version 330 core
out vec4 color;

in vec3 pos;
in vec3 normal;

uniform vec3 uCameraPos;

void main() {
  vec3 lightCol = vec3(2, 2, 1) * 2000.0f;
  vec3 materialCol = vec3(0.7f, 0.5f, 1);


  vec3 light = vec3(0, 100, 4);
  vec3 lightVec = light - pos;
  float dist = length(lightVec);
  vec3 lightDir = lightVec / dist;
  float attenuation = 1.0f / (dist * dist);

  vec3 ambient = 0.1f * materialCol;

  vec3 diffuse = clamp(dot(lightDir, normal), 0, 1) * lightCol * attenuation;

  vec3 E = normalize(uCameraPos - pos);
  vec3 R = reflect(-lightDir, normal);
  float cosAlpha = clamp(dot(E, R), 0, 1); 
  vec3 specular = materialCol * lightCol * pow(cosAlpha, 50) * attenuation;

  vec3 fColor = ambient + diffuse + specular;
  color = vec4(fColor, 1);
})";
