#include "..\..\Board\Board.h"
#include <queue>
#include <ctime>
#include <random>
#include <algorithm>

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

