#pragma once
#include "staticList.h"

namespace openApp {
  class UniqueType {
    size_t index;

  public:
    virtual void uniqueTypeUpdate() {}
    virtual void uniqueTypeAddedToGlobals() {}

    void addedToGlobals() {
      uniqueTypeAddedToGlobals();
    }

    void update() {
      uniqueTypeUpdate();
    }

    UniqueType() : index(-1) {}




    //Static stuff
    //--------------------------------------------------
    static StaticList<UniqueType*> globalUniqueTypeInstances;
    static size_t globalUniqueTypeCount;



    //--------------------------------------------------
    static bool addGlobalUniqueType(UniqueType* unique) {
      if (unique->index < (size_t)-1)
        return false;
      unique->index = globalUniqueTypeCount;
      unique->addedToGlobals();
      globalUniqueTypeCount++;
      globalUniqueTypeInstances.addItem(unique);
      return true;
    }



    //--------------------------------------------------
    static bool removeGlobalUniqueType(UniqueType* unique) {
      size_t i = unique->index;
      if (i >= (size_t)-1)
        return false;

      unique->index = -1;
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
    static void end() {
      UniqueType::globalUniqueTypeInstances.clear();
    }
  };
}