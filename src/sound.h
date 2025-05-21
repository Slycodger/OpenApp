#pragma once

namespace openApp {
  namespace sound {
    void start();
    void end();
    void queueSound(std::string path);
    int playMusic(std::string path);
    void pauseMusic(int index);
    void resumeMusic(int index);
    void stopMusic(int index);
    void update();
  }
}