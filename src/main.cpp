#include "../include/glad/glad.h"
#include "shaders/shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <bits/types/struct_timeval.h>
#include <cmath>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initiate GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
      -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top-left
      0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // top-right
      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
      /*-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left*/
      /*-0.8f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // top-left*/
      /*0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // top-right*/
      /*0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-right*/
      /*-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left*/
  };

  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Configure OpenGL to use line to draw primitive
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    ourShader.use();
    ourShader.setFloat("someUniform", 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteVertexArrays(1, &EBO);
  glfwTerminate();
  return 0;
}
