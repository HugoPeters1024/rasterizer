#include <stdio.h>
#include <iostream>

#define GL_DEBUG

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GLFW/glfw3.h>
#include "linmath.h"
#include "vec.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gameobject.h"
#include "utils.h"
#include "logger.h"
#include "application.h"
#include "keyboard.h"

#ifdef GL_DEBUG
#include "gl_debug.h"
#endif

GLFWwindow* window;
Application* app;
Keyboard* keyboard;

void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  //printf("key: %i, scancode: %i, action: %i\n", key, scancode, action);
}

int main(int argc, char** argv) {
  // Set log level
  log_set_level(L_DEBUG);

  // Setup GLFW handlers
  glfwSetErrorCallback(glfw_error_callback);

  // Initialize GLFW
  if (!glfwInit())
      exit(EXIT_FAILURE);

  // Enforce minimum OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // Initialize window
  window = glfwCreateWindow(640, 480, "Rasterizer", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Window creation failed!\n");
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

#ifdef GL_DEBUG
  printf("OpenGL debugging enabled.\n");
  // Enable debugging
  glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDEBUGPROC(gl_debug_output), nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

  // Print info
  const GLubyte* vendor = glGetString(GL_VENDOR);
  logDebug("Video card:\t\t%s", vendor);
  const GLubyte* renderer = glGetString(GL_RENDERER);
  logDebug("Renderer:\t\t%s", renderer);
  const GLubyte* version = glGetString(GL_VERSION);
  logDebug("OpenGL version:\t%s", version);


  //glCullFace(GL_FRONT_AND_BACK);
  glEnable(GL_DEPTH_TEST);
  glfwSwapInterval(1); //vsync

  logInfo("startup completed");

  keyboard = new Keyboard(window);
  app = new Application();
  app->init();

  while(!(glfwWindowShouldClose(window) | app->shouldClose()))
  {
     keyboard->swapBuffers();
     int w, h;
     glfwGetFramebufferSize(window, &w, &h);

     glViewport(0, 0, w, h);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

     app->loop(w, h, keyboard);

     glfwSwapBuffers(window);
     glfwPollEvents();
  }

  delete app;

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(0);
}
