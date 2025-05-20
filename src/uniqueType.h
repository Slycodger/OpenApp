#pragma once
#include "staticList.h"
#include <map>

namespace openApp {
  class UniqueType {
    size_t uniqueTypeIndex;

  protected:
    virtual void copyTo(UniqueType* ptr) {}

    bool selfContained;

  public:
    virtual void uniqueTypeUpdate() {}
    virtual void uniqueTypeAddedToGlobals() {}
    virtual UniqueType* create() { return new UniqueType(); }
    UniqueType* createCopyOf() {
      UniqueType* t = create();
      t->selfContained = true;
      copyTo(t);
      return t;
    }

    void addedToGlobals() {
      uniqueTypeAddedToGlobals();
    }

    void update() {
      uniqueTypeUpdate();
    }

    UniqueType() : uniqueTypeIndex(-1), selfContained(false) {}
    UniqueType(bool sF) : uniqueTypeIndex(-1), selfContained(sF) {}
    virtual ~UniqueType() {}




    //Static stuff
    //--------------------------------------------------
    static StaticList<UniqueType*> globalUniqueTypeInstances;
    static std::map<std::string, UniqueType*> savedUniqueTypes;
    static size_t globalUniqueTypeCount;



    //--------------------------------------------------
    static bool addGlobalUniqueType(UniqueType* unique) {
      if (unique->uniqueTypeIndex < (size_t)-1)
        return false;
      unique->uniqueTypeIndex = globalUniqueTypeCount;
      unique->addedToGlobals();
      globalUniqueTypeCount++;
      globalUniqueTypeInstances.addItem(unique);
      return true;
    }



    //--------------------------------------------------
    static bool removeGlobalUniqueType(UniqueType* unique) {
      size_t i = unique->uniqueTypeIndex;
      if (i >= (size_t)-1)
        return false;

      unique->uniqueTypeIndex = -1;
      globalUniqueTypeCount--;
      globalUniqueTypeInstances.removeAt(i);
      return true;
    }



    //--------------------------------------------------
    static void updateUniqueTypeInstances() {
      for (UniqueType** uP : UniqueType::globalUniqueTypeInstances) {
        (*uP)->update();
      }
    }



    //--------------------------------------------------
    static bool saveUniqueType(std::string name, UniqueType* ptr) {
      if (savedUniqueTypes.contains(name))
        return false;
      UniqueType* nP = ptr->createCopyOf();
      savedUniqueTypes.insert({ name, nP });
      return true;
    }



    //--------------------------------------------------
    static bool removeUniqueType(std::string name) {
      if (!savedUniqueTypes.contains(name))
        return false;
      return savedUniqueTypes.erase(name);
    }



    //--------------------------------------------------
    static UniqueType* getSavedUniqueType(std::string name) {
      if (!savedUniqueTypes.contains(name))
        return nullptr;
      return savedUniqueTypes[name];
    }


    //--------------------------------------------------
    static void end() {
      UniqueType::globalUniqueTypeInstances.clear();
      for (std::pair<std::string, UniqueType*> p : savedUniqueTypes) {
        if (p.second)
          delete(p.second);
      }
      savedUniqueTypes.clear();
    }
  };
}