#include <stdio.h>

int main() {
    int m = 10;
    while (1) {
        m--;
        printf("%d\n", m);
        if (m == 0) {
            break;
        }
    }
    return 0;
}
