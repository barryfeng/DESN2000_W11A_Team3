    
#include <stdio.h>

int factorial(int a) {
    int result = 1;

    int i = a;
    while (i > 0) {
        result *= i;
        i -= 1;
    }

    printf("%d\n", result);
}

int main(void) {
    int const first[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++) {
        factorial(i);
    }

    return 0;
}
