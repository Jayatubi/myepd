#pragma once

#include <cassert>

template<typename T>
class Singleton {
 public:
  static void CreateInstance();

  static void DestroyInstance();

  static void Renew();

  static void Release();

  static T &GetInstance();

 private:
  static T *instance;
};

template<typename T>
T *Singleton<T>::instance = nullptr;

template<typename T>
void Singleton<T>::CreateInstance() {
  if (instance == nullptr) {
    instance = new T();
  }
}

template<typename T>
void Singleton<T>::DestroyInstance() {
  if (instance != nullptr) {
    delete instance;
    instance = nullptr;
  }
}

template<typename T>
T &Singleton<T>::GetInstance() {
  assert(instance);
  return *instance;
}

template<typename T>
void Singleton<T>::Renew() {
  if (instance == nullptr) {
    instance = new T();
  } else {
    instance = new(instance) T();
  }
}

template<typename T>
void Singleton<T>::Release() {
  if (instance != nullptr) {
    instance->~T();
  }
}

