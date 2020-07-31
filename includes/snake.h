#ifndef __SNAKE_H
#define __SNAKE_H
/* File Description: snake variables,values,function declarations*/

/*** Header file inclusion ***/
/* Game types file */
#include"../includes/types.h"

/*** Symbols ***/
/* Snake movement directions */
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define END 5

#define SNAKE_SPEED_DEFAULT (0.25)
#define SNAKE_LENGTH_DEFAULT (1)

/*** Variables ***/
/* Speed of snake in no.of co-ordinates it
   will be added for next position */
extern float snake_speed;

/* Snake body block count excluding head */
extern unsigned int snake_length;

/* Snake head variable */
extern snake_head head;

/* Snake body pointer 
   A linkedlist shall be used to implement body blocks*/
extern snake_body *body;

/* Egg variable */
extern obstacle egg;

/* Egg size variable */
extern float egg_size;

/* Egg hit status variable */
extern int egg_hit;

/* Snake dead status */
extern int dead;

/*** Global function declarations ***/
/* Snake data initialization function */
extern void snake_init(void);
#endif

