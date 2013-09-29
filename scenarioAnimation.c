#include "scenarioAnimation.h"

void drawScenarioLine(int l) {
	if (l >= 0) {
		gotoxy(l, 0);
	}
	
	printf(GRASS "         " SHORE " " RIVER "                                                                                         " SHORE " " GRASS "         " BG_BLACK "\n");
}

void drawScenario(int boats, int boatCapacity) {
	int i, j;
	
	j = 3 * (boatCapacity / 2);
	if (j < 6) j = 6;
	for (i = 0; i < j * boats + 1; i++) {
		drawScenarioLine(-1);
	}
}
