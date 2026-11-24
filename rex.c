#include <stdio.h>
#include <sys/ttycom.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <ctype.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct rexConfig {
    struct termios original_termios;
    struct winsize window_size;
};

struct rexConfig R;


void clean_up(const char *s);
void disable_raw_mode();
void enable_raw_mode();
char rexReadKey();
void rexDrawRows();
void rexRefreshScreen();
void rexProcessKeyPress();

// Error Exit Logic.
void clean_up(const char *s){
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &R.original_termios) == -1) clean_up("Error setting terminal back to normal");
}

void enable_raw_mode() {

    if (tcgetattr(STDIN_FILENO, &R.original_termios) == -1) clean_up("Error getting termios struct");
    
    atexit(disable_raw_mode);

    struct termios raw = R.original_termios;

    raw.c_lflag = raw.c_lflag & ~(ICANON | ECHO | ISIG | IEXTEN);  // local flags
    raw.c_iflag = raw.c_iflag & ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP); // input flags
    raw.c_oflag = raw.c_oflag & ~(OPOST); // output flags
    raw.c_cflag = raw.c_cflag | ~(CS8); // control flags
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 100;

    if ( tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) clean_up("Error setting termios struct");
}

char rexReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) clean_up("read error");
    }
    return c;
}

void rexDrawRows(){
    int y;
    for (y = 0; y < 24; y++) {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void rexRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    rexDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}


void rexProcessKeyPress() {
    char c = rexReadKey();
    switch (c) {
        case CTRL_KEY('l'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
    }
}

int main() {
    enable_raw_mode();

    while (1) { 
        rexRefreshScreen();
        rexProcessKeyPress();
    }

    return 0;
}