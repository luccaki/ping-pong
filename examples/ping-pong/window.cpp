#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    out vec4 fragColor;

    void main() { 
      gl_Position = vec4(inPosition, 0, 1);
      fragColor = inColor;
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
  
    precision mediump float;

    in vec4 fragColor;

    out vec4 outColor;

    void main() { outColor = fragColor; }
  )gl"};

  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Window::getRandomVelocity() {
  auto &re{m_randomEngine};
  glm::vec2 const direction{m_randomDist(re), m_randomDist(re)};
  velocity = glm::normalize(direction) / 1.5f;
  checkVelocity();
  checkVictory();
}

void Window::checkVictory(){
  if(score.x >= pointsToWin + 1 || score.y >= pointsToWin + 1){
    restartGame();
    velocity = {0.0f, 0.0f};
  }
}

void Window::checkVelocity() {
  if (velocity.x > 0 && velocity.y > 0) {
    if (velocity.x < velocity.y)
      getRandomVelocity();
  } else if (velocity.x > 0 && velocity.y < 0) {
    if (velocity.x < -velocity.y)
      getRandomVelocity();
  } else if (velocity.x < 0 && velocity.y < 0) {
    if (-velocity.x < -velocity.y)
      getRandomVelocity();
  } else if (velocity.x < 0 && velocity.y > 0) {
    if (-velocity.x < velocity.y)
      getRandomVelocity();
  }
}

void Window::onPaint() {
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  updateSliders();
  updateBall(deltaTime);

  if (increaseVelocity && m_velocityTimer.elapsed() > 5) {
    ballVelocity += 0.1;
    m_velocityTimer.restart();
  }
}

void Window::updateBall(float deltaTime) {
  ballPosition.x += velocity.x * deltaTime * ballVelocity;
  ballPosition.y += velocity.y * deltaTime * ballVelocity;

  checkSliderAndBall();
  checkBorderAndBall();

  std::array<glm::vec2, 4> position{
      {{ballPosition.x - 0.01, ballPosition.y - 0.01},
       {ballPosition.x + 0.01, ballPosition.y - 0.01},
       {ballPosition.x + 0.01, ballPosition.y + 0.01},
       {ballPosition.x - 0.01, ballPosition.y + 0.01}}};

  setupModel(position);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::checkSliderAndBall() {
  if (ballPosition.x > 0.9 && ballPosition.x < 0.95) {
    if (ballPosition.y >= sliderRightValue &&
        ballPosition.y <= sliderRightValue + (sliderHeight / 2)) {
      ballPosition.x = 0.9;
      getRandomVelocity();
      if (velocity.x > 0)
        velocity.x = -velocity.x;
      if (velocity.y < 0)
        velocity.y = -velocity.y;
    } else if (ballPosition.y <= sliderRightValue &&
               ballPosition.y >= sliderRightValue - (sliderHeight / 2)) {
      ballPosition.x = 0.9;
      getRandomVelocity();
      if (velocity.x > 0)
        velocity.x = -velocity.x;
      if (velocity.y > 0)
        velocity.y = -velocity.y;
    }
  } else if (ballPosition.x < -0.9 && ballPosition.x > -0.95) {
    if (ballPosition.y >= sliderLeftValue &&
        ballPosition.y <= sliderLeftValue + (sliderHeight / 2)) {
      ballPosition.x = -0.9;
      getRandomVelocity();
      if (velocity.x < 0)
        velocity.x = -velocity.x;
      if (velocity.y < 0)
        velocity.y = -velocity.y;
    } else if (ballPosition.y <= sliderLeftValue &&
               ballPosition.y >= sliderLeftValue - (sliderHeight / 2)) {
      ballPosition.x = -0.9;
      getRandomVelocity();
      if (velocity.x < 0)
        velocity.x = -velocity.x;
      if (velocity.y > 0)
        velocity.y = -velocity.y;
    }
  }
}

void Window::checkBorderAndBall() {
  if (ballPosition.x > 1) {
    score.x++;
    ballPosition.x = 0;
    ballPosition.y = 0;
    getRandomVelocity();
    if (velocity.x > 0)
      velocity.x = -velocity.x;
  } else if (ballPosition.x < -1) {
    score.y++;
    ballPosition.x = 0;
    ballPosition.y = 0;
    getRandomVelocity();
    if (velocity.x < 0)
      velocity.x = -velocity.x;
  } else if (ballPosition.y > 1) {
    ballPosition.y = 1;
    velocity.y = -velocity.y;
  } else if (ballPosition.y < -1) {
    ballPosition.y = -1;
    velocity.y = -velocity.y;
  }
}

void Window::updateSliders() {
  if (m_timer.elapsed() > 0.05) {
    if (wPressed) {
      sliderLeftValue += 0.05;
      if (sliderLeftValue > 1)
        sliderLeftValue = 1;
    }
    if (sPressed) {
      sliderLeftValue -= 0.05;
      if (sliderLeftValue < -1)
        sliderLeftValue = -1;
    }
    if (upPressed) {
      sliderRightValue += 0.05;
      if (sliderRightValue > 1)
        sliderRightValue = 1;
    }
    if (downPressed) {
      sliderRightValue -= 0.05;
      if (sliderRightValue < -1)
        sliderRightValue = -1;
    }

    m_timer.restart();
  }

  std::array<glm::vec2, 4> sliderLeftPosition{
      {{-0.95, sliderLeftValue - (sliderHeight / 2)},
       {-0.9, sliderLeftValue - (sliderHeight / 2)},
       {-0.9, sliderLeftValue + (sliderHeight / 2)},
       {-0.95, sliderLeftValue + (sliderHeight / 2)}}};

  setupModel(sliderLeftPosition);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);

  std::array<glm::vec2, 4> sliderRightPosition{
      {{0.9, sliderRightValue - (sliderHeight / 2)},
       {0.95, sliderRightValue - (sliderHeight / 2)},
       {0.95, sliderRightValue + (sliderHeight / 2)},
       {0.9, sliderRightValue + (sliderHeight / 2)}}};

  setupModel(sliderRightPosition);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::setupModel(std::array<glm::vec2, 4> &position) {

  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(position), position.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::restartGame() {
  score = {0, 0};
  getRandomVelocity();
  ballPosition = {0.0f, 0.0f};
  sliderRightValue = 0.0f;
  sliderLeftValue = 0.0f;
  ballVelocity = 1;
  increaseVelocity = false;
}

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w)
      wPressed = true;
    if (event.key.keysym.sym == SDLK_s)
      sPressed = true;
    if (event.key.keysym.sym == SDLK_UP)
      upPressed = true;
    if (event.key.keysym.sym == SDLK_DOWN)
      downPressed = true;
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_w)
      wPressed = false;
    if (event.key.keysym.sym == SDLK_s)
      sPressed = false;
    if (event.key.keysym.sym == SDLK_UP)
      upPressed = false;
    if (event.key.keysym.sym == SDLK_DOWN)
      downPressed = false;
  }
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto windowTitle{fmt::format("{0} X {1}", static_cast<int>(score.x),
                                 static_cast<int>(score.y))};
    auto const widgetSize{ImVec2(500, 110)};
    ImGui::SetNextWindowPos(
        ImVec2((m_viewportSize.x / 2) - (widgetSize.x / 2), 0));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNavInputs};
    ImGui::Begin(windowTitle.c_str(), nullptr, windowFlags);

    ImGui::SliderFloat("Ball Velocity", &ballVelocity, 0.0f, 3.0f);

    if (ImGui::Button("Start/Restart")) {
      restartGame();
    }

    ImGui::SameLine();
    if (ImGui::Button("Pause/Continue")) {
      pause = !pause;
      if (pause) {
        pauseVelocity = velocity;
        velocity = {0.0f, 0.0f};
      } else {
        velocity = pauseVelocity;
      }
    }

    ImGui::SameLine();
    if (ImGui::Button("Change Color!")) {
      auto &re{m_randomEngine};
      m_colors = {
          {{m_randomColor(re), m_randomColor(re), m_randomColor(re), 1},
           {m_randomColor(re), m_randomColor(re), m_randomColor(re), 1},
           {m_randomColor(re), m_randomColor(re), m_randomColor(re), 1}}};
    }

    ImGui::SameLine();
    if (ImGui::Button("Fire Mode!")) {
      increaseVelocity = !increaseVelocity;
    }

    const char* items[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
    ImGui::Combo("Points to Win", &pointsToWin, items, IM_ARRAYSIZE(items));

    ImGui::End();
  }

  /*
  {
    auto const widgetSize{ImVec2(30, m_viewportSize.y)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x,
                                   m_viewportSize.y - widgetSize.y));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus};
    ImGui::Begin("Slider Right", nullptr, windowFlags);

    ImGui::VSliderFloat("", ImVec2(15, m_viewportSize.y * 0.96),
                        &sliderRightValue, -1.0f, 1.0f);

    ImGui::End();
  }

  {
    auto const widgetSize{ImVec2(30, m_viewportSize.y)};
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus};
    ImGui::Begin("Slider Left", nullptr, windowFlags);

    ImGui::VSliderFloat("", ImVec2(15, m_viewportSize.y * 0.96),
                        &sliderLeftValue, -1.0f, 1.0f);

    ImGui::End();
  }
  */
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}