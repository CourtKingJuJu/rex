#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int enable_raw_mode() {
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, &raw) == -1) {
        printf("Error updating termios struct\n");
        return -1;
    }

    raw.c_lflag = raw.c_lflag & ~(ECHO); 

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    return 1;
}

int main() {

    if (!enable_raw_mode()) {
        printf("Error enabling raw mode");
        return 0;
    }

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        printf("%d\n", c);
    }
    printf("\n%d\n", c);
    return 0;
}