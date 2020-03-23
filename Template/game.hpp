#pragma once

#include <SDL2/SDL.h>

class Game {
public:
  Game();
  ~Game();
  bool Initialize();
  void RunLoop(); // Run the game loop until the game over
  void Shutdown();

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  Uint32 mTicksCount;
  bool mIsRunning;

  struct PosVec{
    float x;
    float y;
  };

  PosVec mBallPos;
  PosVec mPaddlePos;

  constexpr const static char* TITLE = "Template";
  const static int X_POS     = 100;
  const static int Y_POS     = 100;
  const static int WIDTH     = 1024;
  const static int HEIGHT    = 768;
  const static int FLAGS     = 0;

  const static int THICKNESS  = 15;
  constexpr const static float PADDLE_H = 100.0f;
};
