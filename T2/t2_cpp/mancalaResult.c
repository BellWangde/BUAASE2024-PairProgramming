#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>

#define HOLE_SIZE 14

typedef struct {
    int hole[HOLE_SIZE];
    int first;
    bool stop;
    int curPlayer;
} Board;

int sum(int *hole, int player) {
    if (player == 1) {
        return hole[0] + hole[1] + hole[2] + hole[3] + hole[4] + hole[5];
    } else {
        return hole[7] + hole[8] + hole[9] + hole[10] + hole[11] + hole[12];
    }
}

void initialize(Board *board, int player) {
    int i;
    for (i = 0; i < HOLE_SIZE; ++i) {
        if (i != 6 && i != 13) {
            board->hole[i] = 4;
        } else {
            board->hole[i] = 0;
        }
    }
    board->first = board->curPlayer = player;
    board->stop = false;
}

bool sow(Board *board, int player, int pos) {
    if (player != board->curPlayer) {
        return false;
    }
    int start = 7 * player + pos - 8;
    int cnt = board->hole[start];
    board->hole[start] = 0;
    int now = start;
    int i;
    for (i = 0; i < cnt; ++i) {
        now = (now + 1) % HOLE_SIZE;
        if ((player == 1 && now == 13) || (player == 2 && now == 6)) {
            --i;
            continue;
        }
        ++board->hole[now];
    }

    if (player == 1 && board->hole[now] == 1 && 0 <= now && now <= 5 && board->hole[12 - now] > 0) {
        board->hole[6] += (board->hole[now] + board->hole[12 - now]);
        board->hole[now] = 0;
        board->hole[12 - now] = 0;
    } else if ((player == 2 && board->hole[now] == 1) && (7 <= now && now <= 12) && board->hole[12 - now] > 0) {
        board->hole[13] += (board->hole[now] + board->hole[12 - now]);
        board->hole[now] = 0;
        board->hole[12 - now] = 0;
    }

    if (!((player == 1 && now == 6) || (player == 2 && now == 13))) {
        board->curPlayer = 3 - player;
    }

    if (sum(board->hole, 1) == 0) {
        board->hole[13] += sum(board->hole, 2);
        for (int i = 0; i < 13; ++i) {
            if (i != 6) {
                board->hole[i] = 0;
            }
        }
        board->stop = 1;
    } else if (sum(board->hole, 2) == 0) {
        board->hole[6] += sum(board->hole, 1);
        for (int i = 0; i < 13; ++i) {
            if (i != 6) {
                board->hole[i] = 0;
            }
        }
        board->stop = 1;
    }
    return true;
}

bool isStop(Board *board) {
    return board->stop;
}

int score(Board *board) {
    return board->hole[7 * board->first - 1];
}

int scoreDifference(Board *board) {
    return board->hole[7 * board->first - 1] - board->hole[20 - 7 * board->first];
}

int getCurPlayer(Board *board) {
    return board->curPlayer;
}

int *getHole(Board *board) {
    return board->hole;
}

int EMSCRIPTEN_KEEPALIVE mancalaResult(int flag, int seq[], int size) {
    Board *board = malloc(sizeof(Board));
    initialize(board, flag);
    int i;
    for (i = 0; i < size; ++i) {
        int player = seq[i] / 10;
        int pos = seq[i] % 10;
        if (!sow(board, player, pos)) {
            free(board);
            return 30000 + i;
        }
    }

    if (isStop(board)) {
        int sc = scoreDifference(board);
        free(board);
        return 15000 + sc;
    } else {
        int sc = score(board);
        free(board);
        return 20000 + sc;
    }
}
