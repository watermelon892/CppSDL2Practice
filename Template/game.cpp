#include "game.hpp"

Game::Game() {
}

Game::~Game() {
}

bool Game::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("ERROR : %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow(TITLE, X_POS, Y_POS, WIDTH, HEIGHT, FLAGS);

  if (!mWindow) {
    SDL_Log("ERROR : %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
    mWindow,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  mIsRunning = true;

  return true;
}

void Game::Shutdown() {
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      mIsRunning = false;
      break;
    }
  }

  const Uint8* state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE]) {
    mIsRunning = false;
  }
}

void Game::UpdateGame() {
}

void Game::GenerateOutput() {
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255); // R G B A
  SDL_RenderClear(mRenderer);

  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
  SDL_Rect wall{0, HEIGHT / 2, thickness, thickness * 5};
  SDL_RenderFillRect(mRenderer, &wall);

  SDL_Rect ball{
    static_cast<int>(mBallPos.x - thickness / 2),
    static_cast<int>(mBallPos.y - thickness / 2),
    thickness,
    thickness
  };
  SDL_RenderFillRect(mRenderer, &ball);

  SDL_RenderPresent(mRenderer);
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}
