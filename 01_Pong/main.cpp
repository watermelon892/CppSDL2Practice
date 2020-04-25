#include "game.hpp"

int main(void) {
  Game game;
  if (game.Initialize()) {
    game.RunLoop();
  }
  game.Shutdown();
  return 0;
}
