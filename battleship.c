#include "battleship.h"

/// Date Created: 10/26/2019
/// Date Modified: 10/26/2019
/// Input params: none
/// Output params: none
/// Returns: boolean value
/// Preconditions: start of program, functional input.
/// Postconditions: none.
/// Description: Prompts the player for whether they are going to set the board manually or automatically.
bool ask_set_board_manually(void)
{
	do
	{
		char input[4] = { '\0' };
		CLEAR_SCREEN;
		printf("Do you wish to set your ships manually or automatically?\n >> ");
		input_string_and_flush(input, 4);

		for (int i = 0; input[i] != '\0' && i < 4; ++i)
		{
			input[i] = tolower(input[i]);
		}
		printf("Input received: %s\n", input);
		if (!strcmp(input, "yes"))
		{
			return true;
		}
		else if (!strcmp(input, "no"))
		{
			return false;
		}

	} while (true);
}

void set_board_manually(playerdata_t * playerdata)
{
	UNIMPLEMENTED(set_board_manually);
	UNUSED(playerdata);
}
void set_board_automatically(playerdata_t * playerdata)
{
	UNIMPLEMENTED(set_board_automatically);
	UNUSED(playerdata);
}
errorcode_t damage_board(playerdata_t * victim, coordinate_t * coord)
{
	UNIMPLEMENTED(damage_board);
	victim->surviving_ships = 0;
	UNUSED(coord);
	return no_error;
}

void get_coordinate(coordinate_t * coord)
{
	UNIMPLEMENTED(get_coordinate);
	UNUSED(coord);
}

void generate_coord(coordinate_t * coord)
{
	UNIMPLEMENTED(generate_coord);
	UNUSED(coord);
}

void ai_check_opponent_coord(playerdata_t * opponent, coordinate_t * coord)
{
	UNIMPLEMENTED(ai_check_opponent_coord);
	UNUSED(opponent);
	UNUSED(coord);
}

/// Date Created: 10/26/2019
/// Date Modified: 10/26/2019
/// Input params: none
/// Output params: none
/// Returns: random player_t enum
/// Preconditions: 
/// Postconditions: 
/// Description: 
player_t choose_starting_player()
{
	return RANDOM_RANGE(human, ai);
}

/// Date Created: 
/// Date Modified: 
/// Input params: 
/// Output params: 
/// Returns: 
/// Preconditions: 
/// Postconditions: 
/// Description: 
bool ask_play_again(void)
{
	const char * error_message = "";
	char input = '\0';
	bool play_again = false;
	while (true)
	{
		
		CLEAR_SCREEN;
		printf("Do you wish to play again (y/n)?\n\n* %s\n\n >> ", error_message);
		input = getchar();
		if (input == 'y') 
		{
			play_again = true;
			break;
		}
		else if (input == 'n')
		{
			play_again = false;
			break;
		}

		error_message = "Please input y or n.";
	}
	while ((input = getchar()) != '\n') ;
	return play_again;
}