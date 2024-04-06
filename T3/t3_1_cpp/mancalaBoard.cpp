#include "..\..\Board\Board.h"
#include "cstdio"
#include "cstring"

class Board {
private:
    int hole[14] = {4, 4, 4, 4, 4, 4, 0,
                    4, 4, 4, 4, 4, 4, 0};
    int first;
    bool stop = false;
    int curPlayer;
    int sum(int player);
public:
    Board(int player);
    bool sow(int player, int pos);
    bool isStop();
    int score();
    int scoreDifference();
    int getCurPlayer();
    int *getHole();
};

int Board::sum(int player) {
    if (player == 1) {
        return hole[0] + hole[1] + hole[2] + hole[3] + hole[4] + hole[5];
    } else {
        return hole[7] + hole[8] + hole[9] + hole[10] + hole[11] + hole[12];
    }
}

Board::Board(int player) {
    first = curPlayer = player;
}

bool Board::sow(int player, int pos) {
    if (player != curPlayer) {
        return false;
    }

    int start = 6 * (player - 1) + pos - 1;
    int cnt = hole[start];
    hole[start] = 0;
    int now = start;
    for (int i = 0; i < cnt; ++i) {
        now = (now + 1) % 14;
        if ((player == 1 && now == 13) || (player == 2 && now == 6)) {
            --i;
            continue;
        }
        ++hole[now];
    }



    if (player == 1 && hole[now] == 1 && 0 <= now && now <= 5) {
        hole[6] += (hole[now] + hole[13 - now]);
        hole[now] = 0;
        hole[13 - now] = 0;
    } else if ((player == 2 && hole[now] == 1) && (7 <= now && now <= 12)) {
        hole[13] += (hole[now] + hole[13 - now]);
        hole[now] = 0;
        hole[13 - now] = 0;
    }

    if (!((player == 1 && now == 7) || (player == 2 && now == 13))) {
        curPlayer = 3 - player;
    }

    if (sum(1) == 0) {
        hole[13] += sum(2);
        for (int i = 0; i < 13; ++i) {
            hole[i] = 0;
        }
        stop = true;
    } else if (sum(2) == 0) {
        hole[6] += sum(1);
        for (int i = 0; i < 13; ++i) {
            hole[i] = 0;
        }
        stop = true;
    }
    return true;
}

bool Board::isStop() {
    return stop;
}

int Board::score() {
    return hole[7 * first - 1];
}

int Board::scoreDifference() {
    return hole[7 * first - 1] - hole[20 - 7 * first];
}

int Board::getCurPlayer() {
    return curPlayer;
}

int *Board::getHole() {
    return hole;
}

int mancalaResult(int flag, int seq[], int size) {
    auto *board = new Board(flag);
    for (int i = 0; i < size; ++i) {
        int player = seq[i] / 10;
        int pos = seq[i] % 10;
        if (!board->sow(player, pos)) {
            delete board;
            return 30000 + i;
        }
    }

    if (board->isStop()) {
        int sc = board->scoreDifference();
        delete board;
        return 15000 + sc;
    } else {
        int sc = board->score();
        delete board;
        return 20000 + sc;
    }
}

int *mancalaBoard(int flag, int seq[], int size) {
    auto board = new Board(flag);
    for (int i = 0; i < size; ++i) {
        int player = seq[i] / 10;
        int sow = seq[i] % 10;
        board->sow(player, sow);
    }
    int *result = new int [15];
    memcpy(result, board->getHole(), 14 * sizeof(int));
    if (board->isStop()) {
        result[14] = 200 + board->scoreDifference();
    } else {
        result[14] = board->getCurPlayer();
    }
    delete board;
    return result;
}

int main() {
    int flag = ;
    int seq[] = ;
    int size = ;
    int *result = mancalaBoard(flag, seq, size);
    for(int i = 0; i < 15; ++i) {
        printf("%d", result[i]);
    }
}