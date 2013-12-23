#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

// Obvious main is obvious
int main(void)
{
	// Initial variables
	int running = 1;
	char *paddle = "@@@@@@@@@@@";
	int padX, padY;
	int ballX, ballY;
	int ballDirX, ballDirY;
	char ball = 'O';

	// Seeding the psuedo-random number generator
	srand(time(NULL));

	// Starts ncurses
	initscr();

	// Starting coordinates for player paddle
	padX = (COLS - strlen(paddle)) / 2;
	padY = LINES - 1;

	// Starting coordinates for the ball
	ballX = COLS / 2;
	ballY = LINES / 2;

	// Initial direction of the ball randomized
	ballDirX = rand() % 2;
	ballDirY = rand() % 2;

	// Ncurses initialization and display options
	keypad(stdscr, TRUE);
	curs_set(0);
	halfdelay(1);

	// Lots of windows to make the table. Not a great way to do it,
	// but it works.
	WINDOW *testWin;
	WINDOW *testSub1;
	WINDOW *testSub2;
	WINDOW *testSub3;
	WINDOW *testSub4;

	// Locations of the windows
	testWin = newwin(0,0,0,0);
	testSub1 = derwin(testWin, LINES / 2 + 1, COLS / 2 + 1, 0, 0);
	testSub2 = derwin(testWin, LINES / 2 + 1, COLS / 2 + 1, LINES / 2, 0);
	testSub3 = derwin(testWin, LINES / 2 + 1, COLS / 2 + 1, 0, COLS / 2);
	testSub4 = derwin(testWin, LINES / 2 + 1, COLS / 2 + 1, LINES / 2, COLS / 2);

	// Main game loop
	while(running)
	{
		// Erases the screen to reset graphics
		erase();

		// Sets the borders of the windows to make the table fancy
		wborder(testSub1, '|', '*', '-', '#', '+', '+', '+', '#');
		wborder(testSub2, '|', '*', '#', '-', '+', '#', '+', '+');
		wborder(testSub3, '*', '|', '-', '#', '+', '+', '#', '+');
		wborder(testSub4, '*', '|', '#', '-', '#', '+', '+', '+');

		// This pushes the tables from memory to working space
		refresh();
		wrefresh(testWin);
		wrefresh(testSub1);
		wrefresh(testSub2);
		wrefresh(testSub3);
		wrefresh(testSub4);

		// Increment the ball position according to the direction
		if(ballDirX)
			ballX++;
		else
			ballX--;

		if(ballDirY)
			ballY++;
		else
			ballY--;

		// Change the direction of the ball if collides with the sides
		if(ballX <= 1 || ballX >= COLS - 2)
		{
			ballDirX ^= 1;
		}

		if(ballY <= 1 || (ballY >= LINES - 2 && ballX > padX && ballX < padX + strlen(paddle)))
		{
			ballDirY ^= 1;
		}

		// Check to see if the ball has passed the player's paddle
		if(ballY >= LINES)
		{
			running = 0;
		}

		// Printing out of the paddle and the ball
		mvprintw(padY, padX, "%s", paddle);
		mvprintw(ballY, ballX, "%c", ball);

		// Switch case for user input
		switch(getch())
		{
			// Option to end the program
			case KEY_F(1):
				running = 0;
				break;

			// The next two are for movement options for player paddle
			case KEY_LEFT:
				if(padX > 0)
					padX -= 2;

				break;

			case KEY_RIGHT:
				if(padX < COLS - strlen(paddle))
					padX += 2;

				break;
		}
	}

	// Program end, cleanup for ncurses, and window memory cleanup
	delwin(testSub1);
	delwin(testSub2);
	delwin(testSub3);
	delwin(testSub4);
	delwin(testWin);

	endwin();
	return 0;
}
