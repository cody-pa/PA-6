#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include "../kp-lib/kp-lib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// ==========================================================================
// MACROS
// ==========================================================================
#define ERROR_CODES\
	X(no_error, ""),\
	X(already_shot, "Don't shoot in the same place twice.")

// ==========================================================================
// TYPES
// ==========================================================================
typedef enum _player {
	human,
	ai
} player_t;

typedef enum _battleship_square
{
	empty = '-',
	pegged = 'X',
	carrier = 'c', //5
	battleship = 'b', //4
	destroyer = 'd', //3
	submarine = 's', //3
	patrol_boat = 'p' //2
} battleshipsquare_t;

typedef enum _error_code
{
	#define X(ERR_CODE, ERR_MESSAGE) ERR_CODE
	ERROR_CODES
	#undef X
} errorcode_t;

typedef struct coordinate {
	unsigned char x;
	unsigned char y;
} coordinate_t;

typedef struct player_data
{
	battleshipsquare_t gameboard[10][10];
	unsigned char surviving_ships;
} playerdata_t;

// ==========================================================================
// FUNCTIONS
// ==========================================================================
void set_board_manually(playerdata_t * playerdata);
void set_board_automatically(playerdata_t * playerdata);
errorcode_t damage_board(battleshipsquare_t victim[10][10], coordinate_t * coord);
bool get_coordinate(const char * err, coordinate_t * coord);
void generate_coord(coordinate_t * coord);
void ai_check_opponent_coord(playerdata_t * opponent, coordinate_t * coord);
player_t choose_starting_player();
void print_board(battleshipsquare_t board[10][10]);

#endif // BATTLESHIP_H