#include "battleship.h"

typedef struct {
	const char * name;
	unsigned char size;
	battleshipsquare_t symbol;
} battleship_struct;

const battleship_struct battleship_types[5] = {
	#define X_LIST(SHIP, CHAR, NUM) { .name = STRINGIFY(SHIP), .size = NUM, .symbol = SHIP },
	#define X_END(SHIP, CHAR, NUM) { .name = STRINGIFY(SHIP), .size = NUM, .symbol = SHIP }
	SHIPS
	#undef X_LIST
	#undef X_END
};


/// Date Created: 10/26/2019
/// Date Modified: 11/06/2019
/// Input params: 
/// Output params: playerdata_t pointer
/// Returns: none
/// Preconditions: 
/// Postconditions: 
/// Description: prints the board
void print_board(battleshipsquare_t board[10][10], bool show_ships)
{
	UNUSED(show_ships);	
	for (int i = 0; i < 10; ++i)
	{
		// once per row
		printf("%d |", 9-i);
		for (int k = 0; k < 10; ++k)
		{
			fputs(" ", stdout);
			char c[2] = {
				board[i][k],
				'\0'
			};
			
			if (!show_ships)
			{
				switch(*c)
				{
				default:
					c[0] = empty;
				case hit:
				case miss:
					break;
				}
			}
			fputs(c, stdout);
			
		}
		puts("");
	}
	puts("--+----------------------\n  | A B C D E F G H I J");
}

/// Date Created: 11/05/2019
/// Date Modified: 11/05/2019
/// Input params: 
/// Output params: playerdata pointer
/// Returns: none
/// Preconditions: 
/// Postconditions: 
/// Description: get player input to set  ships
void set_board_manually(playerdata_t * playerdata)
{
	battleship_log(DEBUG_PRINT, "Started call of set_board_manually.");
	coordinate_t input_coords[5];
	char * message = "";
	for (int i = 0; i < 5; ++i)
	{
		battleship_log(DEBUG_PRINT, "Getting coords for '%s'.", battleship_types[i].name);
		bool placed_successfully = false;
		do
		{
			CLEAR_SCREEN;
			print_board(playerdata->gameboard, true);
			printf("Place your %s.\n", battleship_types[i].name);
			if (get_coordinate(message, input_coords, battleship_types[i].size))
			{
				battleship_log(DEBUG_PRINT, "Got input successfully.");
				if (place_ship_on_board(playerdata->gameboard, input_coords, battleship_types[i].symbol, battleship_types[i].size))
				{
					battleship_log(DEBUG_PRINT, "Successfully placed ship.");
					placed_successfully = true;
					message = "";
				}
				else
				{
					battleship_log(DEBUG_PRINT, "Ship was malformed.");
					message = "Invalid coord arrangement.";
				}
			}
			else
			{
				battleship_log(DEBUG_PRINT, "Coordinate input validation failed.");
				message = "Invalid placement.";
			}
		} while (!placed_successfully);
	}
	CLEAR_SCREEN;
	print_board(playerdata->gameboard, true);
	puts("This is your board.");
	PAUSE;
	battleship_log(DEBUG_PRINT, "Call to set_board_manually complete.");
}

/// Date Created: 10/26/2019
/// Date Modified: 11/06/2019
/// Input params: 
/// Output params: battleshipsquare 2D array
/// Returns:
/// Preconditions: 
/// Postconditions: 
/// Description: generate ships for board
void set_board_automatically(battleshipsquare_t board[10][10])
{
	coordinate_t ship_coords[5];
	// iterate through each time of ship
	for (int current_ship = 0; current_ship < 5; ++current_ship)
	{
		bool placed_successfully;

		battleship_log(DEBUG_PRINT, "Generating %s.", battleship_types[current_ship].name);
		// for each ship, keep trying until success
		do {
			bool horizontal = RANDOM_RANGE(0, 1);
			placed_successfully = true;
			if (horizontal)
			{
				// generate first coord and then the rest
				generate_coord(&ship_coords[0], 9, 10-battleship_types[current_ship].size);
				for (int ship_segment = 1; ship_segment < 5; ++ship_segment)
				{
					ship_coords[ship_segment].row = ship_coords[0].row;
					ship_coords[ship_segment].col = ship_coords[0].col + ship_segment;
					if (board[ship_coords[ship_segment].row][ship_coords[ship_segment].col] != empty)
					{
						battleship_log(DEBUG_PRINT, "collission occurred.");
						placed_successfully = false;
						break;
					}
				}
			}
			else
			{
				generate_coord(&ship_coords[0], 10-battleship_types[current_ship].size, 9);
				for (int ship_segment = 1; ship_segment < 5; ++ship_segment)
				{
					ship_coords[ship_segment].row = ship_coords[0].row+ship_segment;
					ship_coords[ship_segment].col = ship_coords[0].col;
					if (board[ship_coords[ship_segment].row][ship_coords[ship_segment].col] != empty)
					{
						battleship_log(DEBUG_PRINT, "collission occurred.");
						placed_successfully = false;
						break;
					}
				}
			}
			// after all coords generated, if no collision has happened then write them to the board.
			if (placed_successfully)
			{
				for (int k = 0; k < battleship_types[current_ship].size; ++k)
				{
					board[ship_coords[k].row][ship_coords[k].col] = battleship_types[current_ship].symbol;
				}
			}
		} while (!placed_successfully);
	}
}
errorcode_t damage_board(playerdata_t * victim, coordinate_t * coord)
{

	UNIMPLEMENTED(damage_board);
	victim->surviving_ships = 0;
	UNUSED(coord);
	return no_error;
}

/// Date Created: 10/26/2019
/// Date Modified: 11/05/2019
/// Input params: error message string, size of coordinate array
/// Output params: coordinate array
/// Returns: boolean if operation was successful
/// Preconditions: 
/// Postconditions: 
/// Description: gets a list of comma seperated coordinates. ignores whitespace
bool get_coordinate(const char * err, coordinate_t coord[], short unsigned int count)
{
	battleship_log(DEBUG_PRINT, "Start get_coordinate call.");
	char input[100] = { '\0' };
	switch(count)
	{
	case 0:
		battleship_log(DEBUG_PRINT, "ERROR: invalid count!");
		battleship_log(DEBUG_PRINT, NULL);
		battleship_log(NORMAL_PRINT, NULL);
		CLEAR_SCREEN;
		printf("CRITICAL ERROR: get_coordinate was passed an invalid count of zero!\n");
		PAUSE;
		exit(1);
		return false;
	case 1:
		printf("Input a target coordinate.\n\n * %s \n\n >> ", err);
		break;
	default:
		printf("Input %d target coordinates in vertical or horizontal sequence, seperated by spaces.\n\n * %s \n\n >> ", count, err);
	}
	
	input_line(input, 100);

	//battleship_log("Started scanning coordinates.");
	//battleship_log("")
	char * token = strtok(input, " ");
	short unsigned int tok_count = 0; 
	while (token != NULL && tok_count < count)
	{
		battleship_log(DEBUG_PRINT, "Token: %s", token);
		if (token[0] == '\0') 
		{
			battleship_log(DEBUG_PRINT, "Error: reached unexpected \\0 while scanning input.");
			return false;
		}
		unsigned char x = 9 - ((unsigned char)(token[0]-'0'));
		if ( x <= 9)
		{
			if (token[1] == '\0') 
			{
				battleship_log(DEBUG_PRINT, "Error: reached unexpected \\0 while scanning input.");
				return false;
			}
			if (token[1] >= 'a' && token[1] <= 'z' )
			{
				token[1] -= 'a' - 'A';
			}
			unsigned char y =(unsigned char)(token[1]-'A');
			if (y <= 9)
			{
				coord[tok_count].row = x;
				coord[tok_count].col = y;
				battleship_log(DEBUG_PRINT, "Parse success: coord[%d] = %d, %d", tok_count, coord[tok_count].row, coord[tok_count].col);
			}
			else 
			{
				battleship_log(DEBUG_PRINT, "ERROR: failure parsing y.");
				return false;
			}
		}
		else 
		{
			battleship_log(DEBUG_PRINT, "Error: failure parsing x.");
			return false;
		}

		++tok_count;
		token = strtok(NULL, " ");
	}
	battleship_log(DEBUG_PRINT, "Token is NULL.");
	if (tok_count < count)
	{
		battleship_log(DEBUG_PRINT, "ERROR: Didn't get enough input.");
		return false;
	}
	battleship_log(DEBUG_PRINT, "Call to get_coordinate complete.");
	return true;
}

/// Date Created: 10/26/2019
/// Date Modified: 11/06/2019
/// Input params: max_row, max_col
/// Output params: coord
/// Returns:
/// Preconditions: 
/// Postconditions: 
/// Description: generates a random coordinate according to the specified bounds
void generate_coord(coordinate_t * coord, unsigned char max_row, unsigned char max_col)
{
	if (max_row > 9 || max_col > 10)
	{
		battleship_log(DEBUG_PRINT, "Invalid max was passed to generate_coord!");
		coord->row = 9;
		coord->col = 9;
		return;
	}

	coord->row = RANDOM_RANGE(0, max_row);
	coord->col = RANDOM_RANGE(0, max_col);
	battleship_log(DEBUG_PRINT, "generate_coord generated %d, %d.", coord->row, coord->col);
}

void ai_check_opponent_coord(playerdata_t * opponent, coordinate_t * coord)
{
	UNIMPLEMENTED(ai_check_opponent_coord);
	UNUSED(opponent);
	UNUSED(coord);
}

/// Date Created: 10/26/2019
/// Date Modified: 11/05/2019
/// Input params: none
/// Output params: none
/// Returns: random player_t enum
/// Preconditions: 
/// Postconditions: 
/// Description: chooses random number between 0 and 1
player_t choose_starting_player()
{
	player_t result = RANDOM_RANGE(human, ai);
	battleship_log(DEBUG_PRINT, "choose_starting_player = %d", result);
	return result;
}

/// Date Created: 11/05/2019
/// Date Modified: 11/05/2019
/// Input params: debug enum, format string, variadic args
/// Output params: 
/// Returns: true of logging was successful, false if something went wrong.
/// Preconditions: 
/// Postconditions: 
/// Description: logs the message, or closes the log file if message is NULL. logs either deubug or normal.
bool battleship_log (debug_mode_t mode, const char * fmt, ...)
{
	static FILE * logfile = NULL;
	static FILE * debugfile = NULL;

	// decide which file we're using.
	FILE ** ofile;
	if (mode == DEBUG_PRINT)
	{
		ofile = &debugfile;
	}
	else ofile = &logfile;

	time_t current_time;
	if (fmt)
	{
		if (*ofile == NULL)
		{
			char buffer[50] = { '\0' };

			time(&current_time);
			time(&current_time);
			struct tm* now = localtime(&current_time);
			sprintf(buffer, "%s-%04d-%02d-%02d-%02d-%02d-%02d.log", 
					(mode == DEBUG_PRINT) ? "debug-logs/debug" : "games/game-log",
					now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, 
					now->tm_min, now->tm_sec);

			*ofile = fopen(buffer, "w");
		}

		if (*ofile != NULL)
		{
			if (mode == DEBUG_PRINT)
			{
				time(&current_time);
				struct tm* now = localtime(&current_time);
				fprintf(*ofile, "[%04d/%02d/%02d %02d:%02d:%02d]: ", 
					now->tm_year + 1900, now->tm_mon+1, now->tm_mday, 
					now->tm_hour, 
					now->tm_min, now->tm_sec);
			}
			va_list argptr;
			va_start(argptr, fmt);
			vfprintf(*ofile, fmt, argptr);
			va_end(argptr);

			fprintf(*ofile, "\n");
			fflush(*ofile);
		}
		else 
		{
			fprintf(stderr, "Failed to open log.");
			return false;
		}
	}
	else
	{
		if (*ofile != NULL && fclose(*ofile) == EOF)
		{
			fprintf(stderr, "Failed to close file.");
			return false;
		}
	}

	return true;
}

/// Date Created: 11/05/2019
/// Date Modified: 11/05/2019
/// Input params: size of coord array
/// Output params: gameboard, coord array
/// Returns: true if ship is valid, false otherwise
/// Preconditions: 
/// Postconditions: 
/// Description: attempts to place the ship on the board. returns false if it can't.
bool place_ship_on_board(battleshipsquare_t gameboard[10][10], coordinate_t coords[], battleshipsquare_t symbol, unsigned char size)
{
	if (size < 2) return false;

	bool row[5] = {0};
	// determine which direction the ship is facing
	bool horizontal = (coords[0].row == coords[1].row);
	unsigned char leftmost = coords[0].col;
	unsigned char bottommost = coords[0].row;

	// determine the leftmost / bottommost extreme of the ship coords
	// and check for overlap with other ships
	for (int i = 0; i < size; ++i)
	{
		if (coords[i].row > 9 || coords[i].col > 9)
		{
			return false;
		}
		if (gameboard[coords[i].row][coords[i].col] != empty)
		{
			battleship_log(DEBUG_PRINT, "Coordinate was already taken.");
			return false;
		}
		if (horizontal)
		{
			if (coords[i].row != bottommost)
			{
				return false;
			}
			if (coords[i].col < leftmost) leftmost = coords[i].col;
		}
		else
		{
			if (coords[i].col != leftmost)
			{
				return false;
			}
			if (coords[i].row < bottommost) bottommost = coords[i].row;
		}
		
	}
	// Make sure the ships are all in a row.
	{
		unsigned char place;
		if (horizontal)
		{
			for (int i = 0; i < size; ++i)
			{
				place = coords[i].col - leftmost;
				if (place >= 5 || row[place])
				{
					battleship_log(DEBUG_PRINT, "Coords were not sequential.");
					return false;
				}
				row[place] = true;
			}
		}
		else
		{
			for (int i = 0; i < size; ++i)
			{
				place = coords[i].row - bottommost;
				if (place >= 5 || row[place])
				{
					battleship_log(DEBUG_PRINT, "Coords were not sequential.");
					return false;
				}
				row[place] = true;
			}
		}
	}

	// if we've made it this far, it's a valid ship placement.
	battleship_log(DEBUG_PRINT, "Ship appears to be valid.");
	for (int i = 0; i < size; ++i)
	{
		battleship_log(DEBUG_PRINT, "Assigning playerdata.gameboard[%c][%c] = %c.", coords[i].row, coords[i].col, symbol);
		gameboard[coords[i].row][coords[i].col] = symbol;
	}

	return true;
}