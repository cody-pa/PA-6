#include "battleship.h"

/// Date Created: 10/26/2019
/// Date Modified: 10/26/2019
/// Input params: 
/// Output params: playerdata_t pointer
/// Returns: none
/// Preconditions: 
/// Postconditions: 
/// Description: prints the board
void print_board(playerdata_t * playerdata)
{
	
	for (int i = 0; i < 10; ++i)
	{
		// once per row
		printf("%d |", 9-i);
		for (int k = 0; k < 10; ++k)
		{
			fputs(" ", stdout);
			fputs((char*)&playerdata->gameboard[i][k], stdout);
		}
		puts("");
	}
	puts("--+----------------------\n  | A B C D E F G H I J");
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

bool get_coordinate(const char * err, coordinate_t * coord)
{
	char input[3] = { '\0' };
	printf("Input a target coordinate.\n\n * %s \n\n >> ", err);
	input_line(input, 3);
	size_t x = 9 - ((size_t)(input[0]-'0'));
	if ( x <= 9)
	{
		size_t y =(size_t)(input[1]-'A');
		if (y <= 9)
		{
			coord->x = x;
			coord->y = y;
			return true;
		}
	}
	return false;
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
/// Description: chooses random number between 0 and 1
player_t choose_starting_player()
{
	return RANDOM_RANGE(human, ai);
}