#include "extern/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int width = 800, height = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(
      width, height, "Not so-Black and Dull Window", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to Create a Window :(\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    // Set a new color for OpenGL to clear the screen with
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSwapBuffers(window);

    // Poll for Input from the keyboard
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Clear the Screen with the color set earlier
    glClear(GL_COLOR_BUFFER_BIT);
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
