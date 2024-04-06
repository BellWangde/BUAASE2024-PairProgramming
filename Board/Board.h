#ifndef PAIRPROGRAM_BOARD_H
#define PAIRPROGRAM_BOARD_H


class Board {
private:
    int hole[14] = {4, 4, 4, 4, 4, 4, 0,
                    4, 4, 4, 4, 4, 4, 0};
    int first;
    int step = 0;
    bool stop = false;
    int curPlayer;
    int sum(int player);
public:
    Board(int player);
    Board(int player, int *hole);
    Board(Board* board);
    bool query(int player, int pos);
    bool sow(int player, int pos);
    bool isStop();
    int score();
    int scoreDifference();
    int getCurPlayer();
    int *getHole();
    int getStep();
};


#endif //PAIRPROGRAM_BOARD_H
