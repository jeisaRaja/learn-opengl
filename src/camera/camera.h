#ifndef CAMERA_H
#define CAMERA_H
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 POSITION = glm::vec3(0.0);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 WORLD_UP = glm::vec3(0.0, 1.0f, 0.0f);

class Camera {
public:
  glm::vec3 world_up;
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  float zoom;
  float speed;
  float yaw;
  float pitch;

  Camera(glm::vec3 position = glm::vec3(0.0), glm::vec3 front = FRONT,
         glm::vec3 up = WORLD_UP, glm::vec3 world_up = WORLD_UP,
         float zoom = ZOOM, float speed = SPEED, float yaw = YAW,
         float pitch = PITCH) {
    this->position = position;
    this->front = front;
    this->up = up;
    this->world_up = world_up;
    this->zoom = zoom;
    this->speed = speed;
    this->yaw = YAW;
    this->pitch = PITCH;

    UpdateCameraVectors();
  }

  glm::mat4 GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
  }

  void UpdateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
  }

  void ProcessKeyboardInput(Camera_Movement direction, float deltaTime) {
    float velocity = deltaTime * speed;
    if (direction == FORWARD)
      position += front * velocity;
    if (direction == BACKWARD)
      position -= front * velocity;
    if (direction == RIGHT)
      position += right * velocity;
    if (direction == LEFT)
      position -= right * velocity;
  }

  void ProcessMouseInput(float x, float y) {
    yaw += x;
    pitch += y;

    if (pitch > 89.0f) {
      pitch = 89.0f;
    }
    if (pitch < -89.0f) {
      pitch = -89.0f;
    }

    UpdateCameraVectors();
  };
};

#endif
