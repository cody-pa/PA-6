/*
* Programmer: Cody Paschich
* Class: CptS 121, Lab Section 17L
* Programming Assignment: PA 6
* Date: 11/6/2019
* Description: Game of battleship
	(note: the battleship logs are in the games folder in the project directory.)
*/

#include <stdio.h>
#include "battleship.h"
#include "../kp-lib/kp-lib.h"
#include <time.h>
#include <windows.h>

//#define TEST

int main (void)
{
	srand((unsigned int)time(0));
#ifdef TEST
	
	CLEAR_SCREEN;
	playerdata_t playerdat;
	for (int i = 0; i < 10; ++i)
	{
		for (int k = 0; k < 10; ++k)
		{
			playerdat.gameboard[i][k] = empty;
		}
	}
	set_board_automatically(playerdat.gameboard);
	
	while (playerdat.surviving_ships > 0)
	{
		Sleep(100);
		CLEAR_SCREEN;
		ai_take_turn(NULL, &playerdat);
		printf("%s\n", get_debug_output());
		print_board(playerdat.gameboard, true);
	}

#else
	bool playing = true;
	const char * message = "";
	battleship_log(NORMAL_PRINT, "=========================================================\n\
== BATTLESHIP                                          ==\n\
=========================================================");
	do
	{
		battleship_log(NORMAL_PRINT, "\n======== NEW GAME ============================");
		/////////////////////////////
		/// Initialize Game
		/////////////////////////////
		playerdata_t players[2];
		players[human].surviving_ships = 17;
		players[ai].surviving_ships = 17;

		for (int i = 0; i < 10; ++i)
		{
			for (int k = 0; k < 10; ++k)
			{
				players[human].gameboard[i][k] = empty;
				players[ai].gameboard[i][k] = empty;
			}
		}

		if (yn_prompt("Set your board manually?"))
		{
			set_board_manually(&players[human]);
		}
		else
		{
			set_board_automatically(players[human].gameboard);
		}
		set_board_automatically(players[ai].gameboard);

		player_t current = choose_starting_player();
		player_t opposition;
		bool player_lost = false;
		CLEAR_SCREEN;
		if (current == human)
		{
			puts("Starting with human...");
			battleship_log(NORMAL_PRINT, "Starting with human.");
			PAUSE;
		}
		else
		{
			puts("Starting with ai...");
			battleship_log(NORMAL_PRINT, "Starting with ai.");
			PAUSE;
		}
		/////////////////////////////
		/// Game Loop
		/////////////////////////////
		do
		{
			

			/// Set context for game commands
			if (current == human)
			{
				opposition = ai;
				battleship_log(NORMAL_PRINT, "PLAYER 1's TURN ===============");
			}
			else
			{
				opposition = human;
				battleship_log(NORMAL_PRINT, "PLAYER 2's TURN ===============");
			}

			// Print boards and get input if it's human's turn
			if (current == human) 
			{			
				// loop vars
				coordinate_t selected_coord;
				#define SHOW_BOARDS printf("=== Player %d's turn ===\n\n", current+1);\
					puts("Opponent:");\
					print_board(players[opposition].gameboard, false);\
					puts("\nYour board:");\
					print_board(players[current].gameboard, true)

				// Coordinate Input Loop
				message = "";
				while (true)
				{
					CLEAR_SCREEN;
					SHOW_BOARDS;
					if (get_coordinate(message, &selected_coord, 1))
					{
						// Successfully got coordinate input, apply to board.
						if (damage_board(&players[opposition], &selected_coord)) break;
						else message = "You already shot there.";
					} 
					else
					{
						message = "Invalid coordinate.";
					}
				}
				
				CLEAR_SCREEN;
				SHOW_BOARDS;
				puts("");
				PAUSE;
				#undef SHOW_BOARDS
			}
			else
			// AI takes turn here.
			{
				CLEAR_SCREEN;
				puts("AI is taking turn...");

				battleship_log(DEBUG_PRINT, "Taking AI turn.");
				ai_take_turn(&players[ai], &players[human]);
				
				puts("Result (your board):");
				print_board(players[human].gameboard, true);
				puts("");

				PAUSE;
			} 
			

			if (!(players[opposition].surviving_ships))
			{
				player_lost = true;
			}
			else
			{
				if (current == human)
					current = ai;
				else 
					current = human;
			}
		} while (!player_lost);

		/////////////////////////////
		/// Post-game
		/////////////////////////////
		switch(opposition)
		{
		case human:
			message = "You lost!";
			battleship_log(NORMAL_PRINT, "PLAYER 2 WINS!");
			break;
		case ai:
			message = "You won!";
			battleship_log(NORMAL_PRINT, "PLAYER 1 WINS!");
			break;
		}
		CLEAR_SCREEN;
		puts(message);
		message = "";
		PAUSE;
		CLEAR_SCREEN;
		playing = yn_prompt("Do you want to play again?");
	} while(playing);
	puts("Thanks for playing!");
#endif

	// close the log files.
	battleship_log(NORMAL_PRINT, NULL);
	battleship_log(DEBUG_PRINT, NULL);
	return 0;
}