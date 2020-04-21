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

  mPaddlePos.x = PADDLE_INIT_POS_X;
  mPaddlePos.y = PADDLE_INIT_POS_Y;
  mBallPos.x   = BALL_INIT_POS_X;
  mBallPos.y   = BALL_INIT_POS_Y;
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
  GetEventState();
  GetKeyboardState();
}

void Game::GetEventState() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      mIsRunning = false;
      break;
    }
  }
}

void Game::GetKeyboardState() {
  mPaddleDir = 0;
  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_UP])     mPaddleDir -= 1;
  if (state[SDL_SCANCODE_DOWN])   mPaddleDir += 1;
  if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
}

void Game::UpdateGame() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

  if (deltaTime > 0.05f) deltaTime = 0.05f;

  mTicksCount = SDL_GetTicks();

  LimitMoveRangeOfPaddle(deltaTime);

  MoveBall(deltaTime);
}

void Game::LimitMoveRangeOfPaddle(float deltaTime) {
  if (mPaddleDir != 0) {
    mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
    if (mPaddlePos.y < 0) {
      mPaddlePos.y = 0;
    } else if (mPaddlePos.y > WINDOW_HEIGHT - PADDLE_HEIGHT) {
      mPaddlePos.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
    }
  }
}

void Game::MoveBall(float deltaTime) {
  mBallPos.x += mBallVel.x * deltaTime;
  mBallPos.y += mBallVel.y * deltaTime;

  float diff = mPaddlePos.y - mBallPos.y;
  diff = (diff > 0.0f) ? diff : -diff;

  // Bounce
  if (diff <= PADDLE_HEIGHT / 2.0f &&
      mBallPos.x <= 25.0f &&
      mBallPos.x >= 20.0f &&
      mBallVel.x < 0.0f) {
    // Paddle
    mBallVel.x *= -1.0f;
  } else if (mBallPos.y <= THICKNESS && mBallVel.y < 0.0f) {
    // Top wall
    mBallVel.y *= -1;
  } else if (mBallPos.x >= (1024.0f - THICKNESS) && mBallVel.x > 0.0f) {
    // Right wall
    mBallVel.x *= -1.0f;
  } else if (mBallPos.y >= (768 - THICKNESS) && mBallVel.y > 0.0f) {
    // Bottom wall
    mBallVel.y *= -1;
  } else if (mBallPos.x <= 0.0f) {
    // Left wall (Game Over)
    mIsRunning = false;
  }
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
