#include <stdio.h>
#include "battleship.h"
#include "../kp-lib/kp-lib.h"

int main (void)
{
	bool playing = true;
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
			if (current == human) get_coordinate(&selected_coord);
				else generate_coord(&selected_coord);

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
			exit_message = "You lost!";
			break;
		case ai:
			exit_message = "You won!";
			break;
		}
		CLEAR_SCREEN;
		puts(exit_message);
		PAUSE;
		CLEAR_SCREEN;
		playing = yn_prompt("Do you want to play again?");
	} while(playing);
	puts("Thanks for playing!");
	return 0;
}