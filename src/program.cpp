#include "openglBasics.h"
#include "shaderHandling.h"
#include "exceptionHandling.h"
#include "vector.h"
#include "camera3D.h"
#include "camera2D.h"
#include "program.h"
#include "visual3D.h"
#include "visual2D.h"
#include <chrono>
#include "image.h"
#include "modelLoading.h"
#include "sound.h"

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
unsigned int shaderMaterialUBO = 0;

Shader textureShader;
Shader shader2D;
Shader shader3D;


Mesh3D* triangleMesh3D = nullptr;
Mesh3D* squareMesh3D = nullptr;
Mesh3D* cubeMesh3D = nullptr;

Mesh2D* triangleMesh2D = nullptr;
Mesh2D* squareMesh2D = nullptr;


Vertex3D triangleVertices3D[] = {
  { { -1.f, -1.f, 0.f}, {0.f, 0.f}},
  { { 1.f, -1.f, 0.f}, {1.f, 0.f}},
  { { -1.f, 1.f, 0.f}, {0.f, 1.f}}
};

Vertex3D squareVertices3D[] = {
{{-1, 1, 0}, {0, 1}},
{{-1, -1, 0}, {0, 0}},
{{1, -1, 0}, {1, 0}},
{{1, 1, 0}, {1, 1}},
{{-1, 1, 0}, {0, 1}},
{{1, -1, 0}, {1, 0}},
};

Vertex3D cubeVertices3D[] = {
{{-1, 1, 1}, {0, 1}},
{{-1, -1, 1}, {0, 0}},
{{1, -1, 1}, {1, 0}},
{{1, 1, 1}, {1, 1}},
{{-1, 1, 1}, {0, 1}},
{{1, -1, 1}, {1, 0}},
{{1, 1, -1}, {0, 1}},
{{1, -1, -1}, {0, 0}},
{{-1, -1, -1}, {1, 0}},
{{-1, 1, -1}, {1, 1}},
{{1, 1, -1}, {0, 1}},
{{-1, -1, -1}, {1, 0}},
{{1, 1, 1}, {0, 1}},
{{1, -1, 1}, {0, 0}},
{{1, -1, -1}, {1, 0}},
{{1, 1, -1}, {1, 1}},
{{1, 1, 1}, {0, 1}},
{{1, -1, -1}, {1, 0}},
{{-1, 1, -1}, {0, 1}},
{{-1, -1, -1}, {0, 0}},
{{-1, -1, 1}, {1, 0}},
{{-1, 1, 1}, {1, 1}},
{{-1, 1, -1}, {0, 1}},
{{-1, -1, 1}, {1, 0}},
{{-1, 1, -1}, {0, 1}},
{{-1, 1, 1}, {0, 0}},
{{1, 1, 1}, {1, 0}},
{{1, 1, -1}, {1, 1}},
{{-1, 1, -1}, {0, 1}},
{{1, 1, 1}, {1, 0}},
{{-1, -1, 1}, {0, 1}},
{{-1, -1, -1}, {0, 0}},
{{1, -1, -1}, {1, 0}},
{{1, -1, 1}, {1, 1}},
{{-1, -1, 1}, {0, 1}},
{{1, -1, -1}, {1, 0}},
};



Vertex2D triangleVertices2D[] = {
  { { -1.f, -1.f }, {0.f, 0.f}},
  { { 1.f, -1.f }, {1.f, 0.f}},
  { { -1.f, 1.f }, {0.f, 1.f}}
};

Vertex2D squareVertices2D[] = {
{{-1, 1}, {0, 1}},
{{-1, -1}, {0, 0}},
{{1, -1}, {1, 0}},
{{1, 1}, {1, 1}},
{{-1, 1}, {0, 1}},
{{1, -1}, {1, 0}},
};


namespace openApp {
  namespace program {

    unsigned int getShaderMaterialUBO() {
      return shaderMaterialUBO;
    }
    Shader getShader2D() {
      return shader2D;
    }
    Shader getShader3D() {
      return shader3D;
    }
    unsigned int getGlobalVAO() {
      return VAO;
    }

  }
}

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
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

  const char* textureVertShader[] = {
  "./shaders/viewportShader.vert",
  };
  const char* textureFragShader[] = {
  "./shaders/textureShader.frag"
  };
  ShaderPair textureShaderPair[2] = { ShaderPair{textureVertShader, 1, GL_VERTEX_SHADER}, ShaderPair(textureFragShader, 1, GL_FRAGMENT_SHADER) };
  textureShader = Shader::createShader("textureShader", textureShaderPair, 2);


  const char* vertShader2D[] = {
"./shaders/basic2DShader.vert"
  };
  const char* fragShader2D[] = {
  "./shaders/basicShader.frag"
  };
  ShaderPair shaderPair2D[2] = {ShaderPair{vertShader2D, 1, GL_VERTEX_SHADER}, ShaderPair(fragShader2D, 1, GL_FRAGMENT_SHADER)};
  shader2D = Shader::createShader("visual2DShader", shaderPair2D, 2);


  const char* vertShader3D[] = {
"./shaders/basic3DShader.vert"
  };
  const char* fragShader3D[] = {
  "./shaders/basicShader.frag"
  };
  ShaderPair shaderPair3D[2] = {ShaderPair{vertShader3D, 1, GL_VERTEX_SHADER}, ShaderPair(fragShader3D, 1, GL_FRAGMENT_SHADER)};
  shader3D = Shader::createShader("visual3DShader", shaderPair3D, 2);


  glCreateBuffers(1, &shaderMaterialUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, shaderMaterialUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(Material::ShaderMaterial), 0, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, shaderMaterialUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);




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



  triangleMesh3D = new Mesh3D(triangleVertices3D, 3);
  Mesh3D::addGlobalMesh3D("triangle", triangleMesh3D);

  squareMesh3D = new Mesh3D(squareVertices3D, 6);
  Mesh3D::addGlobalMesh3D("square", squareMesh3D);

  cubeMesh3D = new Mesh3D(cubeVertices3D, 36);
  Mesh3D::addGlobalMesh3D("cube", cubeMesh3D);


  triangleMesh2D = new Mesh2D(triangleVertices2D, 3);
  Mesh2D::addGlobalMesh2D("triangle", triangleMesh2D);

  squareMesh2D = new Mesh2D(squareVertices2D, 6);
  Mesh2D::addGlobalMesh2D("square", squareMesh2D);



  sound::start();
  progStart();

 

  while (!glfwWindowShouldClose(window)) {
    double start = glfwGetTime();
    _APP_TIME = glfwGetTime();
    glfwPollEvents();

    UniqueType::updateUniqueTypeInstances();
    Visual3D::drawVisual3DInstances();
    Visual2D::drawVisual2DInstances();
    sound::update();
    progUpdate();

    defaultView();
    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDepthMask(GL_FALSE);

    textureShader.active();
    Camera3D* mainCam3D = Camera3D::getMainCamera3D();
    if (mainCam3D) {

      glBindTexture(GL_TEXTURE_2D, mainCam3D->renderBuffer);
      glActiveTexture(GL_TEXTURE0);

      glUniform1i(glGetUniformLocation(textureShader, "texTarget"), 0);
      drawScreenQuad();
    }

    Camera2D* mainCam2D = Camera2D::getMainCamera2D();
    if (mainCam2D) {

      glBindTexture(GL_TEXTURE_2D, mainCam2D->renderBuffer);
      glActiveTexture(GL_TEXTURE0);

      glUniform1i(glGetUniformLocation(textureShader, "texTarget"), 0);
      drawScreenQuad();
    }

    glDepthMask(GL_TRUE);

    glfwSwapBuffers(window);

    _DELTA_TIME = glfwGetTime() - start;
  }

  progEnd();
  Visual3D::end();
  Visual2D::end();
  UniqueType::end();
  Mesh3D::end();
  Mesh2D::end();
  Material::end();
  image::end();
  modelLoading::end();
  sound::end();

  delete(triangleMesh3D);
  delete(squareMesh3D);
  delete(cubeMesh3D);
  delete(triangleMesh2D);
  delete(squareMesh2D);
}



namespace openApp {
  namespace program {
    void drawScreenQuad() {
      textureShader.active();
      glBindVertexArray(squareVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }

    void defaultView() {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glStencilFunc(GL_ALWAYS, 0, 0);
      glViewport(0, 0, _SCREEN_SIZE.x, _SCREEN_SIZE.y);
    }
  }
}




std::ostream& openApp::operator<<(std::ostream& stream, const Vector3& vec) {
  stream << vec.x << ", " << vec.y << ", " << vec.z;
  return stream;
}