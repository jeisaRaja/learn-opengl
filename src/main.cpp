#include "../include/glad/glad.h"
#include "../include/stb_image/stb_image.h"
#include "camera/camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "shaders/shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>

unsigned int texture;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera *camera;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.5f;

unsigned int load_image(const std::string filepath);
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
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
      0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
      0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
      0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
      0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
      -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
      -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
      1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
      1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
      0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
      0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
      0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
      0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // Generate the VBO and VAO
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Object VAO
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Setting up the light VAO
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Configure OpenGL to use line to draw primitive
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Create the shader object
  Shader ourShader("./shaders/vertex_shader.vert",
                   "./shaders/fragment_shader.frag");
  Shader lightShader("./shaders/lightVertexShader.vert",
                     "./shaders/lightFragmentShader.frag");

  unsigned int diffuseMap = load_image("./images/green_metal_rust_diff_1k.jpg");
  unsigned int specularMap =
      load_image("./images/green_metal_rust_rough_1k.jpg");

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
    lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

    ourShader.use();
    ourShader.setVec3("viewPos", camera->position);

    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setFloat("material.shininess", 128.0f);

    // For DirLight
    glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
    ourShader.setVec3("dirLight.direction", lightDir);
    ourShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    ourShader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
    ourShader.setVec3("dirLight.specullar", glm::vec3(0.2f, 0.2f, 0.2f));

    // For SpotLight
    ourShader.setVec3("spotLight.position", camera->position);
    ourShader.setVec3("spotLight.direction", camera->front);
    ourShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(22.5f)));

    ourShader.setVec3("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    ourShader.setVec3("spotLight.diffuse", glm::vec3(1.f, 1.f, 1.f));
    ourShader.setVec3("spotLight.specular", glm::vec3(0.3f, 0.3f, 0.3f));

    // Define point light positions
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

    // Stronger, more distinct point lights (good for debugging)
    glm::vec3 ambientBase = glm::vec3(0.2f);
    glm::vec3 diffuseBase = glm::vec3(0.8f);
    glm::vec3 specularBase = glm::vec3(1.0f);

    for (int i = 0; i < 4; ++i) {
      std::string index = std::to_string(i);

      // Slight hue variation per light for easier identification
      glm::vec3 ambient =
          ambientBase + glm::vec3(0.05f * i, 0.02f * i, 0.01f * i);
      glm::vec3 diffuse = diffuseBase + glm::vec3(0.1f * i, 0.05f * i, 0.0f);
      glm::vec3 specular = specularBase;

      ourShader.setVec3("pointLights[" + index + "].position",
                        pointLightPositions[i]);
      ourShader.setVec3("pointLights[" + index + "].ambient", ambient);
      ourShader.setVec3("pointLights[" + index + "].diffuse", diffuse);
      ourShader.setVec3("pointLights[" + index + "].specular", specular);

      ourShader.setFloat("pointLights[" + index + "].constant", 1.0f);
      ourShader.setFloat("pointLights[" + index + "].linear", 0.007f);
      ourShader.setFloat("pointLights[" + index + "].quadratic", 0.0002f);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 view;
    view = camera->GetViewMatrix();
    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(55.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);

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
    for (int i = 0; i < 4; i++) {
      lightShader.setMat4("projection", projection);
      lightShader.setMat4("view", view);

      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.1f));
      lightShader.setVec3("lightPos", lightPos);
      lightShader.setMat4("model", model);
      lightShader.setVec3("viewPos", camera->position);

      glBindVertexArray(lightVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

unsigned int load_image(const std::string filepath) {
  unsigned int textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << filepath << std::endl;
    glDeleteTextures(1, &textureId);
    return 0;
  }

  GLenum format;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;
  else
    format = GL_RGB;

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  return textureId;
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
