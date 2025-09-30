
#ifdef _WIN32
#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <GLFW/glfw3.h>

#include "scene.h"
#include "state.h"
#include "camera2d.h"
#include "color.h"
#include "transform.h"
#include "error.h"
#include "shader.h"
#include "quad.h"
#include "triangle.h"

#include "circle.h"

#include <iostream>

static ScenePtr scene;
static CameraPtr camera;

class MovePointer;
using MovePointerPtr = std::shared_ptr<MovePointer>;
class MovePointer : public Engine 
{
  TransformPtr m_trf;
protected:
  MovePointer (TransformPtr trf) 
  : m_trf(trf) 
  {
  }
public:
  static MovePointerPtr Make (TransformPtr trf)
  {
    return MovePointerPtr(new MovePointer(trf));
  }
  virtual void Update (float dt)
  {
    m_trf->Rotate(-dt/30.0f*180.0f,0,0,1);
  }
};

class MoveBody;
using MoveBodyPtr = std::shared_ptr<MoveBody>;

class MoveBody : public Engine{
  TransformPtr m_trf;

  protected:
    MoveBody (TransformPtr trf) : m_trf(trf){
    }
  public:
    static MoveBodyPtr Make (TransformPtr trf)
  {
    return MoveBodyPtr(new MoveBody(trf));
  }
  virtual void Update (float dt)
  {
    m_trf->Rotate(-dt/30.0f*180.0f,0,0,1);
  }

};

static void initialize (void)
{
  // set background color: black 
  glClearColor(0.0f,0.0f,0.0f,0.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);

  // create objects
  camera = Camera2D::Make(0,10,0,10);

  auto trfSol = Transform::Make();
  trfSol->LoadIdentity();
  trfSol->Translate(5.0f, 5.0f, 1.0f);
  trfSol->Scale(3.0f, 3.0f, 1.0f);

  auto sol = Node::Make(trfSol, {Color::Make(1, 1, 0)}, {Circle::Make(60)});

  auto trfTerra = Transform::Make();
  trfTerra->Translate(1.0f, 1.0f, 0.0f);
  trfTerra->Scale(0.4f, 0.4f, 1.0f);

  auto terra = Node::Make(trfTerra, {Color::Make(0, 0, 1)}, {Circle::Make(60)});
  sol->AddNode(terra);

  auto trfLua = Transform::Make();
  trfLua->Translate(1.0f, 1.0f, 0.0f);
  trfLua->Scale(0.3f, 0.3f, 0.1f);

  auto lua = Node::Make(trfLua, {Color::Make(128, 128, 128)}, {Circle::Make(60)});
  terra->AddNode(lua);

  auto shader = Shader::Make();
  shader->AttachVertexShader("shaders/2d/vertex.glsl");
  shader->AttachFragmentShader("shaders/2d/fragment.glsl");
  shader->Link();

  // build scene
  auto root = Node::Make(shader, {sol});
  scene = Scene::Make(root);
  scene->AddEngine(MoveBody::Make(trfSol));
  scene->AddEngine(MoveBody::Make(trfTerra));
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
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

static void update (float dt)
{
  scene->Update(dt);
}

int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);       // required for mac os
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);  // option for mac os
#endif

    glfwSetErrorCallback(error);

    GLFWwindow* win = glfwCreateWindow(600, 400, "Window title", nullptr, nullptr);
    assert(win);
    glfwSetFramebufferSizeCallback(win, resize);  // resize callback
    glfwSetKeyCallback(win, keyboard);            // keyboard callback

    glfwMakeContextCurrent(win);
#ifdef __glad_h_
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD OpenGL context\n");
    exit(1);
   }
#endif
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  float t0 = float(glfwGetTime());
  while(!glfwWindowShouldClose(win)) {
    float t = float(glfwGetTime());
    update(t-t0);
    t0 = t;
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

