#include "Soldier.h"

int Soldier::dead() {
    int i =player;
    power = -2;
    player = 0;
    return i;
}
