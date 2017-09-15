#include "FCB.h"
#include "Drumbot.h"
#include "Data_Conversions.h"
#include "UI_Pushbuttons.h"
#include "MIDI.h"
#include <stdio.h>


// ----------------------------------------
// Variable Declarations
// ----------------------------------------
DRUM_TYPE Drum[_DRUM_COUNT];

// ----------------------------------------
// Initialization
// ----------------------------------------
void Drum_Setup(DRUM_TYPE* s, UINT8 chan, UINT8 hold_time) {
	s->Hold_Time = hold_time;
	s->Hold_Timer = 0;
	s->Chan = chan;
}

// ----------------------------------------
// Periodic functions
// ----------------------------------------
void Drums_Process(void) {
	UINT8 x;
	for (x = 0; x < _DRUM_COUNT; x++) {
		Drum_Process(&Drum[x]);
	}
}

// Process a drum structure to see if it's time to disengage
void Drum_Process(DRUM_TYPE* s) {
	// Drum isn't engaged?  Then leave!
	if (!(s->Holding)) return;

	// Decrement the holding timer and check for expiration
	if (s->Hold_Timer > 0) {
		s->Hold_Timer--;
		return;
	}
	// If hold timer has expired then disengage the channel
	s->Holding = 0;

	// Check to see if channel is PWM tone generator
	if ((s->Chan) > 11) {
		//period = PWM1_Period - ((s->Chan)-101)*5000;
		switch(s->Chan) {
			case 12 :
				PWM_Set_Duty(2, 0);
				return;
			case 13 :
				PWM_Set_Duty(7, 0);
				return;
		}
		return;
	}

	// If channel is PWM, set duty to zero
	if ((s->Chan) > 100) {
		PWM_Set_Duty(7, 0);
		return;
	}
	// Check to see if channel is a sinking output
	if ((s->Chan) > 7) {
		switch(s->Chan) {
			case 8 :
				_FCB_PWM3_SNK = 0;
				return;
			case 9 :
				_FCB_PWM4_SNK = 0;
				return;
			case 10 :
				_FCB_PWM5_SNK = 0;
				return;
			case 11 :
				_FCB_PWM6_SNK = 0;
				return;

		}
		return;
	}

	// If not PWM then channel is sourcing, so disable via SRC function
	FCB_SRC_OFF(s->Chan);
	// Also turn off the appropriate indicator lamp
	Light_Off(s->Chan);
}

// ----------------------------------------
// Activate a drum output
// ----------------------------------------
void Drum_Hit(DRUM_TYPE* s) {
	UINT16 period;

	// Check for invalid channel
	if ((s->Chan) == 0) return;

	// Setup delay until release
	s->Holding = 1;
	s->Hold_Timer = s->Hold_Time;

	// Check to see if channel is a remote output
	if ((s->Chan) >= 100) {
		MIDI_Hit((s->Chan) - 100);
		return;
	}


	// At some point you should code up a buffered output so that
	//   physical delay times can be compensated for
	// For example, all notes have a default delay of 1 second
	//   that delay can be reduced on actuators with slow responses


	// First check for the sinking and pwm outputs
	if ((s->Chan) > 7) {
		switch(s->Chan) {
			case 8 :
				_FCB_PWM3_SNK = 1;
				return;
			case 9 :
				_FCB_PWM4_SNK = 1;
				return;
			case 10 :
				_FCB_PWM5_SNK = 1;
				return;
			case 11 :
				_FCB_PWM6_SNK = 1;
				return;
			// PWM tone generator
			case 12 :
				PWM_Set_Period(2, PWM1_Period/2);
				PWM_Set_Duty(2, PWM1_Period/4);
				return;
			// PWM tone generator
			case 13 :
				PWM_Set_Period(7, PWM1_Period);
				PWM_Set_Duty(7, PWM1_Period/2);
				return;
		}
		return;
	}

	// Turn on output and indicator LED
	FCB_SRC_ON(s->Chan);
	Light_On(s->Chan);
}

// ----------------------------------------
// Indicator Lights Control
// ----------------------------------------
void Light_On(UINT8 index) {
	UINT8 x = 1;
	x <<= index;
	FCB.Lights_Req.Byte |= x;
}

void Light_Off(UINT8 index) {
	UINT8 x = 1;
	x <<= index;
	FCB.Lights_Req.Byte &= ~(x);
}

