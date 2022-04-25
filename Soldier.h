#ifndef EXE2_SOLDIER_H
#define EXE2_SOLDIER_H

class Soldier {
private:
    int power;
    int player;
public:
    Soldier(int p) : power(p), player(0){}
    int getPower(){return power;}
    void setPower(int p) {power = p;}
    void setPlayer(int p) {player = p;}
    int getPlayer(){return player;}
    int dead();

};
#endif //EXE2_SOLDIER_H
