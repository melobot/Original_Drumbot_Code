#include "Drumbot.h"

// ----------------------------------------
// Song Strings
// ----------------------------------------


// ----------------------------------------
// Melody Strings
// ----------------------------------------

// 0-Big can
// 1-Block Top
// 2-Big yogurt

char Mstr_A[] = "0--0--0--0--";	// 4-Pulse (12)
char Mstr_B[] = "1---1---1---";	// 3-Pulse (12)
char Mstr_C[] = "2-2-2-2-2-2-";	// Tresillio (12)
char Mstr_D[] = "6-4-66-6-4-6";	// 6-8 BELL (12)
char Mstr_E[] = "4---3--5----";	// Wierd one (12)
char Mstr_F[] = "3-23-23-23-2";	// Kee-La (12)
char Mstr_G[] = "C--CD-C-DC--";	// 4 against 3
char Mstr_H[] = "8---8---8---";	// Gong on the 3 pulse
char Mstr_I[] = "8-----8-----"; // Gong on the 4 pulse
char Mstr_J[] = "0--0--1-0-1-";	// I/B Mecru Bass line
char Mstr_K[] = "B-BB9---A-9-";	// I/B bembe low drum
char Mstr_L[] = "----9-----9";	// Experimental bass beat to phase with bembe
char Mstr_M[] = "1";
char Mstr_N[] = "1";
char Mstr_O[] = "1";
char Mstr_P[] = "1";
char Mstr_Q[] = "0000-0-0-";	// New idea for rhythm/melody string
char Mstr_R[] = "1";
char Mstr_S[] = "1";
char Mstr_T[] = "5-55-555---5-5---5-";
char Mstr_U[] = "5---25----5--222"; // (16)
char Mstr_V[] = "5-5-1-5-555-1-11"; // (16)
char Mstr_W[] = "334433443-3-3443";
char Mstr_X[] = "3";
char Mstr_Y[] = "0--1--2--3--4--5--6--7--8--9--A--B--";
char Mstr_Z[] = "------------"; // 12 beats of silence

// ----------------------------------------
// String Pointer Lists
// ----------------------------------------

char* Mstr_List[] = {
	&Mstr_A[0], &Mstr_B[0], &Mstr_C[0], &Mstr_D[0],
	&Mstr_E[0], &Mstr_F[0], &Mstr_G[0], &Mstr_H[0],
	&Mstr_I[0], &Mstr_J[0], &Mstr_K[0], &Mstr_L[0],
	&Mstr_M[0], &Mstr_N[0], &Mstr_O[0], &Mstr_P[0],
	&Mstr_Q[0], &Mstr_R[0], &Mstr_S[0], &Mstr_T[0],
	&Mstr_U[0], &Mstr_V[0], &Mstr_W[0], &Mstr_X[0],
	&Mstr_Y[0], &Mstr_Z[0]};
