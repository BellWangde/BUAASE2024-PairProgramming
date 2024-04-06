#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <emscripten/emscripten.h>

struct Board {
    int hole[14];
    int first;
    int step;
    int stop;
    int curPlayer;
};

int sum(struct Board *board, int player) {
    if (player == 1) {
        return board->hole[0] + board->hole[1] + board->hole[2] + board->hole[3] + board->hole[4] + board->hole[5];
    } else {
        return board->hole[7] + board->hole[8] + board->hole[9] + board->hole[10] + board->hole[11] + board->hole[12];
    }
}

void initBoard(struct Board *board, int player) {
    int hole_init[14] = {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0};
    for (int i = 0; i < 14; ++i) {
        board->hole[i] = hole_init[i];
    }
    board->first = player;
    board->curPlayer = player;
    board->step = 0;
    board->stop = 0;
}

void copyBoard(struct Board *dest, struct Board *src) {
    for (int i = 0; i < 14; ++i) {
        dest->hole[i] = src->hole[i];
    }
    dest->first = src->first;
    dest->curPlayer = src->curPlayer;
    dest->step = src->step;
    dest->stop = src->stop;
}

int query(struct Board *board, int player, int pos) {
    int start = 6 * (player - 1) + pos - 1;
    return board->hole[start] != 0;
}

int sow(struct Board *board, int player, int pos) {
    if (player != board->curPlayer) {
        return 0;
    }
    ++board->step;
    int start = 6 * (player - 1) + pos - 1;
    int cnt = board->hole[start];
    board->hole[start] = 0;
    int now = start;
    for (int i = 0; i < cnt; ++i) {
        now = (now + 1) % 14;
        if ((player == 1 && now == 13) || (player == 2 && now == 6)) {
            --i;
            continue;
        }
        ++board->hole[now];
    }

    if (player == 1 && board->hole[now] == 1 && 0 <= now && now <= 5) {
        board->hole[6] += (board->hole[now] + board->hole[13 - now]);
        board->hole[now] = 0;
        board->hole[13 - now] = 0;
    } else if ((player == 2 && board->hole[now] == 1) && (7 <= now && now <= 12)) {
        board->hole[13] += (board->hole[now] + board->hole[13 - now]);
        board->hole[now] = 0;
        board->hole[13 - now] = 0;
    }

    if (!((player == 1 && now == 7) || (player == 2 && now == 13))) {
        board->curPlayer = 3 - player;
    }

    if (sum(board, 1) == 0) {
        board->hole[13] += sum(board, 2);
        for (int i = 0; i < 13; ++i) {
            board->hole[i] = 0;
        }
        board->stop = 1;
    } else if (sum(board, 2) == 0) {
        board->hole[6] += sum(board, 1);
        for (int i = 0; i < 13; ++i) {
            board->hole[i] = 0;
        }
        board->stop = 1;
    }
    return 1;
}

int isStop(struct Board *board) {
    return board->stop;
}

int score(struct Board *board) {
    return board->hole[7 * board->first - 1];
}

int scoreDifference(struct Board *board) {
    return board->hole[7 * board->first - 1] - board->hole[20 - 7 * board->first];
}

int getCurPlayer(struct Board *board) {
    return board->curPlayer;
}

int *getHole(struct Board *board) {
    return board->hole;
}

int getStep(struct Board *board) {
    return board->step;
}

struct Pair {
    struct Board* first;
    int second;
};

struct Pair makePair(struct Board* first, int second) {
    struct Pair p;
    p.first = first;
    p.second = second;
    return p;
}

#define QUEUE_LEN 100000000
struct Pair boardQueue[QUEUE_LEN];
int front, rear;

struct Pair popQueue() {
    struct Pair p;
    p.first = boardQueue[front % QUEUE_LEN].first;
    p.second = boardQueue[front % QUEUE_LEN].second;
    front++;
    return p;
}

void pushQueue(struct Board *board, int move) {
    boardQueue[rear % QUEUE_LEN].first = board;
    boardQueue[rear % QUEUE_LEN].second = move;
    rear++;
}

void initQueue() {
    front = rear = 0;
}

int decide(struct Board *board, int flag, int maxStep) {
    int maxScoreDifference = -49;
    int bestMove = 0;
    initQueue();
    pushQueue(board, 0);
    while (front != rear) {
        struct Pair p = popQueue();
        struct Board *nowBoard = p.first;
        int move = p.second;
        if (nowBoard->step == maxStep || isStop(nowBoard)) {
            int diff = scoreDifference(nowBoard) * (flag * -2 + 3);
            if (diff > maxScoreDifference) {
                maxScoreDifference = diff;
                bestMove = move;
            }
            free(nowBoard);
            continue;
        }
        int moves[] = {1, 2, 3, 4, 5, 6};
        for (int i = 0; i <= 5; i++) {
            if (query(nowBoard, getCurPlayer(nowBoard), moves[i])) {
                struct Board *nextBoard = (struct Board *)malloc(sizeof(struct Board));
                copyBoard(nextBoard, nowBoard);
                sow(nextBoard, getCurPlayer(nextBoard), moves[i]);
                pushQueue(nextBoard, (move == 0) ? moves[i] : move);
            }
        }
        free(nowBoard);
    }
    return bestMove;
}

int EMSCRIPTEN_KEEPALIVE mancalaOperator(int flag, int status[]) {
    struct Board *board = (struct Board *)malloc(sizeof(struct Board));
    initBoard(board, flag);
    int maxStep = 10;
    return decide(board, flag, maxStep);
}

//int main() {
//    clock_t start = clock();
//    int flag = 1;
//    int status[] = {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0};
//    int res = mancalaOperator(flag, status);
//    clock_t end = clock();
//    double time = ((double) end - (double) start) / CLOCKS_PER_SEC;
//    printf("%d, %f", res, time);
//    return 0;
//}