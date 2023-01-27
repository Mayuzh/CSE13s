#include "stack.h"
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTIONS "srn:"
void RecursionMode(int, char, char, char);
int counter;
void StackMode(int);
void move(Stack *, Stack *);

void StackMode(int n) {
  long Maximum = pow(2, n) - 1;
  int counter = 0;
  Stack *A;
  Stack *B;
  Stack *C;
  A = stack_create(n, 'A');
  for (int i = n; i > 0; i--) {
    stack_push(A, i);
  }
  if (n % 2 == 0) {
    B = stack_create(n, 'C');
    C = stack_create(n, 'B');
  } else {
    B = stack_create(n, 'B');
    C = stack_create(n, 'C');
  }
  while (counter < Maximum) {
    if (stack_peek(A) == 1) {
      move(A, B);
    } else if (stack_peek(B) == 1) {
      move(B, C);
    } else if (stack_peek(C) == 1) {
      move(C, A);
    }
    counter++;
    if (counter < Maximum) {
      if (stack_empty(A)) {
        if (stack_peek(B) != 1) {
          move(B, A);
        } else if (stack_peek(C) != 1) {
          move(C, A);
        }
      } else if (stack_empty(B)) {
        if (stack_peek(C) != 1) {
          move(C, B);
        } else if (stack_peek(A) != 1) {
          move(A, B);
        }
      } else if (stack_empty(C)) {
        if (stack_peek(B) != 1) {
          move(B, C);
        } else if (stack_peek(A) != 1) {
          move(A, C);
        }
      } else if (stack_peek(A) == 1) {
        if (stack_peek(B) < stack_peek(C)) {
          move(B, C);
        } else {
          move(C, B);
        }
      } else if (stack_peek(B) == 1) {
        if (stack_peek(A) < stack_peek(C)) {
          move(A, C);
        } else {
          move(C, A);
        }
      } else {
        if (stack_peek(A) < stack_peek(B)) {
          move(A, B);
        } else {
          move(B, A);
        }
      }

      counter++;

    } // end if

  } // end while
  stack_delete(A);
  stack_delete(B);
  stack_delete(C);
  printf("\nNumber of moves: %d\n", counter);
  return;
}
void move(Stack *l, Stack *r) {
  int item = stack_pop(l);
  stack_push(r, item);
  printf("Move disk %d from peg %c to peg %c\n", item, l->name, r->name);
}
void RecursionMode(int n, char l, char m, char r) {
  if (n == 1) {
    printf("Move disk %d from peg %c to peg %c\n", n, l, m);
    counter++;
  } else {
    RecursionMode(n - 1, l, r, m);
    printf("Move disk %d from peg %c to peg %c\n", n, l, m);
    counter++;
    RecursionMode(n - 1, r, m, l);
  }
}
int main(int argc, char **argv) {
  if (argc
      == 1) // argc[0] is the executable filename, at least argc[1] is not NULL.
  {
    puts("Error: no arguments supplied!");
    return -1;
  }
  int c; // return value of getopt()
  int number = 5;
  bool isStack;
  bool isRecursion;
  while ((c = getopt(argc, argv, OPTIONS))
         != -1) // if there are more than one option
  {
    switch (c) {
    case 'n':
      number = atoi(optarg);
      break;
    case 's':
      isStack = true;
      break;
    case 'r':
      isRecursion = true;
      break;
    }
  }
  if (isStack) {
    printf("================================\n");
    printf("----------   STACKS   ----------\n");
    printf("================================\n");
    StackMode(number);
  }
  if (isStack && isRecursion) {
    printf("\n");
  }
  if (isRecursion) {
    printf("================================\n");
    printf("--------   RECURSION   ---------\n");
    printf("================================\n");
    RecursionMode(number, 'A', 'B', 'C');
    printf("\nNumber of moves: %d\n", counter);
  }
  //if(isStack && isRecursion)

  return 0;
}
