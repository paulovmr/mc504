#include "boatAnimation.h"

void drawStoppedBoat(Boat* boat) {
    int x = boat->x;
    int y = boat->y;
    int capacity = boat->capacity;
    int i;

    gotoxy(x + 3 * capacity - 1, y);     printf("      ");
    gotoxy(x + 3 * capacity - 1, y + 1); printf("      ");
    gotoxy(x + 3 * capacity - 1, y + 2); printf("       " BOAT "/ ");
    gotoxy(x + 3 * capacity - 1, y + 3); printf("______/ ");
    gotoxy(x + 3, y + 4); 
    for (i = 0; i < 3 * capacity - 6; i++) {
        printf(" ");
    }
    printf("         " FG_WHITE BG_BLACK);    
}

void drawBoatWithOarToLeft(Boat* boat) {
    int x = boat->x;
    int y = boat->y;
    int capacity = boat->capacity;
    int qtd = boat->qtd;
    int i;

    if (boat->people[qtd - 1] == SERF) {
        gotoxy(x + 3 * capacity - 1, y);     printf(BOAT "   _" SERF_COLOR "O ");
    } else {
        gotoxy(x + 3 * capacity - 1, y);     printf(BOAT "   _" HACKER_COLOR "O ");
    }
    gotoxy(x + 3 * capacity - 1, y + 1); printf(BOAT "  / " BODY "| ");
    gotoxy(x + 3 * capacity - 1, y + 2); printf(BOAT " / " BODY "/ \\ " BOAT "/");
    gotoxy(x + 3 * capacity - 1, y + 3); printf(BOAT "/_____/");
    gotoxy(x + 3, y + 4); 
    for (i = 0; i < 3 * capacity - 6; i++) {
        printf(" ");
    }
    printf(BOAT " O       " FG_WHITE BG_BLACK);    
}

void drawBoatWithOarToCenter(Boat* boat) {
    int x = boat->x;
    int y = boat->y;
    int capacity = boat->capacity;
    int qtd = boat->qtd;
    int i;

    if (boat->people[qtd - 1] == SERF) {
        gotoxy(x + 3 * capacity - 1, y);     printf(BODY "  __" SERF_COLOR "O ");
    } else {
        gotoxy(x + 3 * capacity - 1, y);     printf(BODY "  __" HACKER_COLOR "O ");
    }
    gotoxy(x + 3 * capacity - 1, y + 1); printf(BOAT " |  " BODY "| ");
    gotoxy(x + 3 * capacity - 1, y + 2); printf(BOAT " | " BODY "/ \\ " BOAT "/ ");
    gotoxy(x + 3 * capacity - 1, y + 3); printf(BOAT "_|____/ ");
    gotoxy(x + 3, y + 4); 
    for (i = 0; i < 3 * capacity - 6; i++) {
        printf(" ");
    }
    printf(BOAT "   O    " FG_WHITE BG_BLACK);    
}

void drawBoatWithOarToRight(Boat* boat) {
    int x = boat->x;
    int y = boat->y;
    int capacity = boat->capacity;
    int qtd = boat->qtd;
    int i;
    
    if (boat->people[qtd - 1] == SERF) {
        gotoxy(x + 3 * capacity - 1, y);     printf(BOAT " ___" SERF_COLOR "O ");
    } else {
        gotoxy(x + 3 * capacity - 1, y);     printf(BOAT " ___" HACKER_COLOR "O ");
    }
    gotoxy(x + 3 * capacity - 1, y + 1); printf(BOAT " \\  " BODY "|");
    gotoxy(x + 3 * capacity - 1, y + 2); printf(BOAT "  \\" BODY "/ \\ " BOAT "/");
    gotoxy(x + 3 * capacity - 1, y + 3); printf(BOAT "___\\__/");
    gotoxy(x + 3, y + 4); 
    for (i = 0; i < 3 * capacity - 6; i++) {
        printf(" ");
    }
    printf(BOAT "      O     " FG_WHITE BG_BLACK);    
}

void drawBoat(Boat* boat, pthread_mutex_t* mutex_sail) {
    int x = boat->x;
    int y = boat->y;
    int status = boat->status;
    int capacity = boat->capacity;
    int qtd = boat->qtd;
    int i;

    pthread_mutex_lock(mutex_sail);
    
    gotoxy(x, y + 1); printf(RIVER BOAT "  ");
    gotoxy(x, y + 2); printf(RIVER BOAT " \\ ");
    gotoxy(x, y + 3); printf(BOAT "  \\ ");

    if (status == 0) {
        qtd++;
    }
    
    for (i = 1; i < qtd; i++) {
        if (boat->people[i - 1] == SERF) {
            gotoxy(x + 3 * i, y + 1); printf(SERF_COLOR " O ");
        } else {
            gotoxy(x + 3 * i, y + 1); printf(HACKER_COLOR " O ");
        }
    
        gotoxy(x + 3 * i, y + 2); printf(BODY "_| ");
        gotoxy(x + 3 * i, y + 3); printf(BOAT "___");
    }
    
    for (i = qtd; i <= capacity; i++) {
        gotoxy(x + 3 * i, y + 1); printf("   ");
        gotoxy(x + 3 * i, y + 2); printf("   ");
        gotoxy(x + 3 * i, y + 3); printf("___");
    }
    
    switch (status) {
        case 0 : drawStoppedBoat(boat);
                 break;
        case 1 : drawBoatWithOarToRight(boat);
                 break;
        case 2 : drawBoatWithOarToCenter(boat);
                 break;
        case 3 : drawBoatWithOarToLeft(boat);
                 break;
    }
    
    gotoxy(0, 0);
    flush();
    
    pthread_mutex_unlock(mutex_sail);
}

void animateStoppedBoat(Boat* boat, pthread_mutex_t* mutex_sail) {
    drawBoat(boat, mutex_sail);
}

void animateBoatTravel(Boat* boat, pthread_mutex_t* mutex_sail) {
    int capacity = boat->capacity;
    
    eraseBoatDelivery(boat, mutex_sail);
    
    while (1) {
        if (boat->status > 1) {
            boat->x++;
        }

        drawBoat(boat, mutex_sail);
    
        if (boat->x == 92 - 3 * capacity) {
            boat->status = 0; // Parado
            break;
        } else {
            boat->status = (boat->status % 3) + 1;
        }
        
        usleep(100000);
    }
    
    drawBoatDelivery(boat, mutex_sail);
    boat->qtd = 0;
    drawBoat(boat, mutex_sail);
    sleep(2);
    
    while (1) {
        drawBoat(boat, mutex_sail);
        
        if (boat->x == 11) {
            break;
        }
        
        boat->x--;
        
        usleep(50000);
    }
}

