#include "visual3D.h"
#include "camera3D.h"
#include <iostream>
#include "texture.h"
#include "modelLoading.h"
#include "sound.h"
#include "visual2D.h"
#include "cameraViewer.h"
#include "font.h"
using namespace openApp;

Camera3D* camera = nullptr;
Camera3D* texCamera = nullptr;

Camera2D* cam2D = nullptr;

Visual2D* visual1 = nullptr;
Visual2D* visual12 = nullptr;
Visual3D* visual2 = nullptr;
Transform3D* loaded = nullptr;
Visual3D* visual3 = nullptr;

Material redColor = Material();
Material imgMat = Material();
Texture* tex;
Texture* fT = nullptr;
CameraViewer* camViewer;
Font* fo = nullptr;

void progStart() {
  camera = new Camera3D(80, program::SCREEN_ASPECT, 0.01f, 90.f, 1, _SCREEN_SIZE, {0.2, 0.3, 0.3, 1});

  camViewer = new CameraViewer(1);
  UniqueType::addGlobalUniqueTypeTree(camViewer);

  cam2D = new Camera2D(1, 1, 1, _SCREEN_SIZE, { 0, 0, 0, 0 });
  cam2D->updateTextureFiltering(GL_NEAREST);
  UniqueType::addGlobalUniqueTypeTree(cam2D);
  Camera2D::setMainCamera2D(cam2D);

  camera->position.z = 9;
  camera->setParent(camViewer);


  redColor.albedo = { 1, 0, 0, 1 };
  Material::addGlobalMaterial("redColor", &redColor);


  int ret = 0;
  Image* img = image::loadImage("./images/45.png", true, GL_RGBA, 0, ret);
  img->buffer;
  if (!ret) {
    tex = new Texture(img, GL_LINEAR, GL_RGBA, GL_UNSIGNED_BYTE);
    imgMat.albedoTexture = tex;
  }
  else
    imgMat.albedo = Vector4(1, 1, 1, 1);

  visual1 = new Visual2D(Mesh2D::getGlobalMesh2D("square"), 1 , 1);
  visual12 = new Visual2D(Mesh2D::getGlobalMesh2D("triangle"), 6, 1);
  visual2 = new Visual3D(Mesh3D::getGlobalMesh3D("triangle"), 1);
  visual3 = new Visual3D(Mesh3D::getGlobalMesh3D("cube"), 1);

  visual1->srcMaterial.albedo = {1, 1, 1, 0};


  visual1->scale = {1, 1};
  visual12->scale = { 0.25f, 0.25f };
  visual12->material = Material::getGlobalMaterial("redColor");
  visual12->position = {-0.5f, 0.25f};

  visual2->position = Vector3{ 5.0f, 1.0f, -2.f };
  visual2->material = Material::getGlobalMaterial("redColor");

  visual3->material = &imgMat;
  visual3->position = Vector3{ 0, 4.0f, -2.f };
  //visual3->setParent(visual2);

  //texCamera->renderBufferSaving(true);
  fo = font::loadFont("./fonts/Arial.ttf", "arial");
  fo->setFontSize(12);
  fT = font::createTextTexture("|hello,\n World jjjjjjjjjjjjjjjjjjaeeeeeeeefhhhhhhhhhhhhhhhhhkkkkkkkkkkkkkkkkkkkkkkkk", fo, {1, 1, 0, 1}, 1080, 1, 128, 1.2, true, 0.25);
  visual1->srcMaterial.albedoTexture = fT;
  
  loaded = dynamic_cast<Transform3D*>(modelLoading::loadModel("./models/model.fbx"));
  if (!loaded) {
    std::cout << "failed to load model\n";
  }
  else {
    Visual3D::addGlobalVisual3DTree(loaded);
  }

  Visual2D::addGlobalVisual2DTree(visual1);
  Visual2D::addGlobalVisual2DTree(visual12);
  Visual3D::addGlobalVisual3DTree(visual2);
  Visual3D::addGlobalVisual3DTree(visual3);
}


bool doom = false;
int dings = 0;
float angle = 0;

void progUpdate() {
  visual3->rotation.y += 30.f * program::DELTA_TIME;
  visual3->rotation.x += 15.f * program::DELTA_TIME;

  camera->position.x = 9 * sin(angle * _degToRadF);
  camera->position.z = 9 * cos(angle * _degToRadF);
  camera->rotation.y = -angle;

  if (program::APP_TIME > 2.0f) {
    angle += 25.f * program::DELTA_TIME;
    loaded->rotation.x += 45.f * program::DELTA_TIME;
    UIVector2 s = cam2D->getSize();
    unsigned int fS = fo->getFontSize();
    if (s.x > 300)
      cam2D->resize(s * 0.99f, true);

    if (fS < 128) {
      fS++;
      delete(fT);
      fo->setFontSize(fS);
      fT = font::createTextTexture("|hello,\n World jjjjjjjjjjjjjjjjjjaeeeeeeeefhhhhhhhhhhhhhhhhhkkkkkkkkkkkkkkkkkkkkkkkk", fo, {1, 1, 0, 1}, 1080, 1, 128, 1.2, true, 0.25);
      visual1->srcMaterial.albedoTexture = fT;
    }

    if (dings < 250) {
      sound::queueSound("./sounds/ding.wav");
      dings++;
    }
    if (!doom) {
      sound::playMusic("./sounds/doom.mp3");
      doom = true;

    }
  }
}

void progEnd() {
  delete(camera);
  if (tex);
    delete(tex);
  delete(texCamera);
  delete(visual1);
  delete(visual2);
  delete(visual3);
  delete(visual12);
  delete(fT);
}