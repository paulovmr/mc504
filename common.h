#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Colors - Background
#define BG_GREEN "\033[48;5;2m"
#define BG_RED "\033[48;5;1m"
#define BG_LIGHT_MAGENTA "\033[48;5;13m"
#define BG_LIGHT_BLUE "\033[48;5;14m"
#define BG_BLACK "\033[48;5;0m"

// Colors - Foreground
#define FG_BLUE "\033[38;5;6m"
#define FG_YELLOW "\033[38;5;3m"
#define FG_RED "\033[38;5;1m"
#define FG_LIGHT_RED "\033[38;5;9m"
#define FG_MAGENTA "\033[38;5;5m"
#define FG_BLACK "\033[38;5;0m"
#define FG_WHITE "\033[38;5;15m"

// Colors
#define GRASS BG_GREEN
#define SHORE BG_RED
#define RIVER BG_LIGHT_BLUE
#define NOTHING BG_BLACK
#define BOAT FG_RED
#define OAR FG_MAGENTA
#define LINUX_HACKER_COLOR FG_MAGENTA
#define MICROSOFT_EMPLOYEE_COLOR FG_YELLOW
#define BODY FG_BLACK

// People Type
#define LINUX_HACKER 0
#define MICROSOFT_EMPLOYEE 1

// Size variables
#define BOAT_HEIGHT 6
#define STANDUP_PERSON_HEIGHT 3
#define STANDUP_PERSON_WIDTH 3

typedef struct Boat {
	int position;
	int x;
	int y;
	int status;
	int capacity;
	int qtd;
	int* people;
    int hackers;
    int serfs;
    int isSailing;
    pthread_barrier_t barrier;
} Boat;

void clearScreen();

void gotoxy(int x, int y);

void flush();


