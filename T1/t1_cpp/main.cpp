#include <cstdio>
#include <emscripten/emscripten.h>

using namespace std;

int bocchiShutUp(int flag, int seq[], int size) {
    int cnt[10] = {0};
    int maxx = 0, num = 0, count = 0;
    if (flag == 1) {
        for (int i = 0; i < size; ++i) {
            if (seq[i] < 20) {
                cnt[seq[i] % 10]++;
            }
        }
        for (int i = 1; i <= 6; ++i) {
            if (cnt[i] > maxx) {
                maxx = cnt[i];
                num = 10 + i;
                count = 1;
            } else if (cnt[i] == maxx) {
                ++count;
            }
        }
        if (count > 1) {
            return 10;
        } else if (count == 1) {
            return num;
        }
    } else if (flag == 2) {
        for (int i = 0; i < size; ++i) {
            if (seq[i] > 20) {
                cnt[seq[i] % 10]++;
            }
        }
        for (int i = 1; i <= 6; ++i) {
            if (cnt[i] > maxx) {
                maxx = cnt[i];
                num = 20 + i;
                count = 1;
            } else if (cnt[i] == maxx) {
                ++count;
            }
        }
        if (count > 1) {
            return 10;
        } else if (count == 1) {
            return num;
        }
    }
    return 10;
}

int main() {
    return 0;
}
