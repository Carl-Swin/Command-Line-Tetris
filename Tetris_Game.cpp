#include "Tetris.h"

// GAME THREAD ////////////////////////////////////////////////////
game::game() : Score(0),
		Rotation(0),
		MenuCurrent(0), 
		IsRunning(1),
		GameOver(0)
{
	MutexLock = CreateMutex(NULL, false, NULL);

	GameThread = CreateThread(
			NULL, 0,
			static_game_thread,
			(void *) this, 0, 
			&GameThreadID);
}
game::~game()
{
	//printf("%d\n", CloseHandle(InputThread));
	CloseHandle(GameThread);
	CloseHandle(MutexLock);
}


//enum tetromino{Sq, Ln, Zb, Sb, Lb, Jb, Tb};
//char Value represents whether the fill is for a piece being controlled or a piece being set
//2+: piece being controlled, 1: piece being set, 0: blank space
int game::fill_piece(char Value)
{
	char ValueMax = (Value < 2) ? Value : Value + 2;
	char ValueMid = (Value < 2) ? Value : Value + 1;
	char ValueMin = Value;

	WaitForSingleObject(MutexLock, INFINITE);
	{
		Grid[Coord.Y][Coord.X] = ValueMax;

		switch (CurrentPiece) {
		case Sq:
			Grid[Coord.Y][Coord.X - 1] = ValueMid;
			Grid[Coord.Y + 1][Coord.X] = ValueMid;
			Grid[Coord.Y + 1][Coord.X - 1] = ValueMin;
			break;
		case Ln:
			if (Rotation == 0) {
				Grid[Coord.Y][Coord.X - 2] = ValueMin;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y - 2][Coord.X] = ValueMin;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y][Coord.X + 2] = ValueMin;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y + 2][Coord.X] = ValueMin;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
			}
			break;
		case Zb:
			if (Rotation == 0) {
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X + 1] = ValueMin;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X - 1] = ValueMin;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y - 1][Coord.X - 1] = ValueMin;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y - 1][Coord.X + 1] = ValueMin;
			}
			break;
		case Sb:
			if (Rotation == 0) {
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X - 1] = ValueMin;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y - 1][Coord.X - 1] = ValueMin;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y - 1][Coord.X + 1] = ValueMin;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X + 1] = ValueMin;
			}
			break;
		case Lb:
			if (Rotation == 0) {
				Grid[Coord.Y - 1][Coord.X + 1] = ValueMin;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y + 1][Coord.X + 1] = ValueMin;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y + 1][Coord.X - 1] = ValueMin;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y - 1][Coord.X - 1] = ValueMin;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
			}
			break;
		case Jb:
			if (Rotation == 0) {
				Grid[Coord.Y - 1][Coord.X - 1] = ValueMin;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y - 1][Coord.X + 1] = ValueMin;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y + 1][Coord.X + 1] = ValueMin;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y + 1][Coord.X - 1] = ValueMin;
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
			}
			break;
		case Tb:
			if (Rotation == 0) {
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 1) {
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 2) {
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
				Grid[Coord.Y][Coord.X + 1] = ValueMid;
			}
			else if (Rotation == 3) {
				Grid[Coord.Y - 1][Coord.X] = ValueMid;
				Grid[Coord.Y + 1][Coord.X] = ValueMid;
				Grid[Coord.Y][Coord.X - 1] = ValueMid;
			}
			break;
		}
	}
	ReleaseMutex(MutexLock);

	return 0;
}
int game::fill_next_piece()
{
	switch (NextPiece) {
	case Sq:
		for (short i = 5; i < 9; ++i) {
			Next[0][i] = 1; 
			Next[1][i] = 1; 
		}
		break;
	case Ln:
		for (short i = 3; i < 11; ++i) Next[1][i] = 1; 
		break;
	case Zb:
		for (short i = 4; i < 8; ++i) {
			Next[0][i] = 1; 
			Next[1][i+2] = 1; 
		}
		break;
	case Sb:
		for (short i = 6; i < 10; ++i) {
			Next[0][i] = 1; 
			Next[1][i-2] = 1; 
		}
		break;
	case Lb:
		Next[0][8] = 1; 
		Next[0][9] = 1; 
		for (short i = 4; i < 10; ++i) Next[1][i] = 1; 
		break;
	case Jb:
		Next[0][4] = 1; 
		Next[0][5] = 1; 
		for (short i = 4; i < 10; ++i) Next[1][i] = 1; 
		break;
	case Tb:
		Next[0][6] = 1; 
		Next[0][7] = 1; 
		for (short i = 4; i < 10; ++i) Next[1][i] = 1; 
		break;
	}

	return 0;
}
int game::unfill_next_piece()
{
	for (short i = 0; i < NEXT_X; ++i) {
		Next[0][i] = Next[1][i] = 0; 
	}

	return 0;
}

int game::check_below(short CoordX, short CoordY)
{
	int ReturnVal = 0;

	if (Grid[CoordY][CoordX] < 2) return 0;
	if ((CoordY + 1 >= EDGE_Y - 2) || Grid[CoordY + 1][CoordX] == 1) ReturnVal = 1;
	
	if (!ReturnVal && Grid[CoordY][CoordX] > 1) {
		if (!ReturnVal && CoordX - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY][CoordX - 1]) 
			ReturnVal = check_below(CoordX - 1, CoordY);
		if (!ReturnVal && CoordX + 1 < GRID_X && Grid[CoordY][CoordX] > Grid[CoordY][CoordX + 1]) 
			ReturnVal = check_below(CoordX + 1, CoordY);
		if (!ReturnVal && CoordY - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY - 1][CoordX]) 
			ReturnVal = check_below(CoordX, CoordY - 1);
		if (!ReturnVal && CoordY + 1 < GRID_Y && Grid[CoordY][CoordX] > Grid[CoordY + 1][CoordX]) 
			if (Grid[CoordY + 1][CoordX] > 1) ReturnVal = check_below(CoordX, CoordY + 1);
	}

	//if (!ReturnVal && Grid[CoordY + 1][CoordX]) 
	//	ReturnVal = check_below(CoordX, CoordY + 1);

	return ReturnVal;
}

int game::check_complete_grid()
{
	int i = 0;

	for (int j = GRID_Y - 1; j >= 0; --j) {
		for (i = 0; i < GRID_X && Grid[j][i]; ++i) {}

		if (i == GRID_X) {
			for (int y = j - 1; y >= 0; --y) {
				for (int x = 0; x < GRID_X; ++x) Grid[y + 1][x] = Grid[y][x];
			}
			++j;
			Score += 82;
		}
	}

	return 0;
}

DWORD WINAPI game::static_game_thread(LPVOID Param) 
{
	game * ThisHandler = (game *) Param;
	//printf("Running in static game thread: %d\n", ThisHandler->InputThreadID);
	return ThisHandler->game_thread();

} // enum tetromino{Sq, Ln, Zb, Sb, Lb, Jb, Tb};
DWORD game::game_thread()
{
	time_t Time;
	int TimeFlag = 1;

	srand((unsigned) time(&Time));
	CurrentPiece = (tetromino)(rand() % 7);
	NextPiece = (tetromino)(rand() % 7);

	Coord.X = 5;
	Coord.Y = 1;

	fill_piece(2);
	fill_next_piece();

	while (IsRunning) {
		if ( (MenuCurrent == 3) && (clock() % GAME_DELAY) < GAME_DELAY/2 && TimeFlag && !GameOver ) {
			if (check_below(Coord.X, Coord.Y)) {
				if (Coord.Y == 1 && Coord.X == 5) {
					printf("GAME OVER");
					GameOver = 1;
				}
				else {
					WaitForSingleObject(MutexLock, INFINITE);
					{
						fill_piece(1);
						check_complete_grid();
						
						CurrentPiece = NextPiece;
						NextPiece = (tetromino)(rand() % 7);

						Coord.X = 5;
						Coord.Y = 1;
						Rotation = 0;

						unfill_next_piece();
						fill_piece(2);
						fill_next_piece();
					}
					ReleaseMutex(MutexLock);
				}
			}
			//else if (!GetAsyncKeyState(0x28) || Coord.Y == EDGE_Y - 2) {
			else if (!GetAsyncKeyState(0x28)) {
			//else {
				WaitForSingleObject(MutexLock, INFINITE);
				{
					fill_piece(0);
					unfill_next_piece();

					++Coord.Y;

					fill_piece(2);
					fill_next_piece();
				}
				ReleaseMutex(MutexLock);
			}

			TimeFlag = 0;
		}
		if ((clock() % GAME_DELAY) >= GAME_DELAY/2) TimeFlag = 1;
	}

	//printf("Exiting game thread: %d\n", InputThreadID);
	return 0;
}
