#include <stdio.h>
#include "LPC24XX.h"

#include <motion_controller.h>

#define LOG(X, Y) fprintf(fp,#X ": Time: %s, File: %s(line %d) " #Y "\n", __TIMESTAMP__, __FILE__, __LINE__)

int main(void) {

    FILE *fp = fopen("logfile.txt", "a");
    if (fp == NULL) {
        perror("Failed to open file");
    }
    
    //File open
    LOG(OPEN, "File open success.");

    fputs("Tram speed: \n", fp);
    fputs("Output voltage: \n\n", fp);

    if (fclose(fp) == 0) {
        LOG(ERROR, "File close succesful.");
    }

    // WORD1 [VELOCITY], WORD2 [BRAKE_DATA], WORD3 [WEATHER]
    

    return 0;
}
