#include "Tetris.h"

// INPUT THREAD ///////////////////////////////////////////////////
input::input() : MenuSelect(3), 
			InputFlag(1),
			DrawFlag(1)
{
	InputThread = CreateThread(
			NULL, 0,
			static_input_thread,
			(void *) this, 0, 
			&InputThreadID);
}
input::~input()
{
	//printf("%d\n", CloseHandle(InputThread));
	CloseHandle(InputThread);
}


//3: Start Game, 2: Options, 1: Exit
int input::input_menu()
{
	int SelectFlag = 0;
	
	do {
		if (GetConsoleWindow() == GetForegroundWindow()) {
			if (GetAsyncKeyState(0x26) & 1) 
				(MenuSelect == 3) ? MenuSelect = 1 : ++MenuSelect;
			if (GetAsyncKeyState(0x28) & 1) 
				(MenuSelect == 1) ? MenuSelect = 3 : --MenuSelect;

			if (GetAsyncKeyState(0x20)) {
				MenuCurrent = MenuSelect;
				DrawFlag = 1;
				SelectFlag = 1;
			}
		}

	} while (!SelectFlag && IsRunning);
	
	return SelectFlag;
}

int input::input_game()
{
	unsigned long Tiden = 0;
	int LocalFlag = 1;

	int Under = -1;
	int Vanster = -1;
	int Hoger = -1;

	do {
		if (GetConsoleWindow() == GetForegroundWindow()) { 
			if (GetAsyncKeyState(0x1B)) IsRunning = 0;
			
			if ( !GameOver && InputFlag && ((clock() % INPUT_DELAY) < (INPUT_DELAY)/2) ) {
				// If you uncomment this, make sure to direct to output to a file (Tetris 2> JUNK.txt), if it prints to the console, it introduces more delay
				//fprintf(stderr, "%lu", Tiden = clock());
				//fprintf(stderr, "%lu, ", clock());

				usleep(INPUT_DELAY/2);
				//while (clock() % INPUT_DELAY != (INPUT_DELAY)/2) {} // Alternative input delay (is weirdly less stable)

				WaitForSingleObject(MutexLock, INFINITE);
				if ( GetAsyncKeyState(0x28) && !check_below(Coord.X, Coord.Y) ) {
					fill_piece(0);
					++Coord.Y;
					fill_piece(2);

					LocalFlag = 0;
				}
				if ( GetAsyncKeyState(0x25) && !check_left(Coord.X, Coord.Y) ) {
					fill_piece(0);
					--Coord.X;
					fill_piece(2);

					LocalFlag = 0;
				}
				if ( GetAsyncKeyState(0x27) && !check_right(Coord.X, Coord.Y) ) {
					fill_piece(0);
					++Coord.X;
					fill_piece(2);

					LocalFlag = 0;
				}
				if ( GetAsyncKeyState(0x26) && !(Hoger = check_right_rotate()) ) {
					if (CurrentPiece != Sq) {
						fill_piece(0);
						(Rotation >= 3) ? Rotation = 0 : ++Rotation;
						if (CurrentPiece == Ln) {
							if (Rotation % 2 && !((Coord.Y >= GRID_Y - 2 || Grid[Coord.Y + 2][Coord.X] == 1) && Rotation == 1)) 
								Coord.Y -= (Rotation - 2);
							else if (!(Coord.X <= 1 && Rotation) && !(Coord.X >= GRID_X - 2 && !Rotation))
								Coord.X -= (Rotation - 1);
						}
						fill_piece(2);
					}

					LocalFlag = 0;
				}
				//if ( (GetAsyncKeyState(0x20)) && !check_below(Coord.X, Coord.Y) ) // Space

				// If you uncomment this, make sure to direct to output to a file (Tetris 2> JUNK.txt), if it prints to the console, it introduces more delay
				//fprintf(stderr, ",%lu,%lu ", clock(), clock() - Tiden);

				ReleaseMutex(MutexLock);
				InputFlag = LocalFlag;
			}
			else if ( (clock() % INPUT_DELAY) >= (INPUT_DELAY)/2 ) InputFlag = 1;
		}

	} while (IsRunning);

/*
	int TheDown, TheLeft, TheRight;
	int InputDown, InputLeft, InputRight;
	TheDown = TheLeft = TheRight = 0;
	InputDown = InputLeft = InputRight = 0;

			if ( (InputDown = GetAsyncKeyState(0x28)) && !(TheDown = check_below(Coord.X, Coord.Y)) ) {
			if ( (InputLeft = GetAsyncKeyState(0x25)) && !(TheLeft = check_left(Coord.X, Coord.Y)) ) {
			if ( (InputRight = GetAsyncKeyState(0x27)) && !(TheRight = check_right(Coord.X, Coord.Y)) ) {

			printf("%x\t%x\t%x\t\t%x\t%x\t%x", 
					InputDown,
					InputLeft,
					InputRight,
					TheDown, 
					TheLeft,
					TheRight);
			for (int i = 0; i < 100; ++i) printf("\b");
//*/
	return 0;
}

int input::check_left(short CoordX, short CoordY)
{
	int ReturnVal = 0;

	if (Grid[CoordY][CoordX] < 2) return 0;
	if ((CoordX <= 0) || Grid[CoordY][CoordX - 1] == 1) ReturnVal = 1;

	if (!ReturnVal && Grid[CoordY][CoordX] > 1) {
		if (!ReturnVal && CoordX - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY][CoordX - 1]) 
			ReturnVal = check_left(CoordX - 1, CoordY);
		if (!ReturnVal && CoordX + 1 < GRID_X && Grid[CoordY][CoordX] > Grid[CoordY][CoordX + 1]) 
			ReturnVal = check_left(CoordX + 1, CoordY);
		if (!ReturnVal && CoordY - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY - 1][CoordX]) 
			ReturnVal = check_left(CoordX, CoordY - 1);
		if (!ReturnVal && CoordY + 1 < GRID_Y && Grid[CoordY][CoordX] > Grid[CoordY + 1][CoordX]) 
			if (Grid[CoordY + 1][CoordX] > 1) ReturnVal = check_left(CoordX, CoordY + 1);
	}

	//if (!ReturnVal && Grid[CoordY + 1][CoordX]) 
	//	ReturnVal = check_contact(CoordX, CoordY + 1);

	return ReturnVal;
}

int input::check_right(short CoordX, short CoordY)
{
	int ReturnVal = 0;

	if (Grid[CoordY][CoordX] < 2) return 0;
	if ((CoordX >= GRID_X - 1) || Grid[CoordY][CoordX + 1] == 1) ReturnVal = 1;

	if (!ReturnVal && Grid[CoordY][CoordX] > 1) {
		if (!ReturnVal && CoordX - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY][CoordX - 1]) 
			ReturnVal = check_right(CoordX - 1, CoordY);
		if (!ReturnVal && CoordX + 1 < GRID_X && Grid[CoordY][CoordX] > Grid[CoordY][CoordX + 1]) 
			ReturnVal = check_right(CoordX + 1, CoordY);
		if (!ReturnVal && CoordY - 1 >= 0 && Grid[CoordY][CoordX] > Grid[CoordY - 1][CoordX]) 
			ReturnVal = check_right(CoordX, CoordY - 1);
		if (!ReturnVal && CoordY + 1 < GRID_Y && Grid[CoordY][CoordX] > Grid[CoordY + 1][CoordX]) 
			if (Grid[CoordY + 1][CoordX] > 1) ReturnVal = check_right(CoordX, CoordY + 1);
	}

	//if (!ReturnVal && Grid[CoordY + 1][CoordX]) 
	//	ReturnVal = check_contact(CoordX, CoordY + 1);

	return ReturnVal;
}
int input::check_left_rotate() // NOT WORKING, NEED TO FIX
{
	short CoordX = Coord.X;
	short CoordY = Coord.Y - 1;
	short CornerX = Coord.X - 1;
	short CornerY = Coord.Y - 1;

	if (Coord.X >= GRID_X - 1 || CoordY >= GRID_Y - 1) return 1;

	for (int i = 0; i < 4; ++i) {
		if (Grid[CoordY][CoordX] > 1) {
			(i < 2) ? ++CoordY : --CoordY;
			(!i || i > 2) ? ++CoordX : --CoordX;

			if (Grid[CoordY][CoordX] == 1) return 1;
		}
		if (Grid[CornerY][CornerX] > 1) {
			(!(i & 0b10)) ? ++( (i % 2) ? CornerY : CornerX ) : --( (i % 2) ? CornerY : CornerX );

			if (Grid[CoordY][CoordX] == 1) return 1;
		}
	}

	return 0;
}
int input::check_right_rotate() // RESUME
{
	short CoordX = Coord.X;
	short CoordY = Coord.Y - 1;

	short CornerX = Coord.X - 1;
	short CornerY = Coord.Y - 1;
	short * Corner = NULL;

	if (Coord.X >= GRID_X - 1 || Coord.Y >= GRID_Y - 1 || Coord.X <= 0) return 1;

	for (int i = 0; i < 4; ++i) {
		if (Grid[CoordY][CoordX] > 1) {
			(i < 2) ? ++CoordY : --CoordY;
			(!i || i > 2) ? ++CoordX : --CoordX;

			if (Grid[CoordY][CoordX] == 1) return 1;
		}
		else {
			(i < 2) ? ++CoordY : --CoordY;
			(!i || i > 2) ? ++CoordX : --CoordX;
		}

		if (Grid[CornerY][CornerX] > 1) {
			Corner = (i % 2) ? &CornerY : &CornerX;
			(*Corner) += (!(i & 0b10)) ? 2 : -2;

			if (Grid[CornerY][CornerX] == 1) return 1;
		}
		else {
			Corner = (i % 2) ? &CornerY : &CornerX;
			(*Corner) += (!(i & 0b10)) ? 2 : -2;
		}
	}

	return 0;
}

DWORD WINAPI input::static_input_thread(LPVOID Param) 
{
	input * ThisHandler = (input *) Param;
	//printf("Running in static input thread: %d\n", ThisHandler->InputThreadID);
	return ThisHandler->input_thread();

}
DWORD input::input_thread()
{
	while (IsRunning) {
		switch (MenuCurrent) {
		case 0: 
			input_menu();
			break;
		case 1: 
			input_menu();
			break;
		case 2: 
			input_menu();
			break;
		case 3: 
			input_game();
			break;
		}
	}

	//printf("Exiting input thread: %d\n", InputThreadID);
	return 0;
}
