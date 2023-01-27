#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PLAYERS 10 // the greatest number of players allowed
#define INIT_BALANCE 3 // the initial balance of each player
#define NUM_FACES 6 // the number of faces for a die

// Define global variables for the game, accessed by both functions main() and rollDie().
uint32_t balance[NUM_PLAYERS]; // balance of each-player
uint32_t pot = 0; // balance of the-pot-in-the-center
const char *names[]
    = { "Happy", "Sleepy", "Sneezy", "Dopey", "Bashful", "Grumpy", "Doc",
        "Mirror Mirror", "Snow White", "Wicked Queen" }; // names of all players

//
// Returns the position of the player to the left.
//
// pos:     The postion of the current player.
// players: The number of players in the game.
//
uint32_t left(uint32_t pos, uint32_t players) {
  return ((pos + players - 1) % players);
}

//
// Returns the position of the player to the right.
//
// pos: The position of the current player.
// players: The number of players in the game.
//
uint32_t right(uint32_t pos, uint32_t players) {
  return ((pos + 1) % players);
}

//
// Update the balance of current-player and her neighbors, and the balance of the-pot-in-the-center after rolling a die.
//
// pos: The position of the current player.
// players: The number of players in the game.
// roll_num: The random number between [0, 5].
//
void rollDie(uint32_t pos, uint32_t players, uint32_t roll_num) {
  typedef enum faciem {
    LEFT,
    RIGHT,
    CENTER,
    PASS
  } faces; // faces is short for enum(type) faciem(identifier, name of the set)
  faces die[]
      = { LEFT, RIGHT, CENTER, PASS, PASS, PASS }; // a die has six faces

  // update the balance of current player and her neighbors, and the balance of pot, according to the roll_num
  if (die[roll_num]
      == LEFT) // if the player rolls L then she gives $1 to the player on her left
  {
    printf("gives $1 to %s", names[left(pos, players)]);
    balance[pos] = balance[pos] - 1;
    balance[left(pos, players)] = balance[left(pos, players)] + 1;
  } else if (
      die[roll_num]
      == RIGHT) // if the player rolls R then she gives $1 to the player on her right
  {
    printf("gives $1 to %s", names[right(pos, players)]);
    balance[pos] = balance[pos] - 1;
    balance[right(pos, players)] = balance[right(pos, players)] + 1;
  } else if (
      die[roll_num]
      == CENTER) // if the player rolls C then she puts $1 in the pot in the center
  {
    printf("puts $1 in the pot");
    balance[pos] = balance[pos] - 1;
    pot++;
  } else if (die[roll_num] == PASS) // if the player rolls ¡¤then she ignores it
  {
    printf("gets a pass");
  } // end if
}

//
// return 0 if finish as expected.
//
int main(void) {
  // define variables
  uint32_t num_players; //number of players
  uint32_t ran_seed; // seed of psudo random number
  uint32_t position = 0; // current position, initial position is 0

  // Display RAND_MAX on this platform, it influences the psudo random number series.
  //printf("RAND_MAX: %d\n", RAND_MAX);

  // read in the random seed from the user, then set with srand()
  printf("Random seed: ");
  scanf("%u", &ran_seed);
  srand(ran_seed); // set random seed

  // read in the number of players from the user
  printf("How many players? ");
  scanf("%u", &num_players); // read the number of the players from user
 
  // initialize balance array
  for (uint32_t i = 0; i < num_players; i++) {
    balance[i] = INIT_BALANCE;
  }

  // play the game
  int cur_num_players
      = num_players; // current number of players, initial value is num_players
  while (cur_num_players > 1) // keep running until there is only one player
  {
    // decide what to do according to the balance of current player
    if (balance[position] == 0) // if the player has no money, just pass
    {

    } else if (balance[position]
               == 1) // if the player has 1 dollar, roll the die once
    {
      printf("%s rolls... ", names[position]);
      rollDie(position, num_players, rand() % NUM_FACES);
      printf("\n"); // current player finish his turn for the round
    } else if (balance[position]
               == 2) // if the player has 2 dorllars, roll the die twice
    {
      printf("%s rolls... ", names[position]);
      rollDie(position, num_players, rand() % NUM_FACES);
      printf(" ");
      rollDie(position, num_players, rand() % NUM_FACES);
      printf("\n"); // current player finish his turn for the round
    } else // if the player has at least 3 dorllars, roll the die third times
    {
      printf("%s rolls... ", names[position]);
      rollDie(position, num_players, rand() % NUM_FACES);
      printf(" ");
      rollDie(position, num_players, rand() % NUM_FACES);
      printf(" ");
      rollDie(position, num_players, rand() % NUM_FACES);
      printf("\n"); // current player finish his turn for the round
    } // end if

    // update current position
    position = right(position, num_players); // move to next player on the right

    // calculate how many active players there are now
    cur_num_players
        = 0; // set current number of active players to 0, to be ready for accumulating
    for (uint32_t i = 0; i < num_players;
         i++) // check how many player's balance is greater than 0
    {
      if (balance
              [i]) // if player i's balance is greater than 0, then increase the current-number-of-players
      {
        cur_num_players++;
      }
    } // end for-loop
  } // end while-loop

  //output the result of the game
  for (uint32_t i = 0; i < num_players; i++) // check each player's balance
  {
    if (balance[i]) // only the winner's balance is greater than 0
    {
      printf("%s wins the $%d pot with $%d left in the bank!\n", names[i], pot,
          balance[i]); // display winner's name, balance of pot & winner
      break; // quit for-loop if find the winner
    }
  }

  // finish as expected
  return 0;
}
