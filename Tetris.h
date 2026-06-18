#pragma once

#define _WIN32_WINNT 0x0600 // states that this runs on windows vista or newer
#include <winsock2.h> // must be put before windows.h

#include <stdio.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>
#include <cctype>
#include <math.h>
#include <time.h>
#include <windows.h>

#include <iostream>


#define GRID_X 10
#define GRID_Y 23
#define NEXT_X 14
#define NEXT_Y 2

#define PATH_LEN 256 + 1

#define OUTLINE_X 18
#define OUTLINE_Y 4

#define EDGE_X 40
#define EDGE_Y 25


#define GAME_DELAY 750
#define INPUT_DELAY 100


#define TOPL 218
#define TOPR 191
#define BOTL 192
#define BOTR 217

#define TEEL 180
#define TEER 195
#define TEEU 193
#define TEED 194
#define CROS 197

#define VERT 179
#define HORI 196
	
#define DUOTOPL 201
#define DUOTOPR 187
#define DUOBOTL 200
#define DUOBOTR 188

#define DUOTEEL 185
#define DUOTEER 204
#define DUOTEEU 202
#define DUOTEED 203
#define DUOCROS 206

#define DUOVERT 186
#define DUOHORI 205

enum tetromino{Sq, Ln, Zb, Sb, Lb, Jb, Tb};

int check_proc_call(char * argv[]);
int title_screen(char TitleScreen[][EDGE_X - 2]);

class game
{
public:
	game();
	~game();

protected:
	int fill_piece(char Value);
	int fill_next_piece();
	int unfill_next_piece();

	int check_below(short CoordX, short CoordY);

	int check_complete_grid();


	char Grid[GRID_Y][GRID_X] = {0};
	char Next[NEXT_Y][NEXT_X] = {0};
	char ScoreStr[7] = {'0'};
	int Score;

	tetromino CurrentPiece;
	tetromino NextPiece;
	COORD Coord;
	int Rotation;

	int GameOver;
	int IsRunning;
	short MenuCurrent;

	void * MutexLock = NULL;

private:
	// Thread to manage game
	static DWORD WINAPI static_game_thread(LPVOID Param);
	DWORD game_thread();
	HANDLE GameThread;
	DWORD GameThreadID;
};

class input : public game
{
public: 
	input();
	~input();

protected:
	int input_menu();
	int input_game();

	int check_left(short CoordX, short CoordY);
	int check_right(short CoordX, short CoordY);
	int check_left_rotate();
	int check_right_rotate();


	short MenuSelect;
	int InputFlag;
	int DrawFlag;

private:
	// Thread to manage input
	static DWORD WINAPI static_input_thread(LPVOID Param);
	DWORD input_thread();
	HANDLE InputThread;
	DWORD InputThreadID;
};

class tetris : public input
{
public:
	tetris();
	~tetris();

	int main_loop();

protected:
	int draw_boarder();
	int draw_clear();
	int draw_menu();
	int draw_select();
	int draw_board();
	int draw_game();


	HWND hWnd;
	unsigned long Count;
};
