#include "extern/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int width = 800, height = 600;

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
    "}\0";

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void shader_info(std::string shader_name, GLuint shader) {
  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << shader << " has failed compilation:\n" << info_log << "\n";
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, "Triangle!", NULL, NULL);
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

  //+-------------------------------------------------------------------------
  // Create a Vertex Shader
  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  // Bind Shader Code
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  // Shader Error Checking
  shader_info("Vertex Shader", vertex_shader);
  //+-------------------------------------------------------------------------
  //+-------------------------------------------------------------------------
  // Create a Fragment Shader
  GLuint fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // Bind Shader Code
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  // Shader Error Checking
  shader_info("Fragment Shader", fragment_shader);
  //+-------------------------------------------------------------------------

  //+-------------------------------------------------------------------------
  // Create a shader program
  GLuint shader_program;
  shader_program = glCreateProgram();

  // Attach Shaders to the shader program
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  // Link shader program to the OpenGL program to run
  glLinkProgram(shader_program);
  // check for linking errors
  int success;
  char info_log[512];
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cout << "Shader Program Linking Failed :(\n" << info_log << "\n";
  }
  //+-------------------------------------------------------------------------

  // Use the program that you just made
  glUseProgram(shader_program);

  // Delete the shader objects since they're not necessary anymore
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  //+-------------------------------------------------------------------------
  // Make a VAO (Vertex Array Object) and VBO (Vertex Buffer Object)
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind this unsigned int as a Vertex Array Object
  glBindVertexArray(VAO);
  // Bind this unsigned int as a Vertex Array Buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Describe the different locations of information in the vertex array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  //+-------------------------------------------------------------------------

  while (!glfwWindowShouldClose(window)) {
    // Set a new color for OpenGL to clear the screen with
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    // Poll for Input from the keyboard
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Clear the Screen with the color set earlier
    glClear(GL_COLOR_BUFFER_BIT);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
