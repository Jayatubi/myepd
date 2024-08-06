#pragma once

#include "core/basic_type/basic_type.h"
#include <Arduino.h>

template<typename T>
class Singleton {
public:
    static void createInstance();

    static void destroyInstance();

    static void renew();

    static void release();

    static T& instance();

    static T* checkInstance();

private:
    static T* _instance;
};

template<typename T>
T* Singleton<T>::_instance = nullptr;

template<typename T>
void Singleton<T>::createInstance() {
    if (_instance == nullptr) {
        _instance = new T();
    }
}

template<typename T>
void Singleton<T>::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

template<typename T>
T& Singleton<T>::instance() {
    assert(_instance);
    return *_instance;
}

template<typename T>
T* Singleton<T>::checkInstance() {
    return _instance;
}


template<typename T>
void Singleton<T>::renew() {
    if (_instance == nullptr) {
        _instance = new T();
    } else {
        _instance = new(_instance) T();
    }
}

template<typename T>
void Singleton<T>::release() {
    if (_instance != nullptr) {
        _instance->~T();
    }
}

