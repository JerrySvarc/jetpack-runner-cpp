#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"

class Game;

class Item {
public:
  virtual void affect_game(Game &game) = 0;
  virtual void reset_effect(Game &game) = 0;
};


class Coin: public Entity, public Item {
public:
	virtual void affect_game(Game& game) override;
	virtual void reset_effect(Game& game) override;
	void init_coin();
};

class GravityReverser : public Entity, public Item {
public:
	virtual void affect_game(Game& game) override;
	virtual void reset_effect(Game& game) override;
	void init_reverser();
};

#endif
