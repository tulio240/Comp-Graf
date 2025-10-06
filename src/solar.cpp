
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
#include "shape.h"
#include "texture.h"

#include <iostream>

# define M_PI           3.14159265358979323846  /* pi */

static ScenePtr scene;
static CameraPtr camera;

class Disk;
using DiskPtr = std::shared_ptr<Disk>;

class Disk : public Shape {
    unsigned int m_vao;
    unsigned int m_ebo;
    unsigned int num_points;

protected:
    Disk(int num_points) : num_points(num_points) {
        std::vector<float> coords;
        std::vector<unsigned int> indexes;

        float radius = 0.7f;

        // centro
        float x0 = 0.0f;
        float y0 = 0.0f;

        coords.insert(coords.end(), {x0, y0, 0.5f, 0.5f});

        for (int i = 0; i < num_points; i++) {
            double theta = 2 * M_PI * i / num_points;
            float xi = x0 + radius * std::cos(theta);
            float yi = y0 + radius * std::sin(theta);

            float u = 0.5f + 0.5f * std::cos(theta);
            float v = 0.5f + 0.5f * std::sin(theta);

            coords.insert(coords.end(), {xi, yi, u, v});

            unsigned int next = (i + 1) % num_points;

            indexes.push_back(0);
            indexes.push_back(i + 1);
            indexes.push_back(next + 1);
        }

        // Create VAO
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // Create VBO
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Create EBO
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), indexes.data(), GL_STATIC_DRAW);


        //Texture
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

public:
    static DiskPtr Make(int num_points) {
        return DiskPtr(new Disk(num_points));
    }

    virtual ~Disk() {}

    virtual void Draw(StatePtr) override {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, num_points * 3, GL_UNSIGNED_INT, 0);
    }
};

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
  glEnable(GL_DEPTH_TEST);

  // habilita blending para transparência
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // create objects
  camera = Camera2D::Make(0,10,0,10);

  auto trfSol = Transform::Make();
  trfSol->LoadIdentity();
  trfSol->Translate(5.0f, 5.0f, 0.0f);
  trfSol->Scale(3.0f, 3.0f, 1.0f);

  auto texSol = Texture::Make("face", "images/sun.png");

  auto sol = Node::Make(trfSol, {Color::Make(1, 1, 1), texSol}, {Disk::Make(60)});

  // órbita da terra (transparente, atrás)
  auto trf1 = Transform::Make();
  trf1->Translate(0.0f, 0.0f, -0.1f);
  auto terra_orbita = Node::Make(trf1, {}, {Disk::Make(60)});
  sol->AddNode(terra_orbita);

  auto trfTerra = Transform::Make();
  trfTerra->Translate(1.0f, 1.0f, 0.0f);
  trfTerra->Scale(0.4f, 0.4f, 1.0f);

  auto texTerra = Texture::Make("face", "images/earth.png");

  auto terra = Node::Make(trfTerra, {texTerra}, {Disk::Make(60)});
  terra_orbita->AddNode(terra);

  // órbita da lua (transparente, atrás)
  auto trf2 = Transform::Make();
  trf2->Translate(0.0f, 0.0f, -0.1f);
  auto lua_orbita = Node::Make(trf2, {}, {Disk::Make(60)});
  terra->AddNode(lua_orbita);

  auto trfLua = Transform::Make();
  trfLua->Translate(1.0f, 1.0f, 0.0f);
  trfLua->Scale(0.3f, 0.3f, 0.1f);

  auto texLua = Texture::Make("face", "images/moon.png");

  auto lua = Node::Make(trfLua, {texLua}, {Disk::Make(60)});
  lua_orbita->AddNode(lua);

  auto shader = Shader::Make();
  shader->AttachVertexShader("shaders/2d/vertex.glsl");
  shader->AttachFragmentShader("shaders/2d/fragment.glsl");
  shader->Link();

  // build scene
  auto root = Node::Make(shader, {sol});
  scene = Scene::Make(root);
  scene->AddEngine(MoveBody::Make(trf1));
  scene->AddEngine(MoveBody::Make(trf2));
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

