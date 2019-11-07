#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include "../kp-lib/kp-lib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#define DEBUG

// ==========================================================================
// MACROS
// ==========================================================================
#define ERROR_CODES\
	X_LIST(no_error, "")\
	X_END(already_shot, "Don't shoot in the same place twice.")

#define SHIPS\
	X_LIST(carrier, 'C', 5)\
	X_LIST(battleship, 'b', 4)\
	X_LIST(cruiser, 'c', 3)\
	X_LIST(submarine, 's', 3)\
	X_END(destroyer, 'd', 2)

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
	hit = '*',
	miss = 'm',
	#define X_LIST(SHIP, CHAR, NUM) SHIP = CHAR,
	#define X_END(SHIP, CHAR, NUM) SHIP = CHAR
	SHIPS
	#undef X_LIST
	#undef X_END
} battleshipsquare_t;

typedef enum {
	DEBUG_PRINT,
	NORMAL_PRINT
} debug_mode_t;

typedef struct coordinate {
	unsigned char row;
	unsigned char col;
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
void set_board_automatically(battleshipsquare_t board[10][10]);
bool damage_board(playerdata_t * victim, coordinate_t * coord);
bool get_coordinate(const char * err, coordinate_t * coord, short unsigned int count);
void generate_coord(coordinate_t * coord, unsigned char max_row, unsigned char max_col);
void ai_take_turn(playerdata_t * self, playerdata_t * opponent);
player_t choose_starting_player();
void print_board(battleshipsquare_t board[10][10], bool show_ships);
bool battleship_log(debug_mode_t mode, const char * fmt, ...);
bool place_ship_on_board(battleshipsquare_t gameboard[10][10], coordinate_t coords[], battleshipsquare_t symbol, unsigned char size);

#endif // BATTLESHIP_H