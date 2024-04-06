#include <stdlib.h>
#include <string.h>
//#include "stdio.h"
#include <emscripten/emscripten.h>

struct Board {
    int hole[14];
    int first;
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
    memcpy(board->hole, hole_init, sizeof(hole_init));
    board->first = board->curPlayer = player;
    board->stop = 0;
}

int sow(struct Board *board, int player, int pos) {
    if (player != board->curPlayer) {
        return 0;
    }

    int start = 7 * player + pos - 8;
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

    if (sum(board, 1) == 0) {
        board->hole[13] += sum(board, 2);
        for (int i = 0; i < 13; ++i) {
            if (i != 6) {
                board->hole[i] = 0;
            }
        }
        board->stop = 1;
    } else if (sum(board, 2) == 0) {
        board->hole[6] += sum(board, 1);
        for (int i = 0; i < 13; ++i) {
            if (i != 6) {
                board->hole[i] = 0;
            }
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

int scoreDifference(struct Board *board, int flag) {
    if (flag) {
        return board->hole[7 * board->first - 1] - board->hole[20 - 7 * board->first];
    } else {
        return board->hole[6] - board->hole[13]; // 玩家1的净胜
    }
}

int getCurPlayer(struct Board *board) {
    return board->curPlayer;
}

int *getHole(struct Board *board) {
    int *hole = (int *)malloc(14 * sizeof(int));
    memcpy(hole, board->hole, 14 * sizeof(int));
    return hole;
}

// EMSCRIPTEN_KEEPALIVE
int * EMSCRIPTEN_KEEPALIVE mancalaBoard(int flag, int seq[], int size) {
    struct Board board;
    initBoard(&board, seq[0] / 10);
    int *result = (int *)malloc(15 * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int player = seq[i] / 10;
        int pos = seq[i] % 10;
        if (i == size - 1) {
            if (player != getCurPlayer(&board) || player != flag) {
                memcpy(result, getHole(&board), 14 * sizeof(int));
                result[14] = 200 + scoreDifference(&board,0);
                return result;
            }
        }
        sow(&board, player, pos);
    }
    memcpy(result, getHole(&board), 14 * sizeof(int));
    if (isStop(&board)) {
        result[14] = 200 + scoreDifference(&board, 0);
    } else {
        result[14] = getCurPlayer(&board);
    }
    return result;
}

//int main() {
//    int flag = 1;
//    int seq[] = {13};
//    int size = 1;
//    int *ptr = mancalaBoard(flag, seq, size);
//    for (int i = 0; i < 15; ++i) {
//        printf("%d ", ptr[i]);
//    }
//}