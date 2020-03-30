#include "game.hpp"

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
  ,mPaddleDir(0)
  ,mIsRunning(true) {}

Game::~Game() {}

bool Game::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("ERROR : %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow(
    WINDOW_TITLE,
    WINDOW_X_POS, WINDOW_Y_POS,
    WINDOW_WIDTH, WINDOW_HEIGHT,
    WINDOW_FLAGS
  );

  if (!mWindow) {
    SDL_Log("ERROR : %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
    mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  mPaddlePos.x =   10.0f;
  mPaddlePos.y =  768.0f / 2.0f;
  mBallPos.x   = 1024.0f / 2.0f;
  mBallPos.y   =  768.0f / 2.0f;
  mBallVel.x   = -200.0f;
  mBallVel.y   =  235.0f;

  return true;
}

void Game::Shutdown() {
  SDL_DestroyRenderer(mRenderer);
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
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
}

void Game::GenerateOutput() {
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

  SDL_RenderClear(mRenderer);

  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  GenerateWall();
  GeneratePaddle();
  GenerateBall();

  SDL_RenderPresent(mRenderer);
}

void Game::GenerateWall() {
  SDL_Rect wall{0, WINDOW_HEIGHT - THICKNESS, 1024, THICKNESS};

  SDL_RenderFillRect(mRenderer, &wall);

  wall.x = WINDOW_WIDTH - THICKNESS;
  wall.y = 0;
  wall.w = THICKNESS;
  wall.h = WINDOW_WIDTH;

  SDL_RenderFillRect(mRenderer, &wall);
}

void Game::GeneratePaddle() {
  SDL_Rect paddle{
    static_cast<int>(mPaddlePos.x),
    static_cast<int>(mPaddlePos.y - THICKNESS / 2),
    THICKNESS,
    static_cast<int>(PADDLE_HEIGHT)
  };
  SDL_RenderFillRect(mRenderer, &paddle);
}

void Game::GenerateBall() {
  SDL_Rect ball{
    static_cast<int>(mBallPos.x - THICKNESS / 2),
    static_cast<int>(mBallPos.y - THICKNESS / 2),
    THICKNESS,
    THICKNESS
  };
  SDL_RenderFillRect(mRenderer, &ball);
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}
