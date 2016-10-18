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

/**************************************************************
* Function: void main_game (void) 		  		              *
* Date Created: 11/01/15                                      *
* Date Last Modified: 11/01/15                                *
* Description: runs games contains all logic 			      *
* Input parameters: None                                      *
* Returns: Nothing                                            *
* Preconditions: calling it.                                  *
* Postconditions: starts game logic                           *
 *************************************************************/
void main_game (void)
{
  //allocate memory for structs
  Battleship_Game_Options main_game_vars = {0};
  Battleship_Game_Board game_board;

  srand ((unsigned int) time (NULL)); //seeds rand so we get different values

  //iniliaze game boards and size
  game_board.game_board_row = 10;
  game_board.game_board_col = 10;
  init_game_board ( game_board.player_game_board, game_board.game_board_row, //init gameboard for player
                    game_board.game_board_col);
  init_game_board ( game_board.computer_game_board, game_board.game_board_row, //init game board for computer
                    game_board.game_board_col);
  init_game_board ( game_board.empty_game_board, game_board.game_board_row, //init game board for computer
                    game_board.game_board_col);


  //get player choice
  main_game_vars.option_selector = battle_ship_game_options_selector ();

  //Depends on player decision
  switch (main_game_vars.option_selector)
  {
    //Game Starts
    case 1:
      games_logic (game_board.player_game_board, game_board.computer_game_board, game_board.empty_game_board); //contains games logic
      main_game ();
      break; //stop execution of playing game

    //Game rules
    case 2:
      game_rules (); //print game rules to the screen
      main_game (); //restart game
      break; //stop execution of game rules

    //Exit Game
    case 3:
      exit_game (); //exit game with a print message
      break; //stop execution of exit code 
  }
}
/*************************************************************
* Function: games_logic (board)                              *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: contains game logic inside                    *
* Input parameters: none                                     *
* Return: none                                               *
* Preconditions: game needs to be started                    *
* Postconditions: game will start                            *
*************************************************************/
void games_logic (char player_board[][10], char computer_board[][10], char empty_board[][10])
{
  //local var
  char last_output[200] = "\0";
  int choice_of_auto_or_man_placement = 0, first_player = 0, computer_row = 0, computer_col = 0,
  winner = 0, player_row = 0, player_col = 0, total_hit = 0;
  FILE *data_file = NULL;

  //struct
  Game_stats game_stats_player;
  Game_stats game_stats_computer;

  data_file = fopen("battleship.log", "w");

  if (NULL == data_file)
  {
    printf ("Couldn't open File: battleship.log.\n");
    exit (1);
  }

  game_stats_player.shots = 0;
  game_stats_player.hits = 0;
  game_stats_player.miss = 0;
  game_stats_player.won = 0;
  game_stats_player.lost = 0;

  game_stats_computer.shots = 0;
  game_stats_computer.hits = 0;
  game_stats_computer.miss = 0;
  game_stats_computer.won = 0;
  game_stats_computer.lost = 0;

  //Game
  choice_of_auto_or_man_placement = ask_man_or_auto_placement ();

  //Ship Placements
  switch (choice_of_auto_or_man_placement)
  {
    //manual placement
    case 1:
      place_ships_man (player_board); //place ships mannually
      break;
    //auto placement
    case 2:
      place_ships_auto (player_board); //place ships auto
      break;
  }
  //place computer ships
  place_ships_auto (computer_board);

  //first
  first_player = rand_true_or_false (); //if 0 computer if 1 player

  //main game
  winner = 0;
  while (winner == 0)
  {
    //print boards
    print_game_board (player_board, 10, 10);
    print_game_board (empty_board, 10, 10);

	//players turn
    puts ("Players turn.");
    fprintf (data_file, "Players turn.\n");

    //ask player
    puts ("Please enter coordinates to shoot.");
    scanf ("%d %d", &player_row, &player_col);
    fprintf (data_file, "Players shots %d %d.\n", player_row, player_col);

    //itterate shot
    game_stats_player.shots++;


    //check player for hit or miss
    if (is_hit_or_miss(computer_board, player_row, player_col) == 1)
    {
      puts ("It was a hit.");
      fprintf (data_file, "Players hits %d %d.\n", player_row, player_col);
      game_stats_player.hits++;
      empty_board[player_col][player_row] = '*';
    }else{
      puts ("It was a miss.");
      fprintf (data_file, "Players misses %d %d.\n", player_row, player_col);
      game_stats_player.miss++;
      empty_board[player_col][player_row] = 'M';
    }

    //computers turn
    puts ("Computer turn.");
    fprintf (data_file, "Computers turn\n");
    computer_shoots (&computer_row, &computer_col);
    fprintf (data_file, "Computer shoots %d %d.\n", computer_row, computer_col);

	//itterate shot
    game_stats_computer.shots++;

    //check computer for hit or miss
    if (is_hit_or_miss(player_board, computer_row, computer_col) == 1)
    {
      puts ("It was a hit.");
      fprintf (data_file, "Computer hits %d %d.\n", computer_row, computer_col);
      game_stats_computer.hits++;
      player_board[computer_col][computer_row] = '*';
    }else{
      puts ("It was a miss.");
      fprintf (data_file, "Computer miss %d %d.\n", computer_row, computer_col);
      game_stats_computer.miss++;
      player_board[computer_col][player_row] = 'M';
    }

    //check to see if game is over if we have a winner
    winner = is_winner (game_stats_player.hits, game_stats_computer.hits);

    //check for if player won
    if (game_stats_player.hits == 17)
    {
      game_stats_player.won++;
      game_stats_computer.lost++;
      puts ("Player 1 wins!");
      fprintf (data_file, "Player 1 wins!");
    }

    //check for if computer won
    if (game_stats_computer.hits == 17)
    {
      game_stats_computer.won++;
      game_stats_player.lost++;
      puts ("Computer wins!");
      fprintf (data_file, "Computer wins!");
    }

    system ("pause");
    system ("cls");
  }
  //game ending message
  puts ("Game Over.");
  puts ("Thanks for playing.");
  system ("pause");

  //Print stats to file
  fprintf (data_file, "Game Over.\n");

  fprintf( data_file, "Player\nHit: %d Miss: %d Total number shots: %d Hit to miss: %.2lf Won: %d Lost: %d\n", game_stats_player.hits, game_stats_player.miss, game_stats_player.shots, ((game_stats_player.miss)/(double)(game_stats_player.shots)), game_stats_player.won,  game_stats_player.lost);

  fprintf( data_file, "Computer\nHit: %d Miss: %d Total number shots: %d Hit to miss: %.2lf Won: %d Lost: %d\n", game_stats_computer.hits, game_stats_computer.miss, game_stats_computer.shots, ((game_stats_computer.miss)/(double)(game_stats_computer.shots)), game_stats_computer.won,  game_stats_computer.lost);

  //close file
  fclose (data_file);


}

/*************************************************************
* Function: void header_battleship (void)		  	             *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: assicc art from http://www.glassgiant.com/    *
* Input parameters: None                                     *
* Returns: Nothing                                           *
* Preconditions: calling it.                                 *
* Postconditions: darws on scree                             *
 *************************************************************/
void header_battleship (void) 
	{
	//print to screen
	printf ( 
  	"                                           7?         7                        \n" 
  	"                                          7I?7       77                        \n" 
  	"                                        777??777     7+7                       \n" 
  	"                                          ???I7    7777                        \n" 
  	"                                          ?I?=?7 77????77               ~~7    \n" 
  	"                                          ?????? 7+?=??+?+77      7=7~777?7777 \n" 
  	"                                        77??????7  ??++++==7      77==7777:7777\n" 
  	"                                       7???????:?I??+?7~:?++777=,?.I+====+I~~7 \n" 
  	"                                     7?=~+=,???+I+=::???++??++++:~~+=+=++=++   \n" 
  	"                                     I????+?,??~+=~??+?=+~??==~+==+==+?++++I77 \n" 
  	"                                   77+II?~,.+~+?+,:~+?,+~???===??++======~?I7  \n" 
  	"                               77+==++????????,++?~+I?+=+=?=+=========~?I77    \n" 
  	"                          7 777=I~?+??=+?+??~=I????+?=:=========~~:?I777       \n" 
  	"                           77:?~?+???=?~=+++??++=:?+=========~~?I777           \n" 
  	"                        I+I?????+????+=+++???+==========~:??I777               \n" 
  	"                    7=+=?+~=,??+:=+++??============~~??I777                    \n" 
  	" 7?          7:II????:+I?????I?I=============::=??I777                         \n" 
  	" 777777 7+7??=++??I+?I??I+=============~~:+??II777                             \n" 
  	"7II???++I????????================:::+??II7777                                  \n" 
  	" I?+??????==============~=:::+??III7777                                        \n" 
  	"  7=========~===~=~:,=+??III7777                                               \n" 
  	"   7===~===~~::=+??III77777                                                    \n" 
  	"   7=~~~:????II7777                                                            \n" 
  	"  77777777777\n");
 	}

/*************************************************************
* Function: void intro_battleship (void)                     *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description:                                               *
* Input parameters: None                                     *
* Returns: Nothing                                           *
* Preconditions: calling it.                                 *
* Postconditions: darws on screen/prints                     *
*************************************************************/
void intro_battleship (void)
 {
  //call header
  header_battleship ();

  //give intro to player
  printf ("\tYou are about to start a game of battleship. Get ready captain.\n");

  //pause game
  system ("pause");

  //clear screen
  system ("cls");
 }

/*************************************************************
* Function: void option_print (void)                         *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description:                                               *
* Input parameters: None                                     *
* Returns: Nothing                                           *
* Preconditions: calling it.                                 *
* Postconditions: darws on screen/prints                     *
*************************************************************/
void option_print (void)
{
  printf("\t\tBATTLE SHIP\n\n\n"
    "\tCaptian we have a battle ahead. Prepare your fleet.\n"
         "\t\t1. Prepare fleet (Start game)\n"
         "\t\t2. Print Rules\n"
         "\t\t3. Exit\n");
}

/*************************************************************
* Function: int battle_ship_game_options_selector (void)     *
* Date Created: 11/02/15                                     *
* Date Last Modified: 11/02/15                               *
* Description: prints gets input from user & clears screen   *
* Input parameters: None                                     *
* Returns: game options                                      *
* Preconditions: calling it.                                 *
* Postconditions: game directions might change               *
*************************************************************/
int battle_ship_game_options_selector (void)
{
  //local var
  int choice = 0;

  do
  {
    //clear screen
    system ("cls");

    //print game options
    option_print ();

    //ask for input
    printf("\n\n\tPlease give enter a choice (Ex: 1,2,3): ");

    //assign choice
    scanf (" %d", &choice);

    //choice correction message
    if ( (choice <= 0) || (choice >= 4) )
    {
      //print choice and message
      printf ("Please make another choice. You picked %d."
                " Which is not between 1 throught 3.\n", choice);

      //pause
      system ("pause");
    }

  } while ( (choice <= 0) || (choice >= 4) );

  //clear screen
  system ("cls");

  //return choice
  return choice;
}

/*************************************************************
* Function: exit_game (void)                                 *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description:                                               *
* Input parameters: None                                     *
* Returns: Nothing                                           *
* Preconditions: calling it.                                 *
* Postconditions: print a message/pause/exit game            *
*************************************************************/
void exit_game (void)
{
  puts ("You selected to exit. Goodbye Captain."); //exit message
  system ("pause"); //pause to wait for user to read message
  exit (0); //exit program with return value of 0
}

/*************************************************************
* Function: game_rules (void)                                 *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description:                                               *
* Input parameters: None                                     *
* Returns: Nothing                                           *
* Preconditions: calling it.                                 *
* Postconditions: print a message/                           *
*************************************************************/
void game_rules (void)
{
  //print game rules
  //rules taken from http://eecs.wsu.edu/~aofallon/cpts121/progassignments/PA6.htm
  puts ("~~~~~~~~~~~WELCOME TO BATTLESHIP~~~~~~~~~~~\n"
        "\t1. Battleship is a two player game.\n"
        "\t2. Player1 is you and Player2 is the computer.\n"
        "\t3. Player1 (you) will place your ships.\n"
        "\t4. Once the board is set the game will start.\n"
        "\t5. Each player will take turns shoting, trying to sink others' fleet.\n"
        "\t6. Player to sink others fleet will win.\n"
        "Press any key to return to the Main Menu.");

  //pause game so the user can read rules
  system ("pause");
}

/*************************************************************
* Function: init_game_board (array[][10] , row, col)         *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: sets all char to be water in game board       *
* Input parameters: 10x10 array, rows, col                   *
* Returns: Nothing                                           *
* Preconditions: array must exist.                           *
* Postconditions: gameboard has ~ for water                  *
*************************************************************/
void init_game_board (char board[][10], int row, int col)
{
  //local vars
  int row_counter = 0, col_counter = 0;

  //loop to itterate through rows and col
  for (; row_counter < row; row_counter++) //itterate through row
  {
    for ( col_counter = 0; col_counter < col; col_counter++)//itterate through col
    {
      board[row_counter][col_counter] = '~'; //set that row, col char to ~
    }
  }

}

/*************************************************************
* Function: print_game_board (array[][10] , row, col)        *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: print game board to screen                    *
* Input parameters: 10x10 array, rows, col                   *
* Returns: Nothing                                           *
* Preconditions: array must exist.                           *
* Postconditions: screen might get wet                       *
*************************************************************/
void print_game_board (char board[][10], int row, int col)
{
  //local vars
  int row_counter = 0, col_counter = 0, counter = 0;

  //compsate for space
  printf("    ");

  //will print top row will be the x axis
  for (counter = 0; counter < row; counter++)
  {
    printf ("%d ", counter );
  }

  //divider
  puts ("\n    - - - - - - - - - -");

  //loop to itterate through rows and col
  for (; row_counter < row; row_counter++) //itterate through rows
  {
    //will print the col y axis

	  printf("%d |", row_counter); 

    for ( col_counter = 0; col_counter < col; col_counter++)//itterate through cols
    {
      printf (" %c", board[row_counter][col_counter]); //set that row, col char to ~
    }
    printf ("\n");
  }

}

/*************************************************************
* Function: ask_man_or_auto_placement (void)                 *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: ask player for man or auto placement          *
* Input parameters: none                                     *
* Return: BOOLEAN                                            *
* Preconditions: call                                        *
* Postconditions: game direction will affected               *
*************************************************************/
int ask_man_or_auto_placement (void)
{
  //local var
  int choice = 0;

  do
  {
    //clear screen
    system ("cls");

    //print and ask for choice
    puts ("Please select from the following menu:\n\n"
          "\t1. Enter positions of ships manually.\n"
          "\t2. Allow the program to randomly select positions of ships.");

    //ask for input
    printf("\n\nPlease give enter a choice (Ex: 1 or 2): ");

    //assign choice
    scanf (" %d", &choice);

    //choice correction message
    if ( (choice <= 0) || (choice >= 3) )
    {
      //print choice and message
      printf ("Please make another choice. You picked %d."
                " Which is not 1 or 2.\n", choice);

      //pause
      system ("pause");
    }

  } while ( (choice <= 0) || (choice >= 3) );

  //clear screen
  system ("cls");

  //return choice
  return choice;

}

/*************************************************************
* Function: rand_first_selector (void)                       *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: choose first player to take a turn            *
* Input parameters: none                                     *
* Returns: TRUE or FALSE BOOLEAN                             *
* Preconditions:                                             *
* Postconditions: direction of game will change              *
*************************************************************/
int rand_true_or_false (void)
{
  //local var
  int random_num = 0;
  //flip a coin
  random_num = rand () % 2;
  //return 0 or 1
  return random_num;
}

/*************************************************************
* Function: place_ships_man (array, row, col)                *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: player enters ships locations                 *
* Input parameters: array row col                            *
* Returns: none                                              *
* Preconditions: array must exist                            *
* Postconditions: ships will be on the board                 *
*************************************************************/
void place_ships_man (char board[][10])
{
  //local var
  int row_col_holder[50] = {0}, counter = 0, checker = 1, placement = 0,
  add_array_x = 0, horziontal_x_check_with_math = 0, vertical_y_check_with_math = 0, add_array_y = 0,
  placement_checker = 0;
  int counter_temp = 0;

  //place carrier
  checker = 1;
  do
  {
    //print board
    print_game_board (board, 10, 10);

    //place carrier
    printf("Please enter the five cells to place the Carrier across.\n"
            "Enter coordinates example [0,1], [0,2] = 0 1 0 2\nEnter coordinates: ");
  
    scanf (" %d %d %d %d %d %d %d %d %d %d", &row_col_holder[11], &row_col_holder[1], &row_col_holder[12], &row_col_holder[2], &row_col_holder[13]
      , &row_col_holder[3], &row_col_holder[14], &row_col_holder[4], &row_col_holder[15], &row_col_holder[5]);
    
    //add all x axis
    add_array_x = 0;
    add_array_x = row_col_holder[1] + row_col_holder[2] + row_col_holder[3] + row_col_holder[4] + row_col_holder[5];

    //do math on x axis to check if horizontal
    horziontal_x_check_with_math = 0;
    if (add_array_x >= 1)
    {
      horziontal_x_check_with_math = add_array_x / (5 * row_col_holder[3]);
    }

    //add all y axis
    add_array_y = 0;
    add_array_y = row_col_holder[11] + row_col_holder[12] + row_col_holder[13] + row_col_holder[14] + row_col_holder[15];

    //do math on y axis to check if vertical
    vertical_y_check_with_math = 0;
    if (add_array_y >= 1)
    {
      vertical_y_check_with_math = add_array_y / (5 * row_col_holder[13]);
    }

    //horziontal checks
    if (add_array_x == 0)
    {
      //is horizontal 
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'c';
      board[row_col_holder[2]][row_col_holder[12]] = 'c';
      board[row_col_holder[3]][row_col_holder[13]] = 'c';
      board[row_col_holder[4]][row_col_holder[14]] = 'c';
      board[row_col_holder[5]][row_col_holder[15]] = 'c';
    } else if (horziontal_x_check_with_math == 1) {
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'c';
      board[row_col_holder[2]][row_col_holder[12]] = 'c';
      board[row_col_holder[3]][row_col_holder[13]] = 'c';
      board[row_col_holder[4]][row_col_holder[14]] = 'c';
      board[row_col_holder[5]][row_col_holder[15]] = 'c';
    }

    //vertical check
    if (add_array_y == 0)
    {
      //is vertical
      checker = 0;
      //place ship
      board[row_col_holder[1]][row_col_holder[11]] = 'c';
      board[row_col_holder[2]][row_col_holder[12]] = 'c';
      board[row_col_holder[3]][row_col_holder[13]] = 'c';
      board[row_col_holder[4]][row_col_holder[14]] = 'c';
      board[row_col_holder[5]][row_col_holder[15]] = 'c';
    } else if (vertical_y_check_with_math == 1){
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'c';
      board[row_col_holder[2]][row_col_holder[12]] = 'c';
      board[row_col_holder[3]][row_col_holder[13]] = 'c';
      board[row_col_holder[4]][row_col_holder[14]] = 'c';
      board[row_col_holder[5]][row_col_holder[15]] = 'c';
    }

    if (checker == 1)
    {
      //error message
      puts ("Your ship placements were either not vertical or horizontal. Please try again");

      //pause
      system ("pause");
    }

    //clear screen
    system ("cls");

    //reset array
    for (counter = 0; counter < 50; counter++)
    {
      row_col_holder[counter] = 0;
    }

  } while (checker == 1);

  //place battleship
  checker = 1;
  do
  {
    //print board
    print_game_board (board, 10, 10);

    //place battleship
    printf("Please enter the four cells to place the battleship across.\n" 
          "Enter coordinates example [0,1], [0,2] = 0 1 0 2\nEnter coordinates: ");
    scanf (" %d %d %d %d %d %d %d %d", &row_col_holder[11], &row_col_holder[1], &row_col_holder[12], &row_col_holder[2], &row_col_holder[13]
      , &row_col_holder[3], &row_col_holder[14], &row_col_holder[4]);

    //add all x axis
    add_array_x = 0;
    add_array_x = row_col_holder[1] + row_col_holder[2] + row_col_holder[3] + row_col_holder[4];

    //do math on x axis to check if horizontal
    horziontal_x_check_with_math = 0;
    if (add_array_x >= 1)
    {
      horziontal_x_check_with_math = add_array_x / (4 * row_col_holder[3]);
    }

    //add all y axis
    add_array_y = 0;
    add_array_y = row_col_holder[11] + row_col_holder[12] + row_col_holder[13] + row_col_holder[14];

    //do math on y axis to check if vertical
    vertical_y_check_with_math = 0;
    if (add_array_y >= 1)
    {
      vertical_y_check_with_math = add_array_y / (4 * row_col_holder[13]);
    }

    //check placement
    placement_checker = 0;
    placement_checker = check_char_on_board (board, row_col_holder, 4);

    //horziontal checks
    if (add_array_x == 0 && placement_checker)
    {
      //is horizontal 
      checker = 0;
      //place ships  
      board[row_col_holder[1]][row_col_holder[11]] = 'b';
      board[row_col_holder[2]][row_col_holder[12]] = 'b';
      board[row_col_holder[3]][row_col_holder[13]] = 'b';
      board[row_col_holder[4]][row_col_holder[14]] = 'b';
    } else if (horziontal_x_check_with_math == 1 && placement_checker) {
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'b';
      board[row_col_holder[2]][row_col_holder[12]] = 'b';
      board[row_col_holder[3]][row_col_holder[13]] = 'b';
      board[row_col_holder[4]][row_col_holder[14]] = 'b';
    }

    //vertical check
    if (add_array_y == 0 && placement_checker)
    {
      //is vertical
      checker = 0;
      //place ship
      board[row_col_holder[1]][row_col_holder[11]] = 'b';
      board[row_col_holder[2]][row_col_holder[12]] = 'b';
      board[row_col_holder[3]][row_col_holder[13]] = 'b';
      board[row_col_holder[4]][row_col_holder[14]] = 'b';
    } else if (vertical_y_check_with_math == 1 && placement_checker){
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'b';
      board[row_col_holder[2]][row_col_holder[12]] = 'b';
      board[row_col_holder[3]][row_col_holder[13]] = 'b';
      board[row_col_holder[4]][row_col_holder[14]] = 'b';
    }

    if (checker == 1)
    {
      //error message
      puts ("Your ship placements were either not vertical or horizontal or you are placing on top of another ship. Please try again");

      //pause
      system ("pause");
    }

    //clear screen
    system ("cls");

    //reset array
    for (counter = 0; counter < 50; counter++)
    {
      row_col_holder[counter] = 0;
    }
  } while (checker == 1);

  //place Cruiser
  checker = 1;
  do
  {
    //print board
    print_game_board (board, 10, 10);

    //place Cruiser
    printf("Please enter the three cells to place the Cruiser across.\n" 
          "Enter coordinates example [0,1], [0,2] = 0 1 0 2\nEnter coordinates: ");
    scanf (" %d %d %d %d %d %d", &row_col_holder[11], &row_col_holder[1], &row_col_holder[12], &row_col_holder[2], &row_col_holder[13]
      , &row_col_holder[3]);

    //add all x axis
    add_array_x = 0;
    add_array_x = row_col_holder[1] + row_col_holder[2] + row_col_holder[3];

    //do math on x axis to check if horizontal
    horziontal_x_check_with_math = 0;
    if (add_array_x >= 1)
    {
      horziontal_x_check_with_math = add_array_x / (3 * row_col_holder[3]);
    }

    //add all y axis
    add_array_y = 0;
    add_array_y = row_col_holder[11] + row_col_holder[12] + row_col_holder[13];

    //do math on y axis to check if vertical
    vertical_y_check_with_math = 0;
    if (add_array_y >= 1)
    {
      vertical_y_check_with_math = add_array_y / (3 * row_col_holder[13]);
    }

    //check placement
    placement_checker = 0;
    placement_checker = check_char_on_board (board, row_col_holder, 3);

    //horziontal checks
    if (add_array_x == 0 && placement_checker)
    {
      //is horizontal 
      checker = 0;
      //place ships  
      board[row_col_holder[1]][row_col_holder[11]] = 'r';
      board[row_col_holder[2]][row_col_holder[12]] = 'r';
      board[row_col_holder[3]][row_col_holder[13]] = 'r';
    } else if (horziontal_x_check_with_math == 1 && placement_checker) {
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'r';
      board[row_col_holder[2]][row_col_holder[12]] = 'r';
      board[row_col_holder[3]][row_col_holder[13]] = 'r';
    }

    //vertical check
    if (add_array_y == 0 && placement_checker)
    {
      //is vertical
      checker = 0;
      //place ship
      board[row_col_holder[1]][row_col_holder[11]] = 'r';
      board[row_col_holder[2]][row_col_holder[12]] = 'r';
      board[row_col_holder[3]][row_col_holder[13]] = 'r';
    } else if (vertical_y_check_with_math == 1 && placement_checker){
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'r';
      board[row_col_holder[2]][row_col_holder[12]] = 'r';
      board[row_col_holder[3]][row_col_holder[13]] = 'r';
    }

    if (checker == 1)
    {
      //error message
      puts ("Your ship placements were either not vertical or horizontal or you are placing on top of another ship. Please try again");

      //pause
      system ("pause");
    }

    //clear screen
    system ("cls");

    //reset array
    for (counter = 0; counter < 50; counter++)
    {
      row_col_holder[counter] = 0;
    }
  } while (checker == 1);

  //place Submarine
  checker = 1;
  do
  {
    //print board
    print_game_board (board, 10, 10);

    //place Submarine
    printf("Please enter the three cells to place the Submarine across.\n" 
          "Enter coordinates example [0,1], [0,2] = 0 1 0 2\nEnter coordinates: ");
    scanf (" %d %d %d %d %d %d", &row_col_holder[11], &row_col_holder[1], &row_col_holder[12], &row_col_holder[2], &row_col_holder[13]
      , &row_col_holder[3]);


    //add all x axis
    add_array_x = 0;
    add_array_x = row_col_holder[1] + row_col_holder[2] + row_col_holder[3];

    //do math on x axis to check if horizontal
    horziontal_x_check_with_math = 0;
    if (add_array_x >= 1)
    {
      horziontal_x_check_with_math = add_array_x / (3 * row_col_holder[2]);
    }

    //add all y axis
    add_array_y = 0;
    add_array_y = row_col_holder[11] + row_col_holder[12] + row_col_holder[13];

    //do math on y axis to check if vertical
    vertical_y_check_with_math = 0;
    if (add_array_y >= 1)
    {
      vertical_y_check_with_math = add_array_y / (3 * row_col_holder[12]);
    }

    //check placement
    placement_checker = 0;
    placement_checker = check_char_on_board (board, row_col_holder, 3);

    //horziontal checks
    if (add_array_x == 0 && placement_checker)
    {
      //is horizontal 
      checker = 0;
      //place ships  
      board[row_col_holder[1]][row_col_holder[11]] = 's';
      board[row_col_holder[2]][row_col_holder[12]] = 's';
      board[row_col_holder[3]][row_col_holder[13]] = 's';
    } else if (horziontal_x_check_with_math == 1 && placement_checker) {
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 's';
      board[row_col_holder[2]][row_col_holder[12]] = 's';
      board[row_col_holder[3]][row_col_holder[13]] = 's';
    }

    //vertical check
    if (add_array_y == 0 && placement_checker)
    {
      //is vertical
      checker = 0;
      //place ship
      board[row_col_holder[1]][row_col_holder[11]] = 's';
      board[row_col_holder[2]][row_col_holder[12]] = 's';
      board[row_col_holder[3]][row_col_holder[13]] = 's';
    } else if (vertical_y_check_with_math == 1 && placement_checker){
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 's';
      board[row_col_holder[2]][row_col_holder[12]] = 's';
      board[row_col_holder[3]][row_col_holder[13]] = 's';
    }

    if (checker == 1)
    {
      //error message
      puts ("Your ship placements were either not vertical or horizontal or you are placing on top of another ship. Please try again");

      //pause
      system ("pause");
    }

    //clear screen
    system ("cls");

    //reset array
    for (counter = 0; counter < 50; counter++)
    {
      row_col_holder[counter] = 0;
    }
  } while (checker == 1);

  //place Destroyer
  checker = 1;
  do
  {
    //print board
    print_game_board (board, 10, 10);

    //place Destroyer
    printf("Please enter the two cells to place the Destroyer across.\n" 
          "Enter coordinates example [0,1], [0,2] = 0 1 0 2\nEnter coordinates: ");
    scanf (" %d %d %d %d", &row_col_holder[11], &row_col_holder[1], &row_col_holder[12], &row_col_holder[2]);

    //add all x axis
    add_array_x = 0;
    add_array_x = row_col_holder[1] + row_col_holder[2];

    //do math on x axis to check if horizontal
    horziontal_x_check_with_math = 0;
    if (add_array_x >= 1)
    {
      horziontal_x_check_with_math = add_array_x / (2 * row_col_holder[2]);
    }

    //add all y axis
    add_array_y = 0;
    add_array_y = row_col_holder[11] + row_col_holder[12];

    //do math on y axis to check if vertical
    vertical_y_check_with_math = 0;
    if (add_array_y >= 1)
    {
      vertical_y_check_with_math = add_array_y / (2 * row_col_holder[12]);
    }

    //check placement
    placement_checker = 0;
    placement_checker = check_char_on_board (board, row_col_holder, 2);


    //horziontal checks
    if (add_array_x == 0 && placement_checker)
    {
      //is horizontal 
      checker = 0;
      //place ships  
      board[row_col_holder[1]][row_col_holder[11]] = 'd';
      board[row_col_holder[2]][row_col_holder[12]] = 'd';
    } else if (horziontal_x_check_with_math == 1 && placement_checker) {
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'd';
      board[row_col_holder[2]][row_col_holder[12]] = 'd';
    }

    //vertical check
    if (add_array_y == 0 && placement_checker)
    {
      //is vertical
      checker = 0;
      //place ship
      board[row_col_holder[1]][row_col_holder[11]] = 'd';
      board[row_col_holder[2]][row_col_holder[12]] = 'd';
    } else if (vertical_y_check_with_math == 1 && placement_checker){
      //is vertical
      checker = 0;
      //place ships
      board[row_col_holder[1]][row_col_holder[11]] = 'd';
      board[row_col_holder[2]][row_col_holder[12]] = 'd';
    }

    if (checker == 1)
    {
      //error message
      puts ("Your ship placements were either not vertical or horizontal or you are placing on top of another ship. Please try again");

      //pause
      system ("pause");
    }

    //clear screen
    system ("cls");

    //reset array
    for (counter = 0; counter < 50; counter++)
    {
      row_col_holder[counter] = 0;
    }
  } while (checker == 1);

}

/*************************************************************
* Function: check_char_on_board (array, coord_x, coords_y)   *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: check for char in the 2 D array               *
* Input parameters: array row col                            *
* Returns: char at that place                                *
* Preconditions: array must exist coords must be passed      *
* Postconditions: char can be used to make decission         *
*************************************************************/
char check_char_on_board_return_char (char board[][10], int coords_x, int coords_y)
{
  //local var
  char char_on_board = '\0';

  //do check
  char_on_board = board[coords_x][coords_y];

  //return char
  return char_on_board;
}

/*************************************************************
* Function: check_char_on_board (board, array_of_coords)     *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: check for char in the 2 D array               *
* Input parameters: array row col                            *
* Returns: int 0 1                                           *
* Preconditions: array must exist coords must be passed      *
* Postconditions: char can be used to make decission         *
*************************************************************/
int check_char_on_board (char board[][10], int array_of_coords[], int num_compare)
{
  //local var counter
  int counter = 0;
  char char_checker = '\0';

  //check around
  for (;counter < num_compare; counter++)
  {
    char_checker =  check_char_on_board_return_char ( board, array_of_coords[1 + counter], array_of_coords[11 + counter]);

    if ( char_checker == '~')
    {}else{
      //return 0 if not water
      return 0;
    }
  }
  
  //return 1 if full run through so can be placed
  return 1;
}

/*************************************************************
* Function: place_ships_auto (array)                         *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: place ships on the board auto                 *
* Input parameters: array                                    *
* Returns: none                                              *
* Preconditions: array must exist                            *
* Postconditions: ships will be on the board                 *
*************************************************************/
void place_ships_auto (char board[][10])
{
  //vertical = 0 hortizonal = 1
  //local var
  int row_start = 0, col_start = 0, direction = 0, counter = 0, checker = 0, placement_checker = 0;
  int array_of_coords[50] = {0};
  int array_of_coords_row[10] = {0};
  int array_of_coords_col[10] = {0};


  //Placing Carrier //0 vertial(col same) //1hortizontal(row same)
  direction = rand_true_or_false ();

  //gen starting points
  gen_start_pt (5, direction, &row_start, &col_start);

  //for carrier
  better_array_loader (array_of_coords_row, array_of_coords_col, row_start, col_start, direction, 5); //load array

  // //for carrier
  // load_array (array_of_coords, row_start, col_start, direction, 5); //load array

  // //place carrier
  // put_ship (board, array_of_coords, 5, 'c');

  //place carrier
  better_put_ship (board, array_of_coords_row, array_of_coords_col, 5, 'c');

  placement_checker = 0;
  while (placement_checker == 0)
  {
    //Placing battleship
    direction = rand_true_or_false ();

    gen_start_pt (4, direction, &row_start, &col_start);

    // load_array (array_of_coords, row_start, col_start, direction, 4);

    better_array_loader (array_of_coords_row, array_of_coords_col, row_start, col_start, direction, 4); //load array


    placement_checker = better_check_char_on_board (board, array_of_coords_row, array_of_coords_col, 4);

  }

  better_put_ship (board, array_of_coords_row, array_of_coords_col, 4, 'b');

  // put_ship (board, array_of_coords, 4, 'b');


  placement_checker = 0;
  while (placement_checker == 0)
  {
    //Placing battleship
    direction = rand_true_or_false ();

    gen_start_pt (3, direction, &row_start, &col_start);

    // load_array (array_of_coords, row_start, col_start, direction, 3);

    better_array_loader (array_of_coords_row, array_of_coords_col, row_start, col_start, direction, 3); //load array


    // placement_checker = check_char_on_board (board, array_of_coords, 3);
    placement_checker = better_check_char_on_board (board, array_of_coords_row, array_of_coords_col, 3);

  }
  
  // put_ship (board, array_of_coords, 3, 'r');

  better_put_ship (board, array_of_coords_row, array_of_coords_col, 3, 'r');


  placement_checker = 0;
  while (placement_checker == 0)
  {
    //Placing battleship
    direction = rand_true_or_false ();

    gen_start_pt (3, direction, &row_start, &col_start);

    // load_array (array_of_coords, row_start, col_start, direction, 3);

    better_array_loader (array_of_coords_row, array_of_coords_col, row_start, col_start, direction, 3); //load array


    // placement_checker = check_char_on_board (board, array_of_coords, 3);
    placement_checker = better_check_char_on_board (board, array_of_coords_row, array_of_coords_col, 3);

  }
  
  // put_ship (board, array_of_coords, 3, 's');

  better_put_ship (board, array_of_coords_row, array_of_coords_col, 3, 's');


  placement_checker = 0;
  while (placement_checker == 0)
  {
    //Placing battleship
    direction = rand_true_or_false ();

    gen_start_pt (2, direction, &row_start, &col_start);

    // load_array (array_of_coords, row_start, col_start, direction, 2);

    better_array_loader (array_of_coords_row, array_of_coords_col, row_start, col_start, direction, 2); //load array


    // placement_checker = check_char_on_board (board, array_of_coords, 2);
    placement_checker = better_check_char_on_board (board, array_of_coords_row, array_of_coords_col, 2);

  }
  
  // put_ship (board, array_of_coords, 2, 'd');

  better_put_ship (board, array_of_coords_row, array_of_coords_col, 2, 'd');


  
  /* You were going to place a battleship with a do while loop check with other
  program so you can place basically use past shit
  Basic thing you are trying to do
  Load array 1 is col 11 is row
  check array 
  if check pass 
  print battle ship on baord
  Things to do make a checker varriable
  use it for do while GOod night*/

}

/*************************************************************
* Function: gen_start_pt (array)                             *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: This is taken from class not mine             *
* Input parameters: len dir row_starter col_start            *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*This function is taken from PA 6 help by permission         *
*************************************************************/
void gen_start_pt (int length, int dir, int *row_start_ptr,
           int *col_start_ptr)
{
  // Note: the way we are generating starting points/coordinates
  // uses the length of the ship to restrict the random
  // values generated. This way we don't need extra logic
  // to determine if we are placing a ship outside
  // the bounds of our board/2-D array.
  if (dir == 0) // vertical
  {
    *col_start_ptr = rand () % 10;
    *row_start_ptr = rand () % (10 - length + 1); // down from the start point
  }
  else // horizontal
  {
    *col_start_ptr = rand () % (10 - length + 1);
    *row_start_ptr = rand () % 10;
  }
}

/*************************************************************
* Function: load_array (array, direction, size)              *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: loads an array with x and y                   *
* Input parameters: len dir row_starter col_start            *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*************************************************************/
void load_array (int array_of_coords[], int row_start, int col_start, int direction, int size)
{
  //local var
  int counter = 0;

  //reset array
  for (counter = 0; counter < 50; counter++)
  {
    array_of_coords[counter] = 0;
  }


  //load array according to direction
  if (direction)
    {
      for (counter = 0; counter < size; counter++)
      {
        //generate row
        array_of_coords[1 + counter] = row_start ;
        array_of_coords[11 + counter] = col_start + counter;
      }
    }
    else
    {
      for (counter = 0; counter < size; counter++)
      {
        //generate row
        array_of_coords[1 + counter] = row_start + counter;
        array_of_coords[11 + counter] = col_start;
      }
    }
}

/*************************************************************
* Function: put_ship (board, array, size, ship)              *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: place ships                                   *
* Input parameters:                                          *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*************************************************************/
void put_ship (char board[][10], int array_of_coords[], int size, char ship)
{
  //local var
  int counter = 0;

  //place ship
  for (counter = 0; counter < size; counter++)
  {
    board[array_of_coords[11 + counter]][array_of_coords[1 + counter]] = ship;
  }
}

/*************************************************************
* Function: computer_shoots (*row, *col)                     *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: place ships                                   *
* Input parameters:                                          *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*************************************************************/
void computer_shoots (int *row, int *col)
{
  *row = rand () % 10;
  *col = rand () % 10;
}

/*************************************************************
* Function: is_miss (row, col)                               *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: place ships                                   *
* Input parameters:                                          *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*************************************************************/
int is_hit_or_miss (char board[][10], int row, int col)
{
  //local var
  if (board[col][row] == '~')
  {
    return 0;
  }else{
    return 1;
  }
}
/*************************************************************
* Function: is_winner (hit, hit)                             *
* Date Created: 11/01/15                                     *
* Date Last Modified: 11/01/15                               *
* Description: place ships                                   *
* Input parameters:                                          *
* Returns: none                                              *
* Preconditions: all inputs exist                            *
* Postconditions: generate startign position                 *
*************************************************************/
int is_winner (int player_hits, int computer_hits)
{
  if ((player_hits == 17)||(computer_hits == 17))
  {
    return 1;
  } 
  
  return 0;
  
}

void better_array_loader (int row[], int col[], int row_start,
                            int col_start, int direction , int ship)
{
  //local var
  int counter = 0;

  //load both arrays
  for (counter = 0; counter < ship; counter++)
  {
    if (direction)
    {
      row[counter] = row_start;
      col[counter] = col_start + counter;
    }else{
      row[counter] = row_start + counter;
      col[counter] = col_start;
    }
  }
}

void better_put_ship (char board[][10], int row[], int col[], int size, char ship)
{
  //local var
  int counter = 0;

  //place ship
  for (counter = 0; counter < size; counter++)
  {
    board[row[counter]][col[counter]] = ship;
  }
}

char beter_check_char_on_board_return_char (char board[][10], int row, int col)
{
  //local var
  char char_on_board = '\0';

  //do check
  char_on_board = board[row][col];

  //return char
  return char_on_board;
}
int better_check_char_on_board (char board[][10], int row[], int col[], int ship_size)
{
  //local var counter
  int counter = 0;
  char char_checker = '\0';

  //check around
  for (;counter < ship_size; counter++)
  {
    char_checker =  beter_check_char_on_board_return_char ( board, row[counter], col[counter]);

    if ( char_checker == '~')
    {}else{
      //return 0 if not water
      return 0;
    }
  }
  
  //return 1 if full run through so can be placed
  return 1;
}
