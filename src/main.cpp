#include "visual3D.h"
#include "camera3D.h"
#include <iostream>
using namespace openApp;

Camera3D* camera = nullptr;
Camera3D* texCamera = nullptr;

Vertex3D triangleVertices[] = {
  { { -1.f, -1.f, 0.f}, {0.f, 0.f}},
  { { 1.f, -1.f, 0.f}, {1.f, 0.f}},
  { { -1.f, 1.f, 0.f}, {0.f, 1.f}}
};

//Vertex3D squareVertices[] = {
//  { { -1.f, -1.f, 0.f}, {0.f, 0.f}},
//  { { 1.f, -1.f, 0.f},  {1.f, 0.f}},
//  { { -1.f, 1.f, 0.f},  {0.f, 1.f}},
//
//  { { 1.f, 1.f, 0.f},   {1.f, 1.f}},
//  { { -1.f, 1.f, 0.f},  {0.f, 1.f}},
//  { { 1.f, -1.f, 0.f},  {1.f, 0.f}},
//};

Vertex3D squareVertices[] = {
{{-14.216, 8, 0}, {0, 1}},
{{-14.216, -8, 0}, {0, 0}},
{{14.216, -8, 0}, {1, 0}},
{{14.216, 8, 0}, {1, 1}},
{{-14.216, 8, 0}, {0, 1}},
{{14.216, -8, 0}, {1, 0}},
};

Vertex3D cubeVertices[] = {
{{-1, 1, 1}, {0, 1}},
{{-1, -1, 1}, {0, 0}},
{{1, -1, 1}, {1, 0}},
{{1, 1, 1}, {1, 1}},
{{-1, 1, 1}, {0, 1}},
{{1, -1, 1}, {1, 0}},
{{1, 1, -1.0000001}, {0, 1}},
{{1, -1, -1.0000001}, {0, 0}},
{{-1, -1, -0.99999994}, {1, 0}},
{{-1, 1, -0.99999994}, {1, 1}},
{{1, 1, -1.0000001}, {0, 1}},
{{-1, -1, -0.99999994}, {1, 0}},
{{1, 0.99999994, -1}, {0, 1}},
{{1, -0.99999994, -1}, {0, 0}},
{{0.99999994, -0.99999994, 1}, {1, 0}},
{{0.99999994, 0.99999994, 1}, {1, 1}},
{{1, 0.99999994, -1}, {0, 1}},
{{0.99999994, -0.99999994, 1}, {1, 0}},
{{-0.99999994, 0.99999994, 1}, {0, 1}},
{{-0.99999994, -0.99999994, 1}, {0, 0}},
{{-1, -0.99999994, -1}, {1, 0}},
{{-1, 0.99999994, -1}, {1, 1}},
{{-0.99999994, 0.99999994, 1}, {0, 1}},
{{-1, -0.99999994, -1}, {1, 0}},
{{-0.99999994, 0.99999994, -1}, {0, 1}},
{{-0.99999994, 1, 1}, {0, 0}},
{{0.99999994, 1, 1}, {1, 0}},
{{0.99999994, 0.99999994, -1}, {1, 1}},
{{-0.99999994, 0.99999994, -1}, {0, 1}},
{{0.99999994, 1, 1}, {1, 0}},
{{-0.99999994, -1, 1}, {0, 1}},
{{-0.99999994, -0.99999994, -1}, {0, 0}},
{{0.99999994, -0.99999994, -1}, {1, 0}},
{{0.99999994, -1, 1}, {1, 1}},
{{-0.99999994, -1, 1}, {0, 1}},
{{0.99999994, -0.99999994, -1}, {1, 0}},
};


Mesh* triangleMesh = nullptr;
Mesh* squareMesh = nullptr;
Mesh* cubeMesh = nullptr;
Visual3D* visual1 = nullptr;
Visual3D* visual2 = nullptr;
Visual3D* visual3 = nullptr;

Material redColor = Material();


void progStart() {
  camera = new Camera3D(80, _SCREEN_ASPECT, 0.01f, 90.f, 1, { 0.2, 0.3, 0.3, 1 });
  UniqueType::addGlobalUniqueType(camera);
  Camera3D::setMainCamera(camera);

  texCamera = new Camera3D(80, _SCREEN_ASPECT, 0.01f, 90.f, 1, { 0, 0, 0, 0.1 });
  UniqueType::addGlobalUniqueType(texCamera);

  camera->position.z = 4;
  texCamera->position.z = camera->position.z;
  texCamera->setParent(camera);

  triangleMesh = new Mesh(triangleVertices, 3);
  Mesh::addGlobalMesh("triangle", triangleMesh);

  squareMesh = new Mesh(squareVertices, 6);
  Mesh::addGlobalMesh("square", squareMesh);

  cubeMesh = new Mesh(cubeVertices, 36);
  Mesh::addGlobalMesh("cube", cubeMesh);

  redColor.albedo = { 1, 0, 0, 1 };
  Material::addGlobalMaterial("redColor", &redColor);

  visual1 = new Visual3D(squareMesh, 1);
  visual2 = new Visual3D(triangleMesh, 1);
  visual3 = new Visual3D(cubeMesh, 1);


  visual1->position = Vector3{ -4, 0, -5.f };
  //visual1->scale = { 8, 4.5f, 1 };

  visual2->position = Vector3{ 5.0f, 1.0f, -2.f };
  visual2->material = Material::getGlobalMaterial("redColor");

  visual3->material = Material::getGlobalMaterial("redColor");
  visual3->position = Vector3{ 0, 1.0f, -2.f };
  //visual3->setParent(visual2);

  texCamera->saveRenderBuffer = true;
  visual1->srcMaterial.albedoTexture = &texCamera->savedRenderBuffer;
  
  
  //visual3D::addGlobalVisual3D(visual1);
  Visual3D::addGlobalVisual3D(visual2);
  Visual3D::addGlobalVisual3D(visual3);
}

void progUpdate() {
  camera->position.z += 0.5f * _DELTA_TIME;
  //visual2->rotation.z += 60.f * _DELTA_TIME;
  //visual2->scale += 0.1f * _DELTA_TIME;
  visual3->rotation.y += 30.f * _DELTA_TIME;
  visual3->rotation.x += 15.f * _DELTA_TIME;
}

void progEnd() {
  delete(camera);
  delete(texCamera);
  delete(visual1);
  delete(visual2);
  delete(visual3);
  delete(triangleMesh);
  delete(squareMesh);
  delete(cubeMesh);
}