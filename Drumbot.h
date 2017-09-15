#ifndef _DRUMBOT_H_
#define _DRUMBOT_H_

#include "Common_Definitions.h"

// -------------------------------------------------------------------------
// Definitions
// -------------------------------------------------------------------------
#define _BOT_COUNT 5
#define _DRUM_COUNT 14
#define _CHAN_COUNT 14

#define _DRUMBOT_PATTERN_LENGTH		12
#define _DRUMBOT_DATA_LENGTH		16

// -------------------------------------------------------------------------
// Type declarations
// -------------------------------------------------------------------------
typedef struct {
	char* Str_Ptr;			// Current string
	char* Char_Ptr;			// Current character within string
	UINT16 Loop_Counter;
} STRING_STRUCT;

typedef struct {
	UINT8 Tick_Timer;
	UINT8 Tick_Time;
	UINT8 Note_Offset;
	UINT8 Last_Note;
	UINT8 Swing_Dat;
	UINT16 String_Counter;
	UINT16 Note_Counter;
	STRING_STRUCT Melody_Str;
	STRING_STRUCT Song_Str;
	unsigned Enabled : 1;
} BOT_TYPE;

typedef struct {
	UINT8 Chan;
	UINT8 Hold_Time;
	UINT8 Hold_Timer;
	unsigned Holding : 1;
} DRUM_TYPE;


// -------------------------------------------------------------------------
// Variable Sharing
// -------------------------------------------------------------------------
extern DRUM_TYPE Drum[_DRUM_COUNT];
extern BOT_TYPE Bot[_BOT_COUNT];
extern UINT16 PWM1_Period;
extern UINT16 Tone_Hz;

// -------------------------------------------------------------------------
// Function Prototypes
// -------------------------------------------------------------------------
void Drumbot_Init(void);

void Drumbot_PB_Trap_OK(void);
void Drumbot_PB_Trap_Next(void);
void Drumbot_PB_Trap_Back(void);

void Drums_Process(void);
void Drum_Process(DRUM_TYPE* s);
void Drum_Setup(DRUM_TYPE* s, UINT8 chan, UINT8 hold_time);
void Drum_Hit(DRUM_TYPE* s);
void Light_Off(UINT8 index);
void Light_On(UINT8 index);

void Bot_Setup(BOT_TYPE* bot, char* song_str, UINT16 tempo, UINT16 offset);
void Bots_Process(void);
void Bot_Process(BOT_TYPE* bot);
SINT16 Bot_String_Advance(STRING_STRUCT* s);
void Bot_Song_Advance(BOT_TYPE* bot);

#endif
