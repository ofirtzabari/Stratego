//
#include "Board.h"

Board::Board() : map(nullptr), turn(1){
    map= new Soldier**[10];
    for (int i = 0; i < 10; i++) {
        map[i] = new Soldier*[10];
        for (int j = 0; j < 10; j++) {
            if( (i==4 || i==5)  && (j==0 || j==1 || j==4 || j==5 || j==8 || j==9))
                map[i][j] = new Soldier(-2);      // -2 is open path
            else if ( (i==4 || i==5)  && (j==2 || j==3 || j==6 || j==7))
                map[i][j] = new Soldier(-3);       // -3 is close path / sea
            else
                map[i][j] = new Soldier(1); // 1 is default
        }
    }
}

/* print the board with soldiers */
void Board::print() {
    for (int i = 0; i < 10; i++) {
        std::cout << i+1 << "\t";
        for (int j = 0; j < 10; j++) {
            if (map[i][j]->getPlayer() == turn || map[i][j]->getPlayer() == 0) {
                if (map[i][j]->getPower() == -2)
                    std::cout << "O ";
                else if (map[i][j]->getPower() == -3)
                    std::cout << "- ";
                else if (map[i][j]->getPower() == 10)
                    std::cout << "A ";
                else if (map[i][j]->getPower() == 0)
                    std::cout << "B ";
                else if (map[i][j]->getPower() == -1)
                    std::cout << "F ";
                else
                    std::cout << map[i][j]->getPower() << " ";
            }
            else
                std::cout << "S ";
        }
        std::cout<<std::endl;
    }
    std::cout << "\n\tA B C D E F G H I J" << std::endl;
}

/* check the soldiers string and placing them  */
int Board::addSoldiers(const std::string& s, int p) {
    if (s == "QUIT" && p == 1){
        std::cout << "P1: QUIT" << std::endl;
        std::cout << "P2 wins the game." << std::endl;
        exit(0);
    }
    else if (s == "QUIT" && p == 2){
        std::cout << "P2: QUIT" << std::endl;
        std::cout << "P1 wins the game." << std::endl;
        exit(0);
    }
     int soldier[12] = {0};
     int validSoldiers[12] = {6,1,8,5,4,4,4,3,2,1,1,1};
     std::stringstream ss (s);
     std::string str;
     while (getline(ss,str,',')) {
         for (char c : str)
             if (!std::isdigit(c) && c != '-' )  ///start the tests, every test return 1 if failed
                 return 1; // return 1 if fail
         if (std::stoi(str) < -1 || std::stoi(str) > 10)
             return 1;
         if (str == "-1")
             soldier[11]++;
         else
             soldier[std::stoi(str)]++;
     }
    for (int i = 0; i < 12; ++i) {
        if ( soldier[i] != validSoldiers[i])
            return 1;                             /// here test end
    }
    std::stringstream ss2 (s);

    /// set soldiers by each player (player 1 or player 2)
    if (p == 1) {
        int i=6, j=0;
        while (i < 10){
            getline(ss2,str,',');
            map[i][j]->setPower(std::stoi(str));
            map[i][j]->setPlayer(1);
            j++;
            if (j == 10) {
                i++;
                j = 0;
            }
        }
    }
    if (p == 2) {
        int i=3, j=0;
        while (i > -1){
            getline(ss2,str,',');
            map[i][j]->setPower(std::stoi(str));
            map[i][j]->setPlayer(2);
            j++;
            if (j == 10) {
                i--;
                j = 0;
            }
        }
    }
    return 0;
}

/* the order of the game */
void Board::play() {
    std::string s;
    int con = 1;
    while (con) {
        std::cout << "P1: " <<std::endl;
        std::cin >> s;
        con = addSoldiers(s,1);
        if (con != 0)
            std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
    }
    con = 1;
    while (con) {
        std::cout << "P2: " <<std::endl;
        std::cin >> s;
        con = addSoldiers(s,2);

    }
    int finish=0;
    while (finish == 0) {

        while (makeMove()){}
        changeTurn();
        finish = isFinish();
    }
}

void Board::changeTurn() {
    if (turn == 1)
        turn = 2;
    else
        turn = 1;
}

int Board::makeMove() {
    int pos[2] = {0}, to[2] = {0};
    std::string s;
    static int bug =0 ;
    static int stratego =0;
    if (bug != 0)
        print();
    if (stratego ==1){
        std::cout << "STRATEGO!!" << std::endl;
        stratego = 0;
    }
    if (turn == 1 && bug != 0)
        std::cout << "P1:" << std::endl;
    else if (turn == 2 && bug != 0)
        std::cout << "P2:" << std::endl;

    std::getline(std::cin, s);
    if (bug == 0) {
        bug++;
        return 1;
    }

    if (s == "QUIT" && turn == 1){
        std::cout << "P1: QUIT" << std::endl;
        std::cout << "P2 wins the game." << std::endl;
        exit(0);
    }
    else if (s == "QUIT" && turn == 2){
        std::cout << "P2: QUIT" << std::endl;
        std::cout << "P1 wins the game." << std::endl;
        exit(0);
    }

    if (s.length() != 5){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}
    if (s[0] < 'A' || s[0] > 'j' || s[3] < 'A' || s[3] > 'j' || ( s[0] > 'J' && s[0] < 'a') || ( s[3] > 'J' && s[3] < 'a')){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}
    if (!std::isdigit(s[1]) || s[2] != ' ' ||  !std::isdigit(s[4])){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}

    pos[0] = s[1] -'1';
    if (std::islower(s[0]))
        pos[1] = s[0] -'a';
    else
        pos[1] = s[0] -'A';
    to[0] = s[4] -'1';
    if (std::islower(s[3]))
        to[1] = s[3] -'a';
    else
        to[1] = s[3] -'A';

    if (map[pos[0]][pos[1]]->getPlayer() != turn || map[to[0]][to[1]]->getPlayer() == turn){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
    return 1;}

    if ((to[0]+to[1] < pos[0]+pos[1]-1 || to[0]+to[1] > pos[0]+pos[1]+1) && map[pos[0]][pos[1]]->getPower() != 2){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}

    if (map[pos[0]][pos[1]]->getPower() == 1 && pos[0] != to[0] && pos[1] != to[1]){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}
    if (map[to[0]][to[1]]->getPower() == -3 ){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}
    if (map[pos[0]][pos[1]]->getPower() == -1 || map[pos[0]][pos[1]]->getPower() == 0){
        std::cerr << "Invalid move; the game awaits a valid move." << std::endl;
        return 1;}

    int i=0;
    if ((map[pos[0]][pos[1]]->getPower() == 1 && map[to[0]][to[1]]->getPower() == 10) || (map[pos[0]][pos[1]]->getPower() == 10 && map[to[0]][to[1]]->getPower() == 1)){
        i = map[to[0]][to[1]]->dead();
        changePlace(pos[0], pos[1], to[0], to[1]);}
    else if ((map[pos[0]][pos[1]]->getPower() < map[to[0]][to[1]]->getPower()) && map[to[0]][to[1]]->getPower() != 0 && map[to[0]][to[1]]->getPower() != -2){
        i = map[pos[0]][pos[1]]->dead();
        changePlace(pos[0], pos[1], to[0], to[1]);}
    else if ((map[pos[0]][pos[1]]->getPower() > map[to[0]][to[1]]->getPower()) && map[to[0]][to[1]]->getPower() != 0 && map[to[0]][to[1]]->getPower() != -2){
        i = map[to[0]][to[1]]->dead();
        changePlace(pos[0], pos[1], to[0], to[1]);}
    else if (map[pos[0]][pos[1]]->getPower() == map[to[0]][to[1]]->getPower()){
        map[to[0]][to[1]]->dead();
        map[pos[0]][pos[1]]->dead();
        stratego = 1;}
    else if (map[to[0]][to[1]]->getPower() == 0 && map[pos[0]][pos[1]]->getPower() != 3){
        i = map[pos[0]][pos[1]]->dead();}
    else if (map[to[0]][to[1]]->getPower() == 0 && map[pos[0]][pos[1]]->getPower() == 3){
        i = map[to[0]][to[1]]->dead();}
    else
        changePlace(pos[0], pos[1], to[0], to[1]);

    if (i == 1)
        std::cout << "STRATEGO!! P2 won the fight" << std::endl;
    else if (i == 2)
        std::cout << "STRATEGO!! P1 won the fight" << std::endl;
    return 0;
}

void Board::changePlace(int pi, int pj, int ti, int tj) {
    Soldier* temp = map[pi][pj];
    map[pi][pj] = map[ti][tj];
    map[ti][tj] = temp;
}

int Board::isFinish(){
    bool flag[2] = {false};
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (map[i][j]->getPower() == -1)
                flag[map[i][j]->getPlayer()-1] = true;

    if (flag[0] && !flag[1])
        return 1;
    else if (!flag[0] && flag[1])
        return 2;
    return 0;
}



