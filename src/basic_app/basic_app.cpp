#include "sb7.h"

class basic_app : public sb7::application {
public:
  void render (double current_time) {
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.5f };
    glClearBufferfv(GL_COLOR, 0, red);
  }
};

DECLARE_MAIN(basic_app)
