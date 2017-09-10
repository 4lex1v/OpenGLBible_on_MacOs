#include <math.h>

#include "sb7.h"

class basic_shaders : public sb7::application {
  
private:
  GLuint rendering_program;
  GLuint vertex_array_object;
  
public:
  GLuint compile_shaders (void) {
    GLuint vertex_shader;
    GLuint fragment_shader; 
    GLuint program; 
    
    // #NOTE(4lex1v) :: sadly my mac only supports OpenGL 4.1
    static const GLchar* vertex_shader_source[] = {
      "#version 410 core                         \n"
      "                                          \n"
      "void main (void) {                        \n"
      "  gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \n"
      "}                                         \n"
    };
    
    // #NOTE(4lex1v) :: sadly my mac only supports OpenGL 4.1
    static const GLchar * fragment_shader_source[] = {
      "#version 410 core                  \n"
      "                                   \n"
      "out vec4 color;                    \n"
      "void main(void) {                  \n"
      "  color = vec4(0.0, 0.8, 1.0, 1.0);\n"
      "}                                  \n"
    };
    
    
    program = glCreateProgram();
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
  }
  
  void startup() {
    rendering_program = compile_shaders();
    
    // #NOTE(4lex1v) :: Sadly my super expensive mac 2017 doesn't support OpenGL 4.5... Have to use "the old stuff"
    // glCreateVertexArrays(1, &vertex_array_object);
    glGenVertexArrays(1, &vertex_array_object);
    
    glBindVertexArray(vertex_array_object);
  }
  
  void shutdown() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
  }
  
  void render (double current_time) {
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.5f };
    const GLfloat color[] = {
      (float) sin(current_time) * 0.5f + 0.5f,
      (float) cos(current_time) * 0.5f + 0.5f,
      0,
      1.0f
    };
      
    glClearBufferfv(GL_COLOR, 0, color);
    glUseProgram(rendering_program);
    glPointSize(40.0f);
    glDrawArrays(GL_POINTS, 0, 1);
  }
};

DECLARE_MAIN(basic_shaders)
