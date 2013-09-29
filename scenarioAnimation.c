#include "scenarioAnimation.h"

void drawScenarioLine(int l) {
	if (l >= 0) {
		gotoxy(l, 0);
	}
	
	printf(GRASS "         " SHORE " " RIVER "                                                                                         " SHORE " " GRASS "         " BG_BLACK "\n");
}

void drawScenario(int boats) {
	int i;
	
	for (i = 0; i < BOAT_HEIGHT * boats + 1; i++) {
		drawScenarioLine(-1);
	}
}
