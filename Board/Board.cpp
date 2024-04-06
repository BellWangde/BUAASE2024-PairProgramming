#include "Board.h"
#include "cstring"
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

Board::Board(int player, int *hole) {
    first = curPlayer = player;
    memcpy(this->hole, hole, 14 * sizeof(int));
}

Board::Board(Board *board) {
    memcpy(this, board, sizeof(Board));
}

bool Board::query(int player, int pos) {
    int start = 7 * player + pos - 8;
    return hole[start] != 0;
}

bool Board::sow(int player, int pos) {
    if (player != curPlayer) {
        return false;
    }
    ++step;
    int start = 7 * player + pos - 8;
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

int Board::getStep() {
    return step;
}