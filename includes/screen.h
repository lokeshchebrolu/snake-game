#ifndef __SCREEN_H
#define __SCREEN_H
#include"../includes/types.h"

/* Symbols */
#define MAX_X 50.0f
#define MAX_Y 50.0f

#define BOUNDARY_X (MAX_X-3)
#define BOUNDARY_Y (MAX_Y-3)

#define WINDOW_POS_X 0
#define WINDOW_POS_Y 0

#define WINDOW_SIZE_X 400
#define WINDOW_SIZE_Y 400

#define BLACK 0,0,0
#define WHITE	1.0f,1.0f,1.0f

#define REFRESH_TIME (1000/60)



/* Variables */
extern int window_id;
extern block game_boundary;

/* Global function declarations  */
extern void screen_init(int *argc,char *argv[]);
extern void display(void);
extern void reshape(int width,int height);
extern void timer(int arg);
extern void specialInput(int key,int x,int y);
#endif
