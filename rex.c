#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


struct termios original_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

int enable_raw_mode() {

    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        printf("Error getting termios struct\n");
        return -1;
    }
    atexit(disable_raw_mode);

    struct termios raw = original_termios;

    raw.c_lflag = raw.c_lflag & ~(ICANON | ECHO | ISIG | IEXTEN);  // local flags
    raw.c_iflag = raw.c_iflag & ~(IXON | ICRNL); // input flags
    raw.c_oflag = raw.c_oflag & ~(OPOST); // output flags
    //raw.c_cflag = raw.c_cflag & ~ // control flags

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
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
    }

    return 0;
}