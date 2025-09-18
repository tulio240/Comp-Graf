#ifdef _WIN32
#include <windows.h>

// Use glad to initialize OpenGL context on Windows
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#else

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#endif


#include "error.h"
#include "shader.h"
#include "circle.h"
#include "ponteiro.h"

#include <stdio.h>
#include <stdlib.h>
#include <glm/ext/matrix_transform.hpp>
#include <chrono>
#include <ctime>

#include <iostream>

static CirclePtr _clock;
static ShaderPtr shd;
static PonteiroPtr hour_pointer;
static PonteiroPtr minutes_pointer;
static PonteiroPtr seconds_pointer;

static glm::vec4 clock_color;
static glm::vec4 hour_color;
static glm::vec4 seconds_color;

glm::mat4 hour_M;
glm::mat4 minutes_M;
glm::mat4 seconds_M;

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(1);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

void get_current_time(int& hour, int& minutes, int& seconds){
  std::chrono::system_clock::time_point now_tp = std::chrono::system_clock::now(); //get current time

  std::time_t now_c_time = std::chrono::system_clock::to_time_t(now_tp); //convert time to time_t

  std::tm* local_time = std::localtime(&now_c_time);

  hour = local_time->tm_hour;
  minutes = local_time->tm_min;
  seconds = local_time->tm_sec;
}

static void initialize ()
{
  clock_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  hour_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  seconds_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

  int h, m, s;
  get_current_time(h, m, s);

  float s_r = s * 6.0f;
  float m_r = m * 6.0f + s_r/60;
  float h_r = h%12 * 30 + m_r/60 + s_r/3600;

  hour_M = glm::mat4(0.1f);
  hour_M = glm::scale(hour_M, glm::vec3(1.0f, 1.8f, 1.0f));
  hour_M = glm::rotate(hour_M, glm::radians(h_r), glm::vec3(0,0,-1));

  minutes_M = glm::mat4(0.1f);
  minutes_M = glm::scale(minutes_M, glm::vec3(0.8f, 1.0f, 1.0f));
  minutes_M = glm::rotate(minutes_M, glm::radians(m_r), glm::vec3(0,0,-1));

  seconds_M = glm::mat4(0.1f);
  seconds_M = glm::scale(seconds_M, glm::vec3(0.3f, 1.0f, 1.0f));
  seconds_M = glm::rotate(seconds_M, glm::radians(s_r), glm::vec3(0,0,-1));

  glClearColor(0.0f,1.0f,1.0f,0.6f);

  _clock = Circle::Make(60);

  hour_pointer = Ponteiro::Make();
  minutes_pointer = Ponteiro::Make();
  seconds_pointer = Ponteiro::Make();

  shd = Shader::Make();
  shd->AttachVertexShader("shaders/vertex.glsl");
  shd->AttachFragmentShader("shaders/fragment.glsl");
  shd->Link();

  Error::Check("initialize");
}

static void display (GLFWwindow* win)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shd->UseProgram();

  glm::mat4 matrix(1.0f);

  shd->SetUniform("icolor", {clock_color});
  shd->SetUniform("M", matrix);

  _clock->Draw();

  shd->SetUniform("icolor", {hour_color});
  shd->SetUniform("M", hour_M);
  hour_pointer->Draw();

  shd->SetUniform("icolor", {hour_color});
  shd->SetUniform("M", minutes_M);
  minutes_pointer->Draw();

  shd->SetUniform("icolor", {seconds_color});
  shd->SetUniform("M", seconds_M);
  seconds_pointer->Draw();

  Error::Check("display");
}

void update (float dt){

  float seconds_r = dt * 6;
  float minutes_r = dt * 1/10;
  float hour_r = dt * 1/600;

  hour_M = glm::rotate(hour_M, glm::radians(hour_r), glm::vec3(0,0,-1));
  minutes_M = glm::rotate(minutes_M, glm::radians(minutes_r), glm::vec3(0,0,-1));
  seconds_M = glm::rotate(seconds_M, glm::radians(seconds_r), glm::vec3(0,0,-1));
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
  //glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(400,400,"Clock",nullptr,nullptr);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  
  glfwMakeContextCurrent(win);
 
#ifdef __glad_h_
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD OpenGL context\n");
    exit(1);
   }
#endif
#ifdef __glew_h__
  glewInit(); 
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW OpenGL context\n");
    exit(1);
  }
#endif

  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
  float delta_time = 0.0f;

  while(!glfwWindowShouldClose(win)) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    std::chrono::duration<float> time_elapsed = now - t0;

    delta_time = time_elapsed.count();

    t0 = now;

    update(delta_time);
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
