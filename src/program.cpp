#include "openglBasics.h"
#include "shaderHandling.h"
#include "exceptionHandling.h"
#include "vector.h"
#include "camera3D.h"
#include "program.h"
#include "visual3D.h"
#include <chrono>

void progStart();
void progUpdate();
void progEnd();

using namespace openApp;
using namespace openApp::program;

float triPoints[] = {
  -1.f, 1.f,
  -1.f, -1.f,
  1.f, -1.f
};

float squarePoints[] = {
  -1.f, 1.f,      0.f, 1.f,
  -1.f, -1.f,     0.f, 0.f,
  1.f, -1.f,      1.f, 0.f,

  1.f, 1.f,       1.f, 1.f,
  -1.f, 1.f,      0.f, 1.f,
  1.f, -1.f,      1.f, 0.f
};

unsigned int triVAO = 0, triVBO = 0;
unsigned int squareVAO = 0, squareVBO = 0;

UIVector2 _SCREEN_SIZE = { 1280, 720 };
float _SCREEN_ASPECT = 1280.f / 720.f;
double _DELTA_TIME = 0;
double _APP_TIME = 0;

unsigned int VAO = 0;
unsigned int* Program::globalVAO = &VAO;

Shader textureShader;


int main() {
  exceptionsStart();


  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow* window = glfwCreateWindow(_SCREEN_SIZE.x, _SCREEN_SIZE.y, "OpenApp", 0, 0);
  glfwMakeContextCurrent(window);


  glfwSwapInterval(0);

  if (!gladLoadGL())
    return -1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

  Visual3D::start();

  const char* vertShader[] = {
    "./shaders/basicShader.vert"
  };
  const char* fragShader[] = {
  "./shaders/basicShader.frag"
  };
  ShaderPair shaderPair[2] = { ShaderPair{vertShader, 1, GL_VERTEX_SHADER}, ShaderPair(fragShader, 1, GL_FRAGMENT_SHADER) };
  Shader shader = shader::createShader("basicShader", shaderPair, 2);


  const char* textureVertShader[] = {
  "./shaders/viewportShader.vert",
  };
  const char* textureFragShader[] = {
  "./shaders/textureShader.frag"
  };
  ShaderPair textureShaderPair[2] = { ShaderPair{textureVertShader, 1, GL_VERTEX_SHADER}, ShaderPair(textureFragShader, 1, GL_FRAGMENT_SHADER) };
  textureShader = shader::createShader("textureShader", textureShaderPair, 2);


  glGenVertexArrays(1, &squareVAO);
  glBindVertexArray(squareVAO);
  glGenBuffers(1, &squareVBO);
  glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(squarePoints), squarePoints, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);



  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);


  glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
  glVertexAttribBinding(0, 0);
  glVertexAttribFormat(1, 2, GL_FLOAT, false, 3 * sizeof(float));
  glVertexAttribBinding(1, 0);

  glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
  glVertexAttribBinding(2, 1);
  glVertexAttribFormat(3, 2, GL_FLOAT, false, 2 * sizeof(float));
  glVertexAttribBinding(3, 1);

  glBindVertexArray(0);


  progStart();

 

  while (!glfwWindowShouldClose(window)) {
    double start = glfwGetTime();
    _APP_TIME = glfwGetTime();
    glfwPollEvents();

    UniqueType::updateUniqueTypeInstances();
    Visual3D::drawVisual3DInstances();
    progUpdate();

    defaultView();
    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    Camera3D* mainCam = Camera3D::getMainCamera();
    if (mainCam) {
      textureShader.active();

      glBindTexture(GL_TEXTURE_2D, Camera3D::getMainCamera()->renderBuffer);
      glActiveTexture(GL_TEXTURE0);

      glUniform1i(glGetUniformLocation(textureShader, "texTarget"), 0);
      drawSquare();
    }


    glfwSwapBuffers(window);

    _DELTA_TIME = glfwGetTime() - start;
  }

  progEnd();
  Visual3D::end();
  UniqueType::end();
  Mesh::end();
  Material::end();
}



namespace openApp {
  namespace program {
    void drawSquare() {
      textureShader.active();
      glBindVertexArray(squareVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }

    void defaultView() {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glStencilFunc(GL_ALWAYS, 0, 0);
    }
  }
}




std::ostream& openApp::operator<<(std::ostream& stream, const Vector3& vec) {
  stream << vec.x << ", " << vec.y << ", " << vec.z;
  return stream;
}