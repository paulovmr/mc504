#include "common.h"

void clearScreen() {
    system("clear");
}

void gotoxy(int x, int y) {
	printf("%c[%d;%df", 0x1B, y, x);
}

void flush() {
	fflush(stdout);
}
