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

//include local header file 
#include "battle_ship.h" //conatins all libaries used, data types, and prototypes 

int main (void)
{
	intro_battleship (); //brief intro
	main_game (); //runs game contains main logic
	return 0; //if everything finishes return 0.
}