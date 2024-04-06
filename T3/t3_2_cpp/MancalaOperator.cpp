#include "..\..\Board\Board.h"
#include <queue>
#include <ctime>
#include <random>
#include <algorithm>

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
    int start = 6 * (player - 1) + pos - 1;
    return hole[start] != 0;
}

bool Board::sow(int player, int pos) {
    if (player != curPlayer) {
        return false;
    }
    ++step;
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

int Board::getStep() {
    return step;
}

std::queue<std::pair<Board*, int>> boardQueue;

int decide(Board *board, int flag, int maxStep) {
    int maxScoreDifference = -49;
    int bestMove = 0;
    boardQueue.push(std::make_pair(board, 0));
    while (!boardQueue.empty()) {
        Board *nowBoard = boardQueue.front().first;
        int move = boardQueue.front().second;
        boardQueue.pop();
        if (nowBoard->getStep() == maxStep || nowBoard->isStop()) {
            int diff = nowBoard->scoreDifference() * (flag * -2 + 3);
            if (diff > maxScoreDifference) {
                maxScoreDifference = diff;
                bestMove = move;
            }
            continue;
        }
        std::vector<int> moves = {1, 2, 3, 4, 5, 6};
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(moves.begin(), moves.end(), g);
        for (int i = 0; i <= 5; i++) {
            if (nowBoard->query(nowBoard->getCurPlayer(), moves[i])) {
                nowBoard->sow(nowBoard->getCurPlayer(), moves[i]);
                Board *nextBoard = new Board(nowBoard);
                if (move == 0) {
                    boardQueue.push(std::make_pair(nextBoard, moves[i]));
                } else {
                    boardQueue.push(std::make_pair(nextBoard, move));
                }
            }
        }
    }
    return bestMove;
}

int mancalaOperator(int flag, int status[]) {
    auto board = new Board(flag, status);
    int maxStep = 1e8;
    return decide(board, flag, maxStep);
}

