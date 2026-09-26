#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <memory>
#include <optional>

class Player : public Entity {
public:
  void init_player();
  int uplift;

  const double player_x_pos = 40;
  double player_y_max = 345;
};
#endif
