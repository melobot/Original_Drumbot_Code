#include "Drumbot.h"
#include "Pattern_Strings.h"
#include "Data_Conversions.h"
#include <stdio.h>

// ----------------------------------------
// Variable Declarations
// ----------------------------------------
BOT_TYPE Bot[_BOT_COUNT];

// ----------------------------------------
// Initialization
// ----------------------------------------
void Bot_Setup(BOT_TYPE* bot, char* song_str, UINT16 tempo, UINT16 offset) {
	// Load the time and tempo
	bot->Tick_Time = tempo;
	bot->Tick_Timer = offset;
	// Set the song string pointer and set character pointer at first element
	bot->Song_Str.Char_Ptr = bot->Song_Str.Str_Ptr = song_str;
	// Debug
	printf("Bot Song Set %u %u %s\n\r",tempo,offset,song_str);
	// Load new data and advance the pointer
	Bot_Song_Advance(bot);
	// Enable the bot to play
	bot->Enabled = 1;
}

// ----------------------------------------
// Periodic process
// ----------------------------------------
void Bots_Process(void) {
	UINT8 x;
	for (x = 0; x < _BOT_COUNT; x++) {
		Bot_Process(&Bot[x]);
	}
}

void Bot_Process(BOT_TYPE* bot) {
	UINT16 chan, delay;

	if (bot->Enabled == 0) return;

	if (bot->Tick_Timer > 0) {
		if (--(bot->Tick_Timer) > 0) return;
	}

	// Load delay until next note
	bot->Tick_Timer = bot->Tick_Time;

	// Decode values as ASCII hex
	if (*(bot->Melody_Str.Char_Ptr) == '-') {
		chan = 0xFFFF;
	} else {
		chan = Hexchar_To_Int(*(bot->Melody_Str.Char_Ptr));
	}

	// If the channel is valid, then request activation
	if (chan < _CHAN_COUNT) {
		// Activate the appropriate note
		bot->Last_Note = chan;
		Drum_Hit(&Drum[chan]);
		printf("%u-%u!\n\r",chan,bot->Tick_Time);
	}

	// Increment to next position in melody string
	if (Bot_String_Advance(&(bot->Melody_Str)) != 0) {
		Bot_Song_Advance(bot);
		return;
	}
}

// ----------------------------------------
// The bots use this to flow through the character strings
//    that spell out what notes to play when and where to go next
// ----------------------------------------
SINT16 Bot_String_Advance(STRING_STRUCT* s) {
	// Increment to next slot
	s->Char_Ptr++;
	// Skip whitespace
	while(*(s->Char_Ptr) == ' ') {
		s->Char_Ptr++;
	}
	// What to do if end of string is reached?
	if (*(s->Char_Ptr) == 0) {
		// Start over at the beginning
		s->Char_Ptr = s->Str_Ptr;
		// If loop counter equals one, then return 1 to indicate it's time to advance
		// A value of zero disables loop counter on this string
		if (s->Loop_Counter == 1) return(1);
		// Otherwise decrement the counter if not zero
		if (s->Loop_Counter > 0) {
			s->Loop_Counter--;
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// "String Song" system
// ---------------------------------------------------------------------------
// Song strings contain groups of 1-character words.  "M"
//		M : melody pattern index A-Z

void Bot_Song_Advance(BOT_TYPE* bot) {
	UINT16 m_num;

	// Skip any spaces between segment codes
	while(*(bot->Song_Str.Char_Ptr) == ' ') {
		bot->Song_Str.Char_Ptr++;
	}

	// Loop when end of string reached
	if (*(bot->Song_Str.Char_Ptr) == 0) {
		// Reload pointer to start of string
		bot->Song_Str.Char_Ptr = bot->Song_Str.Str_Ptr;
		// Skip any spaces before segment codes
		while(*(bot->Song_Str.Char_Ptr) == ' ') {
			bot->Song_Str.Char_Ptr++;
		}		
	}

	// Load the next values from the string
	m_num = *(bot->Song_Str.Char_Ptr++) - 'A';

	// Apply sanity checks
	if (m_num > 25) m_num = 0;

	// Load melody data into structure
	bot->Melody_Str.Str_Ptr = bot->Melody_Str.Char_Ptr = Mstr_List[m_num];
	bot->Melody_Str.Loop_Counter = 1;

	// Debug
	printf("Pattern M%c\n\r", 'A'+m_num);
}
