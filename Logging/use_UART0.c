/**
 * FILENAME :        use_UART0.c
 * DESCRIPTION :     this file calls the assmebly code for transmission via UART0
 * 
 * NOTES :           use this file when UART0 transmissison is needed
 * 
 * AUTHOR :          Adam Witt   
 * 
 * START DATE :      12 Jul 20
 * 
 * CHANGES :
 * -- 12/07/2020 --  File created.
 */

extern void UART0_start(char *input);

int main(void) {
    char *string = "need to input string";
    
    int i = 0;
    while (string[i] != '\0') {
        UART0_start(string);
        i++;
    }

    while(1) {}     // Do nothing more

    return 0;
}

// NOT CURRENTLY USING
static void transmit(char *message) {
    //Receive string and transmit it using UART0

}
