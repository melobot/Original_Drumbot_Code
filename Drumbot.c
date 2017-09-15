#include "FCB.h"
#include "Drumbot.h"
#include "Pattern_Strings.h"
#include "Data_Conversions.h"
#include "UI_Pushbuttons.h"
#include <stdio.h>

// ----------------------------------------
// Variable Declarations
// ----------------------------------------
UINT16 PWM1_Period = 60000;
UINT16 PWM2_Period = 60000;
UINT16 Tone_Hz = 999;
UINT16 Drums_Paused = 0;
UINT16 Global_Tempo = _MS(10);

//char Song_1[] = "W6Z0 X6Z0 Y6Z0 Z6Z0";

// Drumbot
char Song_1[] = "U U U V";	// Bass Drum
char Song_2[] = "Z Z C";
char Song_3[] = "Z J Z Z W";

// Xylophone stuff
//char Song_1[] = "G Z Z Z Z F F F Z Z G G G";
//char Song_2[] = "Z Z Z Z Z Z D D D D D Z Z";

// Fun gong stuff
//char Song_3[] = "I I H Z Z Z Z H Z";


// Da beat
//char Song_4[] = "Z Z Z K K K K K K";
//char Song_5[] = "Z Z Z L L L L L L";

// ----------------------------------------
// Initialization
// ----------------------------------------
void Drumbot_Init(void) {
	printf("Drumbot init\r\n");
	memset(&Drum, 0, sizeof(Drum));
	memset(&Bot, 0, sizeof(Drum));

	// Enable the menu pushbuttons
	PB_Press_Fxn_Push(&PB_OK, Drumbot_PB_Trap_OK, _PB_TRIGGER_QUICK);
	PB_Press_Fxn_Push(&PB_Back, Drumbot_PB_Trap_Back, _PB_TRIGGER_QUICK);
	PB_Press_Fxn_Push(&PB_Next, Drumbot_PB_Trap_Next, _PB_TRIGGER_QUICK);

	// Setup the drum data structures
	// These structures contain data about hardware and timing
	Drum_Setup(&Drum[0], 1, 30);	// 15
	Drum_Setup(&Drum[1], 2, 30); // 15
	Drum_Setup(&Drum[2], 3, 30); // 12
	Drum_Setup(&Drum[3], 4, 30);
	Drum_Setup(&Drum[4], 5, 30);
	Drum_Setup(&Drum[5], 6, 30);
	Drum_Setup(&Drum[6], 7, 30);
	Drum_Setup(&Drum[7], 8, 30);
	// Sinking outputs
	Drum_Setup(&Drum[8], 8, 20);
	Drum_Setup(&Drum[9], 9, 15);
	Drum_Setup(&Drum[10], 10, 10);
	Drum_Setup(&Drum[11], 11, 10);
	// PWM Tone Outputs
	Drum_Setup(&Drum[12], 12, 100);
	Drum_Setup(&Drum[13], 13, 100);

//	Drum_Setup(&Drum[8], 101, 100);
//	Drum_Setup(&Drum[9], 102, 100);

	// Setup the bots.
	//  These are the constructs that read the strings and trigger the notes
	Bot_Setup(&Bot[0], Song_1, 13, 0);
	Bot_Setup(&Bot[1], Song_2, 13, 0);
	Bot_Setup(&Bot[2], Song_3, 13, 0);
//	Bot_Setup(&Bot[3], Song_4, 15, 0);
//	Bot_Setup(&Bot[4], Song_5, 15, 0);

	// Tone generator via PWM
	PWM_Timebase_Init(0xFFFF, 3);
	//PWM_Simple_Init(7, 25000);
	PWM_Simple_Init(7, 0);
	PWM_Simple_Init(2, 0);

	// Cogs are configured, now set them in motion
	Scheduler_Process_Add(Bots_Process, _MS(10));
	Scheduler_Process_Add(Drums_Process, _MS(1));
}

// Tempo adjustments, slow increase, slow decrease


// -------------------------------------------------
// Pushbutton callbacks
// -------------------------------------------------
void Drumbot_PB_Trap_OK(void) {
	if (Drums_Paused == 0) {
		// stop the music
		Scheduler_Process_Halt(Bots_Process);
		Drums_Paused = 1;
	} else {
		// resume the music
		Scheduler_Process_Run(Bots_Process);
		Drums_Paused = 0;
	}

	printf("OK\n\r");
}

void Drumbot_PB_Trap_Next(void) {
	// exit if we are at the limit
	if (PWM1_Period >= ((0xFFFF / 3) * 2)) return;

	// scale the period by 3/2 to drop a fifth
	PWM1_Period += PWM1_Period / 2;

	Tone_Hz = 460630 / PWM1_Period;

	// Should be about 27 steps in 1 - 0xFFFF
	// About 10 steps if you go from 1000 - 0xFFFF
	printf("<Next> Hz=%u\n\r",Tone_Hz);
}

void Drumbot_PB_Trap_Back(void) {
	// exit if we are at the limit
	if (PWM1_Period <= (250)) return;

	// scale the period by 2/3 to go up a fifth
	PWM1_Period -= PWM1_Period / 3;

	Tone_Hz = 460630 / PWM1_Period;

	// Should be about 27 steps in 1 - 0xFFFF
	// About 10 steps if you go from 1000 - 0xFFFF
	printf("<Back> Hz=%u\n\r",Tone_Hz);
}
