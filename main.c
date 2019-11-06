#include <stdio.h>
#include "battleship.h"
#include "../kp-lib/kp-lib.h"
#include <time.h>
#include <signal.h>

//#define TEST

int main (void)
{
	srand(time(0));
#ifdef TEST
	
	CLEAR_SCREEN;
	playerdata_t playerdat;
	for (int z = 0; z < 5; ++z)
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int k = 0; k < 10; ++k)
			{
				playerdat.gameboard[i][k] = empty;
			}
		}
		
		set_board_automatically(playerdat.gameboard);

		print_board(playerdat.gameboard, true);
	}
	PAUSE;

#else
	bool playing = true;
	const char * message;
	do
	{
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
			// loop vars
			coordinate_t selected_coord;

			/// Set context for game commands
			if (current == human)
				opposition = ai;
			else
				opposition = human;

			// Print boards and get input if it's human's turn
			if (current == human) 
			{			
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
						break;
					message = "Invalid coordinate.";
				}
				// Successfully got coordinate input, apply to board.
				damage_board(&players[opposition], &selected_coord);
				CLEAR_SCREEN;
				SHOW_BOARDS;
				PAUSE;
				#undef SHOW_BOARDS
			}
			else
			// AI takes turn here.
			{
				generate_coord(&selected_coord, 9, 9);
				damage_board(&players[opposition], &selected_coord);
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
			break;
		case ai:
			message = "You won!";
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