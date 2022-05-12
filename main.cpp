#include "extern/glad/glad.h"
#include "extern/stb_image.h"

#include "extern/glm/glm.hpp"
#include "extern/glm/gtc/matrix_transform.hpp"
#include "extern/glm/gtc/type_ptr.hpp"
#include "extern/glm/gtx/string_cast.hpp"

#include "helpers/shader_class.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int width = 800, height = 600;

GLfloat vertices[] = {
// Positions          // Texture Coordinates
-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
0.5f, 0.5f, 0.0f,     1.0f, 1.0f,
0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
-0.5f, 0.5f, 0.0f,    0.0f, 1.0f,
};

GLuint indices[] = {
0, 1, 2,
0, 1, 3,
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

  GLFWwindow *window = glfwCreateWindow(width, height, "Transform!", NULL, NULL);
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

  //+-------------------------------
  // Create Shaders
  shader_class vertex_shader("../shader_folder/shader.vert", GL_VERTEX_SHADER);
  shader_class fragment_shader("../shader_folder/shader.frag", GL_FRAGMENT_SHADER);

  //seperately compile the shaders
  vertex_shader.compile_shader();
  fragment_shader.compile_shader();

  //error checking
  vertex_shader.compile_status();
  fragment_shader.compile_status();
  //+-------------------------------------------------------------------------

  //+-------------------------------------------------------------------------
  // Create a shader program
  GLuint shader_program;
  shader_program = glCreateProgram();

  // Attach Shaders to the shader program
  vertex_shader.attach_to(shader_program);
  fragment_shader.attach_to(shader_program);

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
  vertex_shader.delete_shader();
  fragment_shader.delete_shader();

  //+-------------------------------------------------------------------------
  // Make a VAO (Vertex Array Object) and VBO (Vertex Buffer Object)
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind this unsigned int as a Vertex Array Object
  glBindVertexArray(VAO);
  // Bind this unsigned int as a Vertex Array Buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Describe the different locations of information in the vertex array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //+-------------------------------------------------------------------------

  //Create a texture
  GLuint texture_1;
  glGenTextures(1, &texture_1);
  glBindTexture(GL_TEXTURE_2D, texture_1);

  // Set some parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load("../resources/texture_1.png", &width, &height, &nr_channels, 0);

  if(data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    std::cout << "Failed to Create Texture\n";
    return -1;
  }
  stbi_image_free(data);

  glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

  float t1 = 0.0f, t2 = 0.125f, t3 = 0.0f;
  float theta = 100.0f, r1 = 1.0f, r2 = 0.0f, r3 = 0.0;
  float s1 = 1.0f, s2 = 1.0f, s3 = 1.0;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    // Set a new color for OpenGL to clear the screen with
    glClearColor(0.207f, 0.866f, 0.545f, 1.0f);

    glm::mat4 m = glm::mat4(1.0f);

    int time = glfwGetTime();
    m = glm::translate(m, glm::vec3(t1, t2, t3));

    m = glm::rotate(m, (float)time, glm::vec3(r1, r2, r3));

    m = glm::scale(m, glm::vec3(s1 * sin(time), s2 * sin(time), s3 * sin(time)));

    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m));

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_1);

    // Poll for Input from the keyboard
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Clear the Screen with the color set earlier

    glfwSwapBuffers(window);
    glfwPollEvents();
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
