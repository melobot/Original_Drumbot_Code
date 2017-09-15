void CTRL_Comm_Process(void) {
	UINT8 bytes_in_buffer;
	UINT8 outbyte, c;
	UINT8 cs_calc, cs_data;

	// Find out how many bytes are in the FIFO
	bytes_in_buffer = XUART_RXFIFO_Bytes(_CTRL_COMM_CHAN);
	if(bytes_in_buffer == 0) return;
	// Read a byte from the UART buffer
	XUART_RXFIFO_Read(_CTRL_COMM_CHAN, &c, 1);
	Ctrl.Comm_RX_Count++;
	#if _CTRL_COMM_VERBOSE_LEVEL >= 4
		printf("[%c]",Msg_Buf[0]);
	#endif

	switch(Ctrl.Comm_State) {
		case _CTRL_COMM_STATE_IDLE :
			// Examine the 1st character
			switch(c) {
				// Pause
				case ' ' :
					Drumbot_Pause();
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
				// Go faster
				case '+' :
					Drumbot_Faster();
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
				// Go Slower
				case '-' :
					Drumbot_Slower();
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
				// Activate channel n
				case '0' ... '9' :
					Drumbot_Note_Strike(c - '0');
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
				// Activate channel n
				case 'A' ... 'F' :
					Drumbot_Note_Strike(c - 'A');
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
				// These commands require a parameter to be passed
				case 'M' :	// Mute a channel
				case 'm' :	// Unmute a channel
				case 'p' :	// Pattern display
				case 'P' :	// Pattern store
				case 's' :  // Song display
				case 'S' :  // Song store
					Ctrl.CMD = c;
					Ctrl.Comm_State = _CTRL_COMM_STATE_P1;
					return;
			}
			return;

		// Wherein we seek the parameter
		case _CTRL_COMM_STATE_P1 :
			switch(c) {
				case '0' ... '9' :
					Ctrl.P1 = c - '0';
					break;
				case 'A' ... 'F' :
					Ctrl.P1 = c - 'A';
					break;
				default :
					Ctrl.Comm_State = _CTRL_COMM_STATE_IDLE;
					return;
			}

			return;

		case _CTRL_COMM_STATE_TERM :
	}
}
