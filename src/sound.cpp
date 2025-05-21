#include "SFML/Audio.hpp"
#include <queue>
#include <vector>
#include "sound.h"

std::queue<std::string> queuedSounds = std::queue<std::string>();

struct bufferSoundCombo {
  sf::SoundBuffer buffer;
  sf::Sound sound;
  bufferSoundCombo() {
    sound.setBuffer(buffer);
  }
};

const unsigned int maxSounds = 250;
const unsigned int maxMusic = 16;
unsigned int soundCount = 0;
unsigned int musicCount = 0;

bufferSoundCombo** soundPlayers = new bufferSoundCombo*[maxSounds];
sf::Music** musicPlayers = new sf::Music*[maxMusic];

namespace openApp {
  namespace sound {

    void start() {
      for (int i = 0; i < maxSounds; i++)
        soundPlayers[i] = nullptr;
      for (int i = 0; i < maxMusic; i++)
        musicPlayers[i] = nullptr;
    }

    void end() {
      for (int i = 0; i < maxSounds; i++)
        if (soundPlayers[i])
          delete (soundPlayers[i]);
      delete[](soundPlayers);
      for (int i = 0; i < maxMusic; i++)
        if (musicPlayers[i])
          delete(musicPlayers[i]);
      delete[](musicPlayers);
    }

    void queueSound(std::string path) {
      queuedSounds.push(path);
    }

    int playMusic(std::string path) {
      int i = soundCount;
      if (i >= maxMusic)
        return -1;

      sf::Music* music = new sf::Music();
      if (!music->openFromFile(path)) {
        delete(music);
        return -1;
      }
      music->play();
      musicPlayers[i] = music;
      musicCount++;
      return i;
    }

    void pauseMusic(int index) {
      if (index >= 0 && index < musicCount && musicPlayers[index] && musicPlayers[index]->getStatus() == sf::SoundStream::Playing)
        musicPlayers[index]->pause();
    }
    void resumeMusic(int index) {
      if (index >= 0 && index < musicCount && musicPlayers[index] && musicPlayers[index]->getStatus() == sf::SoundStream::Paused)
        musicPlayers[index]->play();
    }
    void stopMusic(int index) {
      if (index >= 0 && index < musicCount && musicPlayers[index] && musicPlayers[index]->getStatus() != sf::SoundStream::Stopped)
        musicPlayers[index]->stop();
    }

    void update() {
      for (int i = 0; i < maxSounds; i++) {
        if (!soundPlayers[i]) {
          if (soundCount < maxSounds && !queuedSounds.empty()) {
            soundPlayers[i] = new bufferSoundCombo();
            soundCount++;
          } else
            continue;
        }

        bufferSoundCombo* s = soundPlayers[i];
        if (s->sound.getStatus() == sf::SoundStream::Playing)
          continue;
        if (!queuedSounds.empty()) {
          if (s->buffer.loadFromFile(queuedSounds.front()))
            s->sound.play();
          queuedSounds.pop();
        } else {
          delete(soundPlayers[i]);
          soundPlayers[i] = nullptr;
        }
      }
      //for (; !queuedSounds.empty(); queuedSounds.pop()) {
      //  if (soundCount <= maxSounds)
      //    break;
      //  bufferSoundCombo* c = new bufferSoundCombo();
      //  if (!c->buffer.loadFromFile(queuedSounds.front())) {
      //    delete(c);
      //    continue;
      //  }
      //  c->sound.play();
      //  soundPlayers[soundCount] = c;
      //  soundCount++;
      //}

      for (int i = 0; i < musicCount; i++) {
        if (musicPlayers[i] && musicPlayers[i]->getStatus() == sf::SoundStream::Stopped) {
          delete(musicPlayers[i]);
          musicPlayers[i] = nullptr;
        }
      }
    }
  }
}