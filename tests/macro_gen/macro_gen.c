#include <stdint.h>
#include <stdio.h>

void draw_pattern(long line, int label_length, FILE *fp);

int main(void) {
    int const label_length = 18;

    FILE *fp = fopen("vel_label.txt", "w");

    for (long i = 0; i < (long)(2 << label_length); i++) {
        fprintf(fp, "#define ");
        draw_pattern(i, label_length, fp);
        fprintf(fp, "	0x%x\n", i);
    }
}

void draw_pattern(long line, int label_length, FILE *fp) {
    for (long bit = label_length - 1; bit >= 0; bit--) {
        if ((long)((line >> bit) & 1)) {
            fprintf(fp, "X");
        } else {
            fprintf(fp, "_");
        }
    }
}