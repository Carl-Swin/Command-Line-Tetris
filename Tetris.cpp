#include "Tetris.h"


tetris::tetris() : Count(0) 
{
	hWnd = (HWND) GetStdHandle(STD_OUTPUT_HANDLE);
}
tetris::~tetris()
{
	//printf("%d\n", CloseHandle(InputThread));
}

int tetris::main_loop()
{
	draw_boarder();
	//draw_board();
	//draw_clear();
	
	while (IsRunning) {
		switch (MenuCurrent) {
		case 0: // Title Screen
			if (DrawFlag) {
				draw_clear();
				draw_menu();
				DrawFlag = 0;
			}
			draw_select();
			break;
		case 1: // Exit
			IsRunning = 0;
			break;
		case 2: // Controls
			IsRunning = 0;
			break;
		case 3: // Game
			if (DrawFlag) {
				draw_clear();
				draw_board();
				DrawFlag = 0;
			}
			draw_game();
			//IsRunning = 0;
			break;
		default:
			IsRunning = 0;
			break;
		}
/*
		if (GetAsyncKeyState(0x43) || GetAsyncKeyState(0x46)) // exit with C or F key
			IsRunning = 0;
*/
	}
	
	return 0;
}

int tetris::draw_boarder()
{
	char TopL = TOPL;
	char TopR = TOPR;
	char BotL = BOTL;
	char BotR = BOTR;

	char Vert = VERT;
	char Hori = HORI;

	COORD DrawCoord;

	DrawCoord.X = 0;
	DrawCoord.Y = 0;

	WriteConsoleOutputCharacter(hWnd, &TopL, 1, {0, 0}, &Count);
	WriteConsoleOutputCharacter(hWnd, &TopR, 1, {EDGE_X, 0}, &Count);
	WriteConsoleOutputCharacter(hWnd, &BotL, 1, {0, EDGE_Y}, &Count);
	WriteConsoleOutputCharacter(hWnd, &BotR, 1, {EDGE_X, EDGE_Y}, &Count);

	for (short i = 1; i < EDGE_Y; ++i) {
		DrawCoord.X = 0;
		DrawCoord.Y = i;
		WriteConsoleOutputCharacter(hWnd, &Vert, 1, DrawCoord, &Count);

		DrawCoord.X = EDGE_X;
		WriteConsoleOutputCharacter(hWnd, &Vert, 1, DrawCoord, &Count);
	}
	for (short i = 1; i < EDGE_X; ++i) {
		DrawCoord.X = i;
		DrawCoord.Y = 0;
		WriteConsoleOutputCharacter(hWnd, &Hori, 1, DrawCoord, &Count);

		DrawCoord.Y = EDGE_Y;
		WriteConsoleOutputCharacter(hWnd, &Hori, 1, DrawCoord, &Count);
	}

	WriteConsoleOutputCharacter(hWnd, "/\\", 2, {EDGE_X/4 + 9, EDGE_Y + 1}, &Count);
	WriteConsoleOutputCharacter(hWnd, "Ctrl + Scroll to zoom", 21, {EDGE_X/4, EDGE_Y + 2}, &Count);
	WriteConsoleOutputCharacter(hWnd, "\\/", 2, {EDGE_X/4 + 9, EDGE_Y + 3}, &Count);

	return 0;
}

int tetris::draw_clear()
{
	char Clear[] = "                                       ";

	for (short i = 1; i < EDGE_Y; ++i) {
		WriteConsoleOutputCharacter(hWnd, Clear, EDGE_X - 1, {1, i}, &Count);
	}
	return 0;
}
	
int tetris::draw_menu()
{
	char TitleScreen[EDGE_Y - 2][EDGE_X - 2] = { ' ' };
	
	title_screen(TitleScreen);

	for (int i = 0; i < 3; ++i)
		WriteConsoleOutputCharacter(hWnd, &(TitleScreen[i][0]), EDGE_X - 2, {1, (short)(i+1)}, &Count);

	WriteConsoleOutputCharacter(hWnd, "Start Game", 10, {EDGE_X/3 + 3, EDGE_Y - 7}, &Count);
	WriteConsoleOutputCharacter(hWnd, "Controls", 8, {EDGE_X/3 + 3, EDGE_Y - 5}, &Count);
	WriteConsoleOutputCharacter(hWnd, "Exit", 4, {EDGE_X/3 + 3, EDGE_Y - 3}, &Count);

	return 0;
}

int tetris::draw_select()
{
	char Select = ' ';

	Select = (MenuSelect == 3) ? '>' : ' ';
	WriteConsoleOutputCharacter(hWnd, &Select, 1, {EDGE_X/3, EDGE_Y - 7}, &Count);

	Select = (MenuSelect == 2) ? '>' : ' ';
	WriteConsoleOutputCharacter(hWnd, &Select, 1, {EDGE_X/3, EDGE_Y - 5}, &Count);

	Select = (MenuSelect == 1) ? '>' : ' ';
	WriteConsoleOutputCharacter(hWnd, &Select, 1, {EDGE_X/3, EDGE_Y - 3}, &Count);

	return 0;
}

int tetris::draw_board()
{
	char TopL = TOPL;
	char TopR = TOPR;

	char TeeR = TEER;
	char TeeU = TEEU;

	char Vert = VERT;
	char Hori = HORI;

	char DuoBotL = DUOBOTL;
	char DuoBotR = DUOBOTR;

	char DuoTeeU = DUOTEEU;

	char DuoVert = DUOVERT;
	char DuoHori = DUOHORI;

	//char TestPiece[2] = {(char)219, (char)219};

	char DuoToMonoU = 207;
	char DuoToMonoL = 182;

	for (short i = OUTLINE_Y; i < EDGE_Y - 1; ++i) {
		WriteConsoleOutputCharacter(hWnd, &DuoVert, 1, {OUTLINE_X, i}, &Count);
		WriteConsoleOutputCharacter(hWnd, &DuoVert, 1, {EDGE_X - 1, i}, &Count);
	}
	for (short i = 2; i < EDGE_X - 1; ++i) {
		WriteConsoleOutputCharacter(hWnd, &DuoHori, 1, {i, EDGE_Y - 1}, &Count);
	}


	for (short i = EDGE_Y - 6; i < EDGE_Y - 1; ++i) { 
		WriteConsoleOutputCharacter(hWnd, &Vert, 1, {1, i}, &Count);
		if (i < EDGE_Y - 3) WriteConsoleOutputCharacter(hWnd, &Vert, 1, {11, i}, &Count);
		if (i > EDGE_Y - 4) WriteConsoleOutputCharacter(hWnd, &Vert, 1, {OUTLINE_X - 2, i}, &Count);
	}
	for (short i = 2; i < OUTLINE_X - 2; ++i) {
		WriteConsoleOutputCharacter(hWnd, &Hori, 1, {i, EDGE_Y - 4}, &Count);
		if (i < 11) WriteConsoleOutputCharacter(hWnd, &Hori, 1, {i, EDGE_Y - 7}, &Count);
	}


	WriteConsoleOutputCharacter(hWnd, &DuoTeeU, 1, {OUTLINE_X, EDGE_Y - 1}, &Count);
	WriteConsoleOutputCharacter(hWnd, &DuoBotR, 1, {EDGE_X - 1, EDGE_Y - 1}, &Count);


	WriteConsoleOutputCharacter(hWnd, &DuoToMonoU, 1, {1, EDGE_Y - 1}, &Count);
	WriteConsoleOutputCharacter(hWnd, &DuoToMonoU, 1, {OUTLINE_X - 2, EDGE_Y - 1}, &Count);


	WriteConsoleOutputCharacter(hWnd, &TeeR, 1, {1, EDGE_Y - 4}, &Count);
	WriteConsoleOutputCharacter(hWnd, &TeeU, 1, {11, EDGE_Y - 4}, &Count);
	WriteConsoleOutputCharacter(hWnd, &TopR, 1, {OUTLINE_X - 2, EDGE_Y - 4}, &Count);

	WriteConsoleOutputCharacter(hWnd, &TopL, 1, {1, EDGE_Y - 7}, &Count);
	WriteConsoleOutputCharacter(hWnd, &TopR, 1, {11, EDGE_Y - 7}, &Count);
	WriteConsoleOutputCharacter(hWnd, "Score:", 6, {2, EDGE_Y - 6}, &Count);
	WriteConsoleOutputCharacter(hWnd, "0000000", 7, {3, EDGE_Y - 5}, &Count);
		
	// Test tiles, ***MAKE THE TILES OF EACH PIECE TWO CHARACTERS LIKE BELOW***
	//WriteConsoleOutputCharacter(hWnd, TestPiece, 2, {3, 3}, &Count);
	
	return 0;
}

int tetris::draw_game()
{
	char CenterBlock[] = {(char) 178, (char) 178};
	char Block[] = {(char) 219, (char) 219};

	sprintf(ScoreStr, "%07d\0", Score);
	WriteConsoleOutputCharacter(hWnd, ScoreStr, 7, {3, EDGE_Y - 5}, &Count);

	for (short j = 0; j < GRID_Y; ++j) {
		for (short i = 0; i < GRID_X; ++i) {
			if (Grid[j][i]) {
				WriteConsoleOutputCharacter(
						hWnd, 
						(Grid[j][i] == 4) ? CenterBlock : Block, 2, 
						{(short)(OUTLINE_X+(i*2)+1), (short)(1+j)}, 
						&Count);
			}
			else {
				WriteConsoleOutputCharacter(
						hWnd, 
						"  ", 2, 
						{(short)(OUTLINE_X+(i*2)+1), (short)(1+j)}, 
						&Count);
			}
		}
	}

	for (short j = 0; j < NEXT_Y; ++j) {
		for (short i = 0; i < NEXT_X; ++i) {
			if (Next[j][i]) {
				WriteConsoleOutputCharacter(
						hWnd, 
						Block, 1, 
						{(short)(2+i), (short)(EDGE_Y+j-3)}, 
						&Count);
			}
			else {
				WriteConsoleOutputCharacter(
						hWnd, 
						" ", 1, 
						{(short)(2+i), (short)(EDGE_Y+j-3)}, 
						&Count);
			}
		}
	}

	return 0;
}
