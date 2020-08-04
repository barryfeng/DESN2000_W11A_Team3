#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define BUF_SIZE 56

char* replace_char(char* str, char find, char replace);

int main(void) {

    FILE* fp = fopen("brakefont.txt", "r");
    FILE* fp2 = fopen("newbrake.txt", "w");
    char buf[BUF_SIZE];

    while (!feof(fp)) {
        char *pEnd;
        fgets(buf, BUF_SIZE, fp);
        replace_char(buf, 'X', '1');
        replace_char(buf, '_', '0');

        for (int i = 0; buf[i] != '\0'; i++) {
            fprintf(fp2, "%c", buf[i]);
        }

        // printf("num: %ull\n", strtoull(buf, &pEnd, 2));

        printf("\n");
    }
}

char* replace_char(char* str, char find, char replace) {
    char* current_pos = strchr(str, find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}
