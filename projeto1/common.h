#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Background Colors
#define BG_GREEN      "\033[48;5;2m"
#define BG_RED        "\033[48;5;1m"
#define BG_LIGHT_BLUE "\033[48;5;14m"
#define BG_BLACK      "\033[48;5;0m"

// Foreground Colors
#define FG_YELLOW  "\033[38;5;3m"
#define FG_RED     "\033[38;5;1m"
#define FG_MAGENTA "\033[38;5;5m"
#define FG_BLACK   "\033[38;5;0m"
#define FG_WHITE   "\033[38;5;15m"

// Scenario Colors
#define GRASS        BG_GREEN
#define SHORE        BG_RED
#define RIVER        BG_LIGHT_BLUE
#define BOAT         FG_RED
#define OAR          FG_MAGENTA
#define HACKER_COLOR FG_MAGENTA
#define SERF_COLOR   FG_YELLOW
#define BODY         FG_BLACK

// People Type
#define HACKER 0
#define SERF   1

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

/* Limpa o terminal para o início da animação */
void clearScreen();

/* Posiciona o cursor em (x, y) */
void gotoxy(int x, int y);

/* Força a escrita em buffer na tela */
void flush();

