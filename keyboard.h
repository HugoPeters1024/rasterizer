#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <vector>
#include <map>

enum action {
  MOVE_RIGHT,
  MOVE_LEFT,
  MOVE_FORWARD,
  MOVE_BACKWARD,
  MOVE_UP,
  MOVE_DOWN,

  LOOK_UP,
  LOOK_DOWN,
  LOOK_LEFT,
  LOOK_RIGHT,

  JUMP,
};

class Keyboard
{
  private:
    GLFWwindow* window;
    std::vector<int> key_map, old_key_map;
    std::map<action, int> action_map;
    int map_size;

    void generateActionMap();

  public:
    Keyboard(GLFWwindow* window);
    // These methods suffer from this bug: https://github.com/glfw/glfw/issues/747
    // A fix is to be released in 3.3.0
    bool isPressed(action a) const; // Pressed in this tick
    bool isReleased(action a) const; // Released in this tick

    void swapBuffers();
    bool isDown(action a) const;  // Down at all
};

Keyboard::Keyboard(GLFWwindow* window) 
{ 
  this->window = window;

  // Preserves keypresses until polled to prevent missing events
  // This implicitly requires that all keys are polled to maintain
  // normal behavior
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

  map_size = GLFW_KEY_LAST + 1;
  key_map = std::vector<int>(map_size);
  old_key_map = std::vector<int>(map_size);
  action_map = std::map<action, int>();
  generateActionMap();
}

void Keyboard::swapBuffers()
{
  // Keys below barcode 32 are not in use and generate errors.
  for(int i=32; i<map_size; i++)
  {
    old_key_map[i] = key_map[i];
    key_map[i] = glfwGetKey(window, i);
  }
}

bool Keyboard::isDown(action a) const
{
   int key = action_map.at(a);
   return key_map[key] == GLFW_PRESS;
}

bool Keyboard::isPressed(action a) const
{
   int key = action_map.at(a);
   return key_map[key] == GLFW_PRESS && old_key_map[key] != GLFW_PRESS;
}

bool Keyboard::isReleased(action a) const
{
   int key = action_map.at(a);
   return key_map[key] != GLFW_PRESS && old_key_map[key] == GLFW_PRESS;
}


void Keyboard::generateActionMap()
{
  action_map[MOVE_LEFT]     = GLFW_KEY_A;
  action_map[MOVE_RIGHT]    = GLFW_KEY_D;
  action_map[MOVE_FORWARD]  = GLFW_KEY_W;
  action_map[MOVE_BACKWARD] = GLFW_KEY_S;
  action_map[MOVE_UP]       = GLFW_KEY_Q;
  action_map[MOVE_DOWN]     = GLFW_KEY_E;

  action_map[LOOK_UP]       = GLFW_KEY_UP;
  action_map[LOOK_DOWN]     = GLFW_KEY_DOWN;
  action_map[LOOK_LEFT]     = GLFW_KEY_LEFT;
  action_map[LOOK_RIGHT]    = GLFW_KEY_RIGHT;
  
  action_map[JUMP]          = GLFW_KEY_SPACE;
}
#endif
