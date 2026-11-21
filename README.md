# rex

Systems Programming Project Rex. A custom text editor.

## Notes

To have the terminal taken over like when you do vim my_file. you need to build a terminal user interface this requires putting the terminal into something called raw mode and switch to the alternate screen buffer.

## Learnings

- Terminal is naturally in canonical / cooked mode. In this mode text is only sent to your program when the user hits enter. this is normally a good thing but a problem for fine grain control through the terminal.
- Pointers give two big advantages the fact that we can pass address and save memory, and that it gives you the ability to modify the original object not just the copy.
- ~ is the bitwise not operator we can use it here to turn off flags by using the current binary flag the inverse (~) of the current binary flag and using the AND operator on them. The important flags we need to turn off to enter raw mode is: 1. Echo. Disabling echo is how you write text but the terminal doesn't show anything back to you. This is the same thing that happens to your terminal when you type sudo.
