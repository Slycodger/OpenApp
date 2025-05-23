#pragma once
#include "staticList.h"
#include <map>

namespace openApp {
  class UniqueType {
    size_t uniqueTypeIndex;

  protected:
    virtual void copyTo(UniqueType* ptr) {}
    virtual void uniqueTypeUpdate() {}
    virtual void uniqueTypeAddedToGlobals() {}
    virtual void uniqueTypeRemovedFromGlobals() {}
    virtual void uniqueTypeSetParent(UniqueType* ptr) {}
    virtual void uniqueTypeAddedChild(UniqueType* ptr) {}
    virtual void uniqueTypeRemovingChild(size_t index) {}



    size_t childIndex;
    UniqueType* parent;

    bool selfContained;

  public:
    StaticList<UniqueType*> children;


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


    void setParent(UniqueType* p) {
      p->addChild(this);
    }

    UniqueType* getParent() {
      return parent;
    }

    void removeParent() {
      if (!parent)
        return;
      parent->uniqueTypeRemovingChild(childIndex);
      parent->children.removeAt(childIndex);
      childIndex = -1;
      parent = nullptr;
      uniqueTypeSetParent(nullptr);
    }

    void addChild(UniqueType* c) {
      if (c->parent)
        c->removeParent();
      c->childIndex = children.addItem(c);
      if (c->childIndex >= (size_t)-1)
        return;
      c->parent = this;
      if (uniqueTypeIndex < (size_t)-1)
        UniqueType::addGlobalUniqueTypeTree(c);
      else
        UniqueType::removeGlobalUniqueType(c);
      c->uniqueTypeSetParent(this);
      uniqueTypeAddedChild(c);
    }

    void removeChild(size_t& visual3DIndex) {
      UniqueType* c = *children[visual3DIndex];
      if (!c)
        return;
      c->removeParent();
    }

    bool hasParent() {
      return parent;
    }



    UniqueType() : uniqueTypeIndex(-1), selfContained(false), parent(0), childIndex(-1), children() {}
    UniqueType(bool sF) : uniqueTypeIndex(-1), parent(0), children(), childIndex(-1), selfContained(sF) {}
    virtual ~UniqueType() {
      if (selfContained) {
        for (UniqueType** child : children) {
          if (*child)
            delete(*child);
        }
        children.clear();
      }
    }




    //Static stuff
    //--------------------------------------------------
    static StaticList<UniqueType*> globalUniqueTypeInstances;
    static std::map<std::string, UniqueType*> savedUniqueTypes;
    static size_t globalUniqueTypeCount;



    //--------------------------------------------------
    static bool addGlobalUniqueTypeTree(UniqueType* unique) {
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
      for (UniqueType** c : unique->children)
        removeGlobalUniqueType(*c);
      unique->uniqueTypeRemovedFromGlobals();
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