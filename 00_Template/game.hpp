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
  void GetEventState();
  void GetKeyboardState();

  void UpdateGame();

  void GenerateOutput();

  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  Uint32 mTicksCount;
  bool mIsRunning;

  constexpr const static char* WINDOW_TITLE = "Template";
  const static int WINDOW_X_POS  = 100;
  const static int WINDOW_Y_POS  = 100;
  const static int WINDOW_WIDTH  = 1024;
  const static int WINDOW_HEIGHT = 768;
  const static int WINDOW_FLAGS  = 0;
};
