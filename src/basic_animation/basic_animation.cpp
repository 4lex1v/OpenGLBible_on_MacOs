#include <math.h>

#include "sb7.h"

class basic_animation : public sb7::application {
public:
  void render (double current_time) {
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.5f };
    const GLfloat color[] = {
      (float) sin(current_time) * 0.5f + 0.5f,
      (float) cos(current_time) * 0.5f + 0.5f,
      0,
      1.0f
    };
      
    glClearBufferfv(GL_COLOR, 0, color);
  }
};

DECLARE_MAIN(basic_animation)
