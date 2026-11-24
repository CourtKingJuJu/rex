# rex

Systems Programming Project Rex. A custom text editor.

## Notes

To have the terminal taken over like when you do vim my_file. you need to build a terminal user interface this requires putting the terminal into something called raw mode and switch to the alternate screen buffer.

## Learnings

- Terminal is naturally in canonical / cooked mode. In this mode text is only sent to your program when the user hits enter. this is normally a good thing but a problem for fine grain control through the terminal.
- Pointers give two big advantages the fact that we can pass address and save memory, and that it gives you the ability to modify the original object not just the copy.
- ~ is the bitwise not operator we can use it here to turn off flags by using the current binary flag the inverse (~) of the current binary flag and using the AND operator on them. The important flags we need to turn off to enter raw mode is: 1. Echo. Disabling echo is how you write text but the terminal doesn't show anything back to you. This is the same thing that happens to your terminal when you type sudo. 2. ICANON this flag lets us read each input byte by byte instead of when you press enter. 3. ISIG this flag stops things like control c and control z from sending SIGINT and SIGTSTP to the terminal that would normally suspend the process. 4. IEXTEN this flag turns off another control command control v and control o. this allows them to be read as a byte.
- The input flags we need to turn off to enter raw mode are as follows. 1. IXON this is to turn off flow control output. things like control s and control q stop and start data flow respectfully. Back in time this could be used to let a device like a printer catch up from data. 2. ICRNL The flag is required to stop the terminal from taking every carriage return (carriage is a thing that moves the cursor) and replaces it with a new line. This causes problems because it reads the byte 10 but we want to read control m (13) instead.
- output flags. 1. OPOST this flag is responsible for adding a carriage \r when you print a new line using \n (\r\n). we want to remove this so it isn't done automatically when a new line is created.
- BRKINT, INPCK, and ISTRIP these don't really have a visual effect but back in the day were considered needed to do to set a terminal to raw mode so they get included.
- CS8 is a bit mask this sets the character size for bits to 8 bits per byte.
- We require errno because this value gets in Portable operating system interfaces POSIX when a function returns with -1 it sets to a positive value. In our case we are letting the text editor timeout and return -1 but it sets the errno to EAGAIN so we need to treat this as a normal input.
- Control key macro using define. This way we can pass hard coded characters and check vs the byte we read. This helps us avoid writing Hex directly and adds a level of abstraction to the code.
- \x1b this byte is how we start escape sequences followed by a single left bracket every time.
- Most of the escape sequences for this project come from this lib  VT100. This doesn't support all terminals but this is widely supported and documented well for most terminals.
