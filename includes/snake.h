#ifndef __SNAKE_H
#define __SNAKE_H

/* Header file inclusion */
#include"../includes/types.h"

/* Symbols */
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define END -1


/* Variables */
extern float snake_speed;
extern unsigned int snake_length;

extern snake_head head;
extern snake_body *body;

extern obstacle egg;
extern float egg_size;
extern int egg_hit;

extern int dead;

/* Global function declarations */
extern void snake_init(void);
#endif

