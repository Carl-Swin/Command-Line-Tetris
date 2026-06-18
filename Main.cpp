#include "Tetris.h"

// GetAsyncKeyState return:
// MSB = Key currently down
// LSB = Key _has_been_down_ since last call to function


int main(int argc, char * argv[])
{
	tetris Tetris;
	int CheckProc = check_proc_call(argv);

	if (!CheckProc) Tetris.main_loop();
	
	return 0;
}


int check_proc_call(char * argv[])
{
	char * LpFilePart = NULL;
	char Buffer[PATH_LEN] = {'\0'};
	char ProcName[PATH_LEN] = {'\0'};

	GetFullPathNameA(argv[0], PATH_LEN, Buffer, &LpFilePart);
	strcpy(ProcName, argv[0]);
	*ProcName = (char) toupper((int) *ProcName);

	if (LpFilePart) {
		if ( !GetFullPathNameA(ProcName, PATH_LEN, Buffer, &LpFilePart) ) {
			GetFullPathNameA(argv[0], PATH_LEN, Buffer, &LpFilePart);
		}
	}
	else return 1;

	if (!strstr(LpFilePart, ".exe")) {
		LpFilePart = &(LpFilePart[strlen(LpFilePart)]);
		strcpy(LpFilePart, ".exe\0");
	}

	if (strcmp(argv[0], Buffer)) {
		printf("\nWARNING: Do not run the program: \"%s\" from a command prompt\n"
			"**Your key presses won't be cleared and will overflow into the command prompt on exit\n",
			argv[0]);
		return 1;
	}
	
	return 0;
}

int title_screen(char TitleScreen[][EDGE_X - 2])
{
	int C = 8;
	
	//	   [Y][X]
	TitleScreen[0][C+0] = DUOHORI;
	TitleScreen[0][C+1] = DUOTEED;
	TitleScreen[0][C+2] = DUOHORI;

	TitleScreen[0][C+4] = DUOTOPL;
	TitleScreen[0][C+5] = DUOHORI;
	TitleScreen[0][C+6] = DUOHORI;

	TitleScreen[0][C+8] = DUOHORI;
	TitleScreen[0][C+9] = DUOTEED;
	TitleScreen[0][C+10] = DUOHORI;

	TitleScreen[0][C+12] = DUOTOPL;
	TitleScreen[0][C+13] = DUOHORI;
	TitleScreen[0][C+14] = DUOTOPR;

	TitleScreen[0][C+16] = DUOHORI;
	TitleScreen[0][C+17] = DUOTEED;
	TitleScreen[0][C+18] = DUOHORI;

	TitleScreen[0][C+20] = DUOTOPL;
	TitleScreen[0][C+21] = DUOHORI;
	TitleScreen[0][C+22] = DUOHORI;


	TitleScreen[1][C+1] = DUOVERT;

	TitleScreen[1][C+4] = DUOTEER;
	TitleScreen[1][C+5] = DUOHORI;
	TitleScreen[1][C+6] = DUOHORI;

	TitleScreen[1][C+9] = DUOVERT;

	TitleScreen[1][C+12] = DUOTEER;
	TitleScreen[1][C+13] = DUOTEED;
	TitleScreen[1][C+14] = DUOBOTR;

	TitleScreen[1][C+17] = DUOVERT;

	TitleScreen[1][C+20] = DUOBOTL;
	TitleScreen[1][C+21] = DUOHORI;
	TitleScreen[1][C+22] = DUOTOPR;
	

	TitleScreen[2][C+1] = DUOTEEU;

	TitleScreen[2][C+4] = DUOBOTL;
	TitleScreen[2][C+5] = DUOHORI;
	TitleScreen[2][C+6] = DUOHORI;

	TitleScreen[2][C+9] = DUOTEEU;

	TitleScreen[2][C+12] = DUOTEEU;
	TitleScreen[2][C+13] = DUOBOTL;
	TitleScreen[2][C+14] = DUOHORI;

	TitleScreen[2][C+16] = DUOHORI;
	TitleScreen[2][C+17] = DUOTEEU;
	TitleScreen[2][C+18] = DUOHORI;

	TitleScreen[2][C+20] = DUOHORI;
	TitleScreen[2][C+21] = DUOHORI;
	TitleScreen[2][C+22] = DUOBOTR;

	return 0;
}
