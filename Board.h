#ifndef EXE2_BOARD_H
#define EXE2_BOARD_H

#include "Soldier.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

class Board{
private:
    Soldier*** map;
    int turn;
public:
    Board();
    void print();
    int addSoldiers(const std::string& s ,int p);
    void play();
    void changeTurn();
    int makeMove();
    void changePlace(int pi, int pj, int ti, int tj);
    int isFinish();

};

#endif //EXE2_BOARD_H
