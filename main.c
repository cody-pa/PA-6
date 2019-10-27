#include <stdio.h>
#include "battleship.h"
#include "../kp-lib/kp-lib.h"
#include <time.h>

//#define TEST

int main (void)
{
	srand(time(0));
#ifdef TEST
	playerdata_t player;
	coordinate_t coord;
	
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
			set_board_automatically(&players[human]);
		}

		player_t current = choose_starting_player();
		player_t opposition;
		bool player_lost = false;
		CLEAR_SCREEN;
		if (current == human)
		{
			puts("Starting with human...");
			PAUSE;
		}
		else
		{
			puts("Starting with ai...");
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

			// get input if human move, automatic input if ai
			if (current == human) 
			{			
				message = "";
				while (true)
				{
					CLEAR_SCREEN;
					printf("=== Player %d's turn ===\n", 1);
					print_board(&player);
					if (get_coordinate(message, &coord))
						break;
					message = "Invalid coordinate.";
				}
				CLEAR_SCREEN;
				player.gameboard[coord.x][coord.y] = pegged;
				print_board(&player);
				PAUSE;
			}
			else
			{
				generate_coord(&selected_coord);
			} 

			damage_board(&players[opposition], &selected_coord);

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
		const char * exit_message;
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
	return 0;
}