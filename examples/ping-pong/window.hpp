#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include <fmt/format.h>

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
  std::uniform_real_distribution<float> m_randomColor{0.0f, 1.0f};

  abcg::Timer m_timer;
  abcg::Timer m_velocityTimer;

  std::array<glm::vec4, 3> m_colors{{{0.36f, 0.83f, 1.00f, 1},
                                     {0.63f, 0.00f, 0.61f, 1},
                                     {1.00f, 0.69f, 0.30f, 1}}};

  float sliderRightValue{0.0f};
  float sliderLeftValue{0.0f};
  float sliderHeight{0.3f};

  float ballVelocity{1.0f};

  bool wPressed{false};
  bool sPressed{false};
  bool upPressed{false};
  bool downPressed{false};

  bool increaseVelocity{false};

  glm::vec2 ballPosition{0.0f, 0.0f};
  glm::vec2 velocity{0.0f, 0.0f};

  glm::vec2 score{0, 0};
  
  bool pause{false};
  glm::vec2 pauseVelocity{0.0f, 0.0f};

  int pointsToWin{4};

  void setupModel(std::array<glm::vec2, 4>&);
  void updateSliders();
  void updateBall(float);
  void getRandomVelocity();
  void checkBorderAndBall();
  void checkSliderAndBall();
  void restartGame();
  void checkVelocity();
  void checkVictory();
};

#endif