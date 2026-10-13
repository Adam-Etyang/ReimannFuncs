#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <iostream>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

void framebufferSizeCallback(GLFWwindow *, int width, int height) {
  glViewport(0, 0, width, height);
}

GLuint compileShader(GLenum type, const char *source) {
  const GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    char log[1024]{};
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    std::cerr << "Shader compilation failed: " << log << '\n';
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint createProgram() {
  constexpr char vertexShaderSource[] = R"(
        #version 410 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        out vec3 vertexColor;

        void main() {
            gl_Position = vec4(position, 1.0);
            vertexColor = color;
        }
    )";

  constexpr char fragmentShaderSource[] = R"(
        #version 410 core
        in vec3 vertexColor;
        out vec4 fragmentColor;

        void main() {
            fragmentColor = vec4(vertexColor, 1.0);
        }
    )";

  const GLuint vertexShader =
      compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  const GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  if (vertexShader == 0 || fragmentShader == 0) {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
  }

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLint linked = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLint logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    char log[1024]{};
    glGetProgramInfoLog(program, sizeof(log), nullptr, log);
    std::cerr << "Shader program linking failed: " << log << '\n';
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

} // namespace

int main() {
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(kWindowWidth, kWindowHeight,
                       "ReimanViz - OpenGL Starter", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create an OpenGL 4.1 window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSwapInterval(1);

  std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';

  constexpr float vertices[] = {
      0.0F,  0.65F, 0.0F,  0.95F, 0.38F, 0.28F, -0.65F, -0.5F, 0.0F,
      0.25F, 0.72F, 0.92F, 0.65F, -0.5F, 0.0F,  0.48F,  0.86F, 0.55F,
  };

  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  glGenVertexArrays(1, &vertexArray);
  glGenBuffers(1, &vertexBuffer);
  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  const GLuint program = createProgram();
  if (program == 0) {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glClearColor(0.055F, 0.075F, 0.11F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(program);
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteVertexArrays(1, &vertexArray);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
