#include "application.h"
#include "module/gfx.h"
#include "module/console.h"


template<typename ... ClassTypes>
class Modules {
 public:
  Modules() {
  }

  ~Modules() {
  }
};

template<typename FirstType, typename ... RestTypes>
class Modules<FirstType, RestTypes ...> : public Modules<RestTypes ...> {
 public:
  Modules() {
    FirstType::Release();
  }

  ~Modules() {
    FirstType::Renew();
  }
};


void Application::Bootstrap() {
  Modules<
    GFX,
    Console
  >();
}

void Application::Teardown() {

}

void Application::Tick() {

}
