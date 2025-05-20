#pragma once
#include "exceptionHandling.h"
#include <iterator>
#include <cstddef>

namespace openApp {
  template<typename T>
  class StaticList {
    T** internal;
    size_t bufferSize;
    size_t _size;
    size_t farthest;

    size_t getOpenArea(size_t size, bool& ref) {
      size_t curCount = 0;
      size_t lenStart = 0;
      for (size_t i = 0; i < bufferSize; i++) {
        if (!internal[i])
          curCount++;
        else {
          lenStart = i + 1;
          curCount = 0;
        }
        if (curCount >= size) {
          ref = true;
          return lenStart;
        }
      }

      ref = false;
      return 0;
    }

  public:
    struct StaticListIterator {
      using difference_type = std::ptrdiff_t;
      using value_type = T*;

      value_type* ptr;
      value_type* end;

      value_type operator*() const {
        return *ptr;
      }


      //prefix
      StaticListIterator& operator++() {
        ptr++;
        while (!*ptr && ptr < end)
          ptr++;
        return *this;
      }

      //postfix
      StaticListIterator operator++(int) {
        StaticListIterator tmp = *this;
        ++*this;
        return tmp;
      }

      bool operator ==(const StaticListIterator& it) const {
        return ptr == it.ptr;
      }

      StaticListIterator() : ptr(0), end(0) {}
      StaticListIterator(value_type* p) : ptr(p), end(0) {}
      StaticListIterator(value_type* p, value_type* e) : ptr(p), end(e) {}
    };
    static_assert(std::forward_iterator<StaticListIterator>);


    StaticListIterator begin() {
      return StaticListIterator(internal, internal + farthest + 1);
    }
    StaticListIterator end() {
      return StaticListIterator(internal + farthest + 1, internal + farthest + 1);
    }

    size_t size() {
      return _size;
    }

    void clear() {
      for (size_t i = 0; i < bufferSize; i++) {
        if (internal[i])
          delete(internal[i]);
      }
      delete[](internal);
      internal = 0;
      bufferSize = 0;
      _size = 0;
      farthest = 0;
    }

    size_t addItem(T t) {
      bool ref = false;
      size_t index = getOpenArea(1, ref);
      if (!ref) {
        if (bufferSize >= 1)
          resize(bufferSize * 2);
        else
          resize(1);

        index = getOpenArea(1, ref);
        if (!ref)
          return -1;
      }
       if (index > farthest)
        farthest = index;
      internal[index] = new T();
      *internal[index] = t;
      _size++;
      return index;
    }
    size_t addItemRange(T* t, size_t len) {
      if (!t) {
        openApp::logError("NULL FIELD", "openApp::StaticList::addItemRange", "t", "cannot be null");
        return -1;
      }

      bool ref = false;
      size_t index = getOpenArea(len, ref);
      if (!ref) {
        if (bufferSize > 1)
          resize(bufferSize + len);
        else
          resize(1);

        index = getOpenArea(len, ref);
        if (!ref)
          return -1;
      }
      if (index + len - 1 > farthest)
        farthest = index + len - 1;
      for (size_t i = 0; i < len; i++) {
        internal[index + i] = new T();
        *internal[index + i] = t[i];
      }
      _size += len;
      return index;
    }

    bool removeAt(size_t index) {
      if (index >= bufferSize) {
        openApp::logError("INDEX OUT-OF-RANGE", "openApp::StaticList::removeAt", "index", "index past buffered range");
        return false;
      }

      delete(internal[index]);
      internal[index] = nullptr;
      _size--;
      return true;
    }

    bool removeRange(size_t index, size_t count) {
      if (index >= bufferSize || index + count >= bufferSize) {
        openApp::logError("INDEX OUT-OF-RANGE", "openApp::StaticList::removeRange", "index - count", "start index or start index with count past buffered range");
        return false;
      }

      for (size_t i = index; i < index + count; i++) {
        delete(internal[i]);
        internal[i] = nullptr;
      }
      _size -= count;
      return true;
    }

    void resize(size_t newSize) {
      T** newLoc = new T * [newSize];

      for (size_t i = 0; i < newSize; i++) {
        if (i < bufferSize)
          newLoc[i] = internal[i];
        else
          newLoc[i] = nullptr;
      }

      for (size_t i = newSize; i < bufferSize; i++) {
        if (internal[i]) {
          delete(internal[i]);
          _size--;
        }
      }
      bufferSize = newSize;
      delete[](internal);
      internal = newLoc;
    }

    void shrinkToFit() {
      size_t farthest = 0;
      for (size_t i = 0; i < bufferSize; i++)
        if (internal[i])
          farthest = i;
      resize(farthest);
    }

    T* operator[](size_t i) {
      if (i >= bufferSize)
        return nullptr;
      return internal[i];
    }

    StaticList() : internal(0), bufferSize(0), _size(0), farthest(0) {}
    StaticList(size_t buffer) : internal(new T* [buffer] {0}), bufferSize(buffer), _size(0), farthest(0) {}

    ~StaticList() {
      for (size_t i = 0; i < bufferSize; i++) {
        if (internal[i])
          delete(internal[i]);
      }
      delete[](internal);
    }
  };
}
