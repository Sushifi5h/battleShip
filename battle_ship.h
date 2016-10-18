/*******************************************************************************************
* Student: Vikram Singh StudentID: 11491025                                                *
* Class: CptS 121, Fall  2015; Lab Section 5                                               *
* Programming Assignment 6: Battle ship 						                                       *
* Date: November 01, 2015                                                                  *
* Description: This program will allow user to play battleship							               *
*																						                                               *
*Relevant functions:																	                                     *
*																						                                               *
*Other Requirements: 1 player                           								                   *
*******************************************************************************************/

//The next to lines help this header file from being called over and also know as (Including gaurd) //Learned from wikki and youtube (https://en.wikipedia.org/wiki/Include_guard)
#ifndef BATTLE_SHIP_H
#define BATTLE_SHIP_H

//Stops annoying errors
#define _CRT_SECURE_NO_WARNINGS //Stop unwanted printf errors

//Including libarises that will be used to write this program
#include <stdio.h> //Fuctions that will be used but not limited to scanf (), printf (), fopen (), fclose (), fscanf (), fprintf ()
#include <stdlib.h> //Need to generate rand numbers and clearing screen and pausing program
#include <time.h> //Need to seed different values for srand

//Data Structs

//game options
typedef struct main_game_options{
	int option_selector;
} Battleship_Game_Options;


//game board
typedef struct game_board{
	//game boards arrays
	char player_game_board[10][10];
  	char computer_game_board[10][10];
  	char empty_game_board[10][10];

  	//dimmessions
	int game_board_row;
	int game_board_col;
} Battleship_Game_Board;

//game stats
typedef struct game_stats{
	int hits;
	int miss;
	int shots;
	int miss_to_loss;
	int won;
	int lost;
} Game_stats;

//Prototypes
int battle_ship_game_options_selector (void); //returns an int from the user 
void main_game (void); //runs main game/contains logic of the game
void header_battleship (void) ; //prints header to screen
void intro_battleship (void); //intro to gamess
void option_print (void); //just prints options
void games_logic (char player_board[][10], char computer_board[][10], char empty_board[][10]);//contains games logic
void exit_game (void); //exits game returns 0
void game_rules (void); //print game rules
void init_game_board (char board[][10], int row, int col);//used to add water to game board
void print_game_board (char board[][10], int row, int col); //prints board
int ask_man_or_auto_placement (void); //ask player for man or auto
int rand_true_or_false (void);//returns 0 or 1
void place_ships_man (char board[][10]); //place ships manual
char check_char_on_board_return_char (char board[][10], int coords_x, int coords_y); //checks to see the car on teh coords
int check_char_on_board (char board[][10], int array_of_coords[], int num_compare); //itterate through array of coords to find what is needed
void place_ships_auto (char board[][10]); //place ships on the board auto
void gen_start_pt (int length, int dir, int *row_start_ptr, int *col_start_ptr); //gives starting location
void load_array (int array_of_coords[], int row_start, int col_start, int direction, int size); //loads an array with coords
void put_ship (char board[][10], int array_of_coords[], int size, char ship); //place ship on board
void computer_shoots (int *row, int *col);//computer taking shots
int is_hit_or_miss (char board[][10], int row, int col); //check for hit
int is_winner (int player_hits, int computer_hits); //check for winners
void better_array_loader (int row[], int col[], int row_start,
                            int col_start, int direction , int ship); //better loader
void better_put_ship (char board[][10], int row[], int col[], int size, char ship); //better ship putter
char beter_check_char_on_board_return_char (char board[][10], int row, int col); //better char checker
int better_check_char_on_board (char board[][10], int row[], int col[], int ship_size); //better board checker

#endif //This statement ends the gaurd