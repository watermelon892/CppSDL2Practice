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

  mPaddlePos.x = INIT_PADDLE_POS_X;
  mPaddlePos.y = INIT_PADDLE_POS_Y;
  mBallPos.x   = INIT_BALL_POS_X;
  mBallPos.y   = INIT_BALL_POS_Y;
  mBallVel.x   = INIT_BALL_VEL_X;
  mBallVel.y   = INIT_BALL_VEL_Y;

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

  if (IsHitPaddle())          mBallVel.x *= -1.0f;
  else if (IsHitTopWall())    mBallVel.y *= -1.0f;
  else if (IsHitRightWall())  mBallVel.x *= -1.0f;
  else if (IsHitBottomWall()) mBallVel.y *= -1.0f;
  else if (IsHitLeftWall())   mIsRunning = false; // Game Over
}

bool Game::IsHitPaddle() {
  float diff = mPaddlePos.y - mBallPos.y;
  diff = (diff > 0.0f) ? diff : -diff;
  return (diff <= PADDLE_HEIGHT / 2.0f &&
          mBallPos.x <= 25.0f &&
          mBallPos.x >= 20.0f &&
          mBallVel.x < 0.0f);
}

bool Game::IsHitTopWall() {
  return mBallPos.y <= THICKNESS && mBallVel.y < 0.0f;
}

bool Game::IsHitRightWall() {
  return (mBallPos.x >= (float)(WINDOW_WIDTH - THICKNESS) &&
          mBallVel.x > 0.0f);
}

bool Game::IsHitBottomWall() {
  return (mBallPos.y >= (float)(WINDOW_HEIGHT - THICKNESS)  &&
          mBallVel.y > 0.0f);
}

bool Game::IsHitLeftWall() {
  return mBallPos.x <= 0.0f;
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
  SDL_Rect wall{0, WINDOW_HEIGHT - THICKNESS, WINDOW_WIDTH, THICKNESS};

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
