#include "core/Player.h"

#ifndef PLAYER_MARISA_H
#define PLAYER_MARISA_H

class PlayerMarisa : public Player {
private:
    void attack() override;
public:
    PlayerMarisa();
};

#endif // PLAYER_MARISA_H