#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define MAX_SIZE (128 * 1024 * 1024)
char text[MAX_SIZE + 1];

int rand31() {
    return ((rand() << 16) ^ rand()) & 0x7fff;
}

char randChar() {
    return (char)(rand() % (127 - 32 + 1) + 32);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <seed> <minL> <maxL> <totalL>\n", argv[0]);
        return 1;
    }
    int seed = atoi(argv[1]);
    int minL = atoi(argv[2]);
    int maxL = atoi(argv[3]);
    int totalL = atoi(argv[4]);

    if (minL <= 0) {
        minL = 1;
    }
    if (maxL < 2 * minL + 1) {
        maxL = 2 * minL + 1;
    }
    assert(totalL >= minL);

    if (seed == -1) {
        seed = time(0);
    }
    fprintf(stderr, "seed = %d\n", seed);
    fprintf(stderr, "minL = %d, maxL = %d, totalL = %d\n", minL, maxL, totalL);
    srand(seed);

    size_t count = 0;
    size_t sumL = 0;
    while (sumL < totalL) {
        int curL;
        if (totalL - sumL < 2 * minL + 1) {
            curL = totalL - sumL;
        } else { // totalL - sumL >= 2 * minL + 2
            curL = (rand31() % (maxL - minL + 1)) + minL;
            if (sumL + curL + 1 + minL > totalL) {
                curL = totalL - sumL - minL - 1;
            }
        }
//        fprintf(stderr, "sumL = %d, curL = %d\n", (int)sumL, (int)curL);
        assert(minL <= curL && curL <= maxL);

        while (curL > 0) {
            text[sumL++] = randChar();
            --curL;
        }
        text[sumL++] = '\0';
        ++count;
    }

    FILE* textFile = fopen("indexing_file.txt", "w");

    size_t* starts = malloc(count * sizeof(size_t));
    size_t i, pos;
    for (i = 0, pos = 0; i < count; ++i) {
        size_t len = strlen(text + pos);
        if (i != 0) {
            fprintf(textFile, "\n");
        }
        fprintf(textFile, "%s", text + pos);
        starts[i] = pos;
        pos += len + 1;
    }
    fclose(textFile);

    FILE* queryFile = fopen("queries_file.txt", "w");
    FILE* answerFile = fopen("answers_file.txt", "w");
    sumL = 0;
    while (sumL < totalL) {
        if (rand() % 2 == 0) {
            int i = rand31() % count;
            fprintf(queryFile, "%s\n", text + starts[i]);
            fprintf(answerFile, "YES\n");
            sumL += strlen(text + starts[i]) + 1;
        } else {
            int curL = max(15, min(totalL - sumL, rand31() % (maxL - minL + 1) + minL));
            sumL += curL + 1;
            while (curL > 0) {
                fprintf(queryFile, "%c", randChar());
                --curL;
            }
            fprintf(queryFile, "\n");
            fprintf(answerFile, "NO\n");
        }
    }
    fprintf(queryFile, "exit\n");

    fclose(queryFile);
    fclose(answerFile);

    free(starts);

    return 0;
}
