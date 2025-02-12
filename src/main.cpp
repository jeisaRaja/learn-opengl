#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/stb_image/stb_image.h"
#include "camera/camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "shaders/shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

int width, height, nrChannels;
unsigned int texture;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;

Camera *camera;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.5f;

void load_image(void);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  cameraSpeed = deltaTime * 2.f;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W)) {
    camera->ProcessKeyboardInput(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S)) {
    camera->ProcessKeyboardInput(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D)) {
    camera->ProcessKeyboardInput(RIGHT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A)) {
    camera->ProcessKeyboardInput(LEFT, deltaTime);
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  camera = new Camera();
  glfwSetCursorPosCallback(window, mouse_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initiate GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
      // positions        // texture coords (s,t)
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, ///
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
                                       //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                                       //
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
                                       //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                                       //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                                       //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //

  };

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Configure OpenGL to use line to draw primitive
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");
  Shader lightShader("./shaders/lightVertexShader.vs",
                     "./shaders/lightFragmentShader.fs");

  load_image();

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.f),
                        glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 view;
    view = camera->GetViewMatrix();
    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(55.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    ourShader.setMat4("model", model);
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(0.8f, 0.3f, 0.5f));
      model = glm::scale(model, glm::vec3(0.5f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

void load_image(void) {

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *data =
      stbi_load("./images/pebbles.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xOffset = xpos - lastX;
  float yOffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xOffset *= sensitivity;
  yOffset *= sensitivity;

  camera->ProcessMouseInput(xOffset, yOffset);
}
