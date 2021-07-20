/*                                                                     Pong Clone                                                                                                  */
//*	The purpose of this code is to run pong in the command prompt without programming a graphics library or a screen buffer, this code will only work in windows operating systems *//

#include <iostream>																	        //Include basic input/output functions
#include "windows.h"																		//Windows specific functions
using namespace std;																		//Using the default name-space

//Game loop variables
bool bPaused = false;																		//Boolean value to check whether the game is paused or not
bool bGameRunning = true;																	//Boolean value to check whether the game loop should continue to run 
int gameSpeed = 200;																		//Variable to keep track of games speed, lower = faster
int gameSpeedInc = 10;																		//Variable to control the amount we change the game speed in increment

//Score keeping
int ply1Score = 0;																			//Player 1 score variable 
int ply2Score = 0;																			//Player 2 score variable
int winScore = 5;																			//Score required to win

//Enumerator for ball directions
enum eDirection1 { eUp, eDown }; enum eDirection2 { eRight, eLeft };						//Two enumerators to keep track of both x and y ball direction and limit to these options	
eDirection1 eDir1 = eDown;																	//Down or up starting direction
eDirection2 eDir2 = eRight;																	//Right or left starting direction		

//Player 1 symbol and coordinate
char ply1SYM = '|';																			//Symbol used for the movable character 
int ply1SYMx = 4;																			//Player symbol x position
int ply1SYM_0 = 4;																			//Player symbol y position first character
int ply1SYM_1 = ply1SYM_0 + 1;																//Player symbol y position second character, location one down from the first character

//Player 2 symbol and coordinate
char ply2SYM = '|';																			//Symbol used for the movable character 
int ply2SYMx = 34;																			//Player symbol x position
int ply2SYM_0 = 4;																			//Player symbol y position first character
int ply2SYM_1 = ply2SYM_0 + 1;																//Player symbol y position second character, location one down from the first character

//Ball symbol and coordinate
char ballSYM = '@';																			//Symbol used for our ball
int ballSYMx = 4;																			//Ball symbol x position
int ballSYMy = 1;																			//Ball symbol y position, needs to have a boarder above or below in order to get started moving

//Misc symbols
char emptySYM = ' ';																		//Symbol to keep track of the empty characters in our map																								
char scoreBoarder = ',';																	//Symbol to be used for hit detection and keeping score

//Map rows and columns
const int mRows = 10;																		//Number of rows in our map arrays
const int mCols = 40;																		//Number of columns in our map arrays

//Default game map
char map[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0										
	/*y*/",   @                                 ,",//1										
	/*+*/",                                     ,",//2
		 ",                                     ,",//3
		 ",   |                             |   ,",//4
		 ",   |                             |   ,",//5
		 ",                                     ,",//6
		 ",                                     ,",//7
		 ",                                     ,",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//Pause map
char pausedMap[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0
	/*y*/".              _     _                .",//1
	/*+*/".             | |   | |               .",//2
		 ".             | |   | |               .",//3
		 ".             | |   | |               .",//4
		 ".             | |   | |               .",//5
		 ".             | |   | |               .",//6
		 ".             |_|   |_|               .",//7
		 ".                                     .",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//Game over map
char gameOverMap[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0
	/*y*/".                                     .",//1
	/*+*/".                                     .",//2
		 ".                                     .",//3
		 ".            -GAME_OVER-              .",//4
		 ".                                     .",//5
		 ".                                     .",//6
		 ".                                     .",//7
		 ".                                     .",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//Start map
char StartMap[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0
	/*y*/".                                     .",//1
	/*+*/".                                     .",//2
		 ".                                     .",//3
		 ".       -PRESS_ENTER_TO_START-        .",//4
		 ".                                     .",//5
		 ".                                     .",//6
		 ".                                     .",//7
		 ".                                     .",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//player 1 wins, map
char ply1WinsMap[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0
	/*y*/".                                     .",//1
	/*+*/".                                     .",//2
		 ".                                     .",//3
		 ".           -PLAYER_1_WINS-           .",//4
		 ".                                     .",//5
		 ".                                     .",//6
		 ".                                     .",//7
		 ".                                     .",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//Player 2 wins, map
char ply2WinsMap[mRows][mCols] =
{
	/*-*/"#############-PONG_CLONE-##############",//0
	/*y*/".                                     .",//1
	/*+*/".                                     .",//2
		 ".                                     .",//3
		 ".           -PLAYER_2_WINS-           .",//4
		 ".                                     .",//5
		 ".                                     .",//6
		 ".                                     .",//7
		 ".                                     .",//8
		 "#############-PONG_CLONE-##############",//9
		//0123456789...  
			/* < - x + > */
};

//Function declaration/prototyping														
void update();
void** draw(char Map[mRows][mCols]);
void playerInput();
void logic();
void paused();
void gameOver();
void** copyMap(char newMap[mRows][mCols], char oldMap[mRows][mCols]);
void ballReset();
void init();
void** reset(char Map[mRows][mCols]);
void displayInfo();
void startGame();

//Main loop
int main()
{
	init();																					//Initialize variables, ball position, gamespeed, scores, draw Start menu, pause screen
	
	while (bGameRunning)                                                                    //While bGameRunning is set to true
	{
		update();																			//Clears the screen, displays info and draws the map, ball and paddles to the screen
		playerInput();																		//retrieve input from the player and change variables accordingly
		logic();																			//Decide when the ball should change direction and when winning states should be executed
		Sleep(gameSpeed);																	//Change the speed of the game, lower values = higher speed
	}

	return 0;																				//Exit and return the value of 0, code executed successfully
}

//Update the screen
void update()
{
	displayInfo();																			//Print out UI info after clearing screen
	draw(map);																				//Draw the game map to the screen
	cout << endl << endl;																	//Move text further down			
	cout << "Player 1: 'W' & 'S'" << endl;													//UI message
	cout << "Player 2: 'Up' & 'Down'" << endl;												//UI message
}

//Draw map to the screen, Prints out each row of the map array every run					// "sizeOf(map) / sizeOf(map[0])" gives the number of rows	
void** draw(char Map[mRows][mCols])															//Pointer to pointer function as char arrays are already seen as pointers
{
	for (int i = 0; i < mRows; i++)
	{
		cout << Map[i] << endl; 															//Print out each down of this map
	}
	return 0;
}

//Get player input
void playerInput()
{	//Player 1 controls
	if (GetAsyncKeyState('S'))
	{
		if (map[ply1SYM_1 + 1][ply1SYMx] == emptySYM)										//if there is an empty space under our last player character. and not a boarder!
		{
			map[ply1SYM_0][ply1SYMx] = emptySYM;											//replace the first player character with an empty space
			map[ply1SYM_1][ply1SYMx] = emptySYM;											//replace the second player character with an empty space
			ply1SYM_0++;																	//move first player character down one space 
			ply1SYM_1++;																	//move second player character down one space
			map[ply1SYM_0][ply1SYMx] = ply1SYM;												//draw the first part of our symbol in the new position 
			map[ply1SYM_1][ply1SYMx] = ply1SYM;												//draw the second part of our symbol in the new position 
		}
	}
	if (GetAsyncKeyState('W'))
	{
		if (map[ply1SYM_0 - 1][ply1SYMx] == emptySYM)										//if there is an empty space above our first player character. and not a boarder! 
		{
			map[ply1SYM_0][ply1SYMx] = emptySYM;											//replace the first player character with an empty space
			map[ply1SYM_1][ply1SYMx] = emptySYM;											//replace the second player character with an empty space
			ply1SYM_0--;																	//move first player character down one space     
			ply1SYM_1--;																	//move second player character down one space
			map[ply1SYM_0][ply1SYMx] = ply1SYM;												//draw the first part of our symbol in the new position 
			map[ply1SYM_1][ply1SYMx] = ply1SYM;												//draw the second part of our symbol in the new position 
		}
	}//Player 2 controls
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (map[ply2SYM_1 + 1][ply2SYMx] == emptySYM)										//if there is an empty space under our last player character. and not a boarder!
		{
			map[ply2SYM_0][ply2SYMx] = emptySYM;											//replace the first player character with an empty space
			map[ply2SYM_1][ply2SYMx] = emptySYM;											//replace the second player character with an empty space
			ply2SYM_0++;																	//move first player character down one space 
			ply2SYM_1++;																	//move second player character down one space
			map[ply2SYM_0][ply2SYMx] = ply1SYM;												//draw the first part of our symbol in the new position 
			map[ply2SYM_1][ply2SYMx] = ply1SYM;												//draw the second part of our symbol in the new position 
		}
	}
	if (GetAsyncKeyState(VK_UP))
	{
		if (map[ply2SYM_0 - 1][ply2SYMx] == emptySYM)										//if there is an empty space above our first player character. and not a boarder! 
		{
			map[ply2SYM_0][ply2SYMx] = emptySYM;											//replace the first player character with an empty space
			map[ply2SYM_1][ply2SYMx] = emptySYM;											//replace the second player character with an empty space
			ply2SYM_0--;																	//move first player character down one space     
			ply2SYM_1--;																	//move second player character down one space
			map[ply2SYM_0][ply2SYMx] = ply1SYM;												//draw the first part of our symbol in the new position 
			map[ply2SYM_1][ply2SYMx] = ply1SYM;												//draw the second part of our symbol in the new position 
		}
	}//UI Controls
	if (GetAsyncKeyState(VK_ESCAPE))														//if the player presses escape									
	{
		gameOver();																			//End the game
	}
	if (GetAsyncKeyState(VK_F1))															//If the player presses F1
	{
		if (bPaused != true)																//if bPaused = false
		{
			paused();																		//Pause the game
		}
		else if (bPaused == true)															//If bPaused = true
		{
			bPaused = false;																//Reset bool so that we can pause the game after continuing
		}
	}
	if (GetAsyncKeyState(VK_F2))															//If the player presses F2
	{
		if (gameSpeed > 10)																	//As long as gameSpeed is a positive number
		{
			gameSpeed -= gameSpeedInc;														//decrease gameSpeed which increases game speed 
		}
	}
}

//Logic function
void logic()
{	//Ball Logic
	switch (eDir1)
	{
	case eDown:
	{
		if (map[ballSYMy + 1][ballSYMx] == emptySYM)										 //Space below?
		{
			map[ballSYMy][ballSYMx] = emptySYM;											 //clear ball
			ballSYMy++;																	 //move the ball
			map[ballSYMy][ballSYMx] = ballSYM;											 //redraw the ball
		}
		else if (map[ballSYMy + 1][ballSYMx] != emptySYM)								 //No space below?
		{
			eDir1 = eUp;																 //Change direction
		}
		break;
	}
	case eUp:
	{
		if (map[ballSYMy - 1][ballSYMx] == emptySYM)									//Space above?
		{
			map[ballSYMy][ballSYMx] = emptySYM;											//clear ball
			ballSYMy--;																	//move the ball
			map[ballSYMy][ballSYMx] = ballSYM;											//redraw the ball
		}
		else if (map[ballSYMy - 1][ballSYMx] != emptySYM)								//No space above?
		{
			eDir1 = eDown;																//Change direction
		}
		break;
	}
	}
	switch (eDir2)
	{
	case eRight:
	{
		if (map[ballSYMy][ballSYMx + 1] == emptySYM)									//Space right?
		{
			map[ballSYMy][ballSYMx] = emptySYM;											//clear ball
			ballSYMx++;																	//move the ball
			map[ballSYMy][ballSYMx] = ballSYM;											//redraw the ball
		}
		else if (map[ballSYMy][ballSYMx + 1] != emptySYM)								//No space below?
		{
			if (map[ballSYMy][ballSYMx + 1] == scoreBoarder)								//If ball hits our right hand score boarder
			{
				ply1Score++;															//Player 1 gets a point
				ballReset();															//Ball position reset
			}
			eDir2 = eLeft;																//Change direction
		}
		break;
	}
	case eLeft:
	{
		if (map[ballSYMy][ballSYMx - 1] == emptySYM)									//Space left?
		{
			map[ballSYMy][ballSYMx] = emptySYM;											//clear ball
			ballSYMx--;																	//move the ball
			map[ballSYMy][ballSYMx] = ballSYM;											//redraw the ball
		}
		else if (map[ballSYMy][ballSYMx - 1] != emptySYM)								//No space below?
		{
			if (map[ballSYMy][ballSYMx - 1] == scoreBoarder)
			{
				ply2Score++;															//Player 2 gets a point
				ballReset();															//Ball position reset
			}
			eDir2 = eRight;																//Change direction
		}
		break;
	}
	}//Winning cases
	if (ply1Score == winScore)																//If Player 1s score reaches the set winScore variable
	{
		system("cls");																		//Clear the screen
		cout << endl << endl << endl << endl << endl;										//Move the to be map down
		reset(ply1WinsMap);																	//Draw player wins map and give the player the option to reset
	}

	if (ply2Score == winScore)																//If Player 2s score reaches the set winScore variable
	{
		system("cls");																		//Clear the screen
		cout << endl << endl << endl << endl << endl;										//Move the to be map down
		reset(ply2WinsMap);																	//Draw player wins map and give the player the option to reset
	}
}

//Pause function
void paused()
{
	displayInfo();																			//Print out UI info after clearing screen
	draw(pausedMap);																		//Draw the pause map to the screen
	system("pause > null");																	//Pause the screen and remove "press any key to continue" text
	bPaused = true;																			//Reset bPaused so that the game has to wait for another key press 
}

//GameOver Function
void gameOver()
{
	bGameRunning = false;																	//Stop running the game loop
	system("cls");																			//Clear the screen
	cout << endl << endl << endl;																//Move map further down
	cout << "PLayer 1 Score: " << ply1Score << "    ";										//Player 1 score
	cout << "PLayer 2 Score: " << ply2Score << "    " << endl << endl;						//Player 2 score
	reset(gameOverMap);																		//Display the game over map and give the player the option to reset the game
}

//Reset ball position
void ballReset()
{
	map[ballSYMy][ballSYMx] = ' ';															//clear ball
	ballSYMx = mCols / 2;																	//Reset ball position to the center
	ballSYMy = mRows / 2;																	//Reset ball position to the center	
	map[ballSYMy][ballSYMx] = ballSYM;														//Draw ball
}

//Initialization																	
void init()//Takes a bool value in to skip the extra return key press
{
	HWND consoleWindow = GetConsoleWindow();
	consoleWindow-;
	ballReset();																			//Reset ball position
	gameSpeed = 80;																			//Reset gameSpeed
	ply1Score = 0;																			//Reset player 1 score
	ply2Score = 0;																			//Reset player 2 score
	system("cls");																			//Clear the screen
	cout << endl << endl << endl << endl << endl;											//Move the map down		
	draw(StartMap);																			//Draw start map
	system("pause > null");																	//pause screen without message
}

//Takes in a map array and prints it to the screen, then gives the player the option to reset
void** reset(char Map[mRows][mCols])
{
	draw(Map);																				//Draw the passed in map to the screen
	cout << endl << endl;																	//Move text further down from the map
	cout << "Restart Y/N?";																	//Message to player
	system("pause > null");																	//Pause the screen without default messages
	if (GetAsyncKeyState('Y'))																//If the 'Y' key is pressed
	{
		bGameRunning = true;																//Run game loop
		init();																				//Run initialization
	}
	else if (GetAsyncKeyState(VK_ESCAPE))													//If anything other than 'Y' is pressed
	{
		bGameRunning = false;																//End the game loop
	}
	return 0;																				//Function must return a value
}

//Function called when a new game is started, asks the player to start the game or exit and close
void startGame()
{
	if (GetAsyncKeyState(VK_RETURN))														//If the return key is pressed
	{

		bGameRunning = true;																//Run game loop
		init();																				//Run initialization
	}
	else if (!GetAsyncKeyState(VK_RETURN))													//If any key other than the return key is pressed
	{
		bGameRunning = false;																//Exit game loop
		exit;																				//Exit function
	}
}

//Display recurring information to the screen
void displayInfo()
{
	system("cls");		                                                                    //Clear the screen
	cout << ("ESC: Exit") << endl;															//Print messages
	cout << ("F1: Pause ") << endl;															//Print messages
	cout << ("F2: Increase game speed") << endl;											//Print messages
	cout << "PLayer 1 Score: " << ply1Score << "    ";										//Player 1 score
	cout << "PLayer 2 Score: " << ply2Score << "    ";										//Player 2 score
	cout << endl << "First player to get: " << winScore << " points wins";					//Win score
	cout << endl << endl;																	//Move map further down
}

//*Not implemented* (Copy passed in old map array into new one)
/*void** copyMap(char newMap[mRows][mCols], char oldMap[mRows][mCols])						//Pointer to pointer function that takes in the old map and new map as variables
{
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			newMap[i][j] = oldMap[i][j];													//New map characters are equal to the old maps characters
		}
	}
	return 0;
}*/