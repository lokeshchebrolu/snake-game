#ifndef __SCREEN_H
#define __SCREEN_H
/* File Description: Game screen values, function declarations*/

/* Game types file */
#include"../includes/types.h"

/*** Symbols ***/
/* 2D grid plane size */
/* --------------------------------- */
/*|       (0,MAX_Y)|                |*/
/*|                |                |*/
/*|                |                |*/
/*|                |                |*/
/*|                |                |*/
/*|                |                |*/
/*|--------------(0,0)--------------|*/
/*|(-MAX_X,0)      |       (MAX_X,0)|*/
/*|                |                |*/
/*|                |                |*/
/*|                |                |*/
/*|                |                |*/
/*|      (0,-MAX_Y)|                |*/
/* --------------------------------- */
#define MAX_X (50.0f)	/* X-axix maximum value */
#define MAX_Y (50.0f) /* Y-axix maximum value */

/* Quadrants */
#define FIRST_QUADRANT	(1)
#define SECOND_QUADRANT	(2)
#define THIRD_QUADRANT	(3)
#define FOURTH_QUADRANT	(4)


/* Game outer boundary values inside 2D grid plane*/
/* _________________________________ */
/*|           SCORE_BOARD           |*/
/*|_________________________________|*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|          GAME_BOUNDARY          |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|                                 |*/
/*|_________________________________|*/

#define BOUNDARY_X (MAX_X-1) /* X-axix Boundary value */
#define BOUNDARY_Y (MAX_Y-6) /* Y-axix Boundary value */

#define SCORE_BOARD_BOUNDARY_X (MAX_X-1)
#define SCORE_BOARD_BOUNDARY_Y (MAX_Y-1)
#define SCORE_BOARD_HEIGHT (4)

/* Window Initial position co-ordinates */
#define WINDOW_POS_X (0)
#define WINDOW_POS_Y (0)

/* Window SIZE in Pixels */
#define WINDOW_SIZE_X (600)
#define WINDOW_SIZE_Y (600)

/* Macros for colours in R,G,B values */
#define BLACK 0,0,0
#define WHITE	1.0f,1.0f,1.0f

/* Frame Refresh time */
/* Set to 60Fps = 1000ms/60 */
#define REFRESH_TIME (1000/60)


/*** Variables ***/
/* Variable to hold the created window id value
   This is used for closing window */
extern int window_id;

/* Game outer boundary vertices variable */
extern block game_boundary;
extern block score_board_boundary;

/*** Global function declarations  ***/
/* Function for screen data initialization */
extern void screen_init(int *argc,char *argv[]);

/* OpenGL default display function which is used for drawing each frame */
extern void display(void);

/* OpenGL default function used when window is resized.
   Windows all values will be recalibrated after this function called */
extern void reshape(int width,int height);

/* OpenGL default timer function as per REFRESH_TIME */
extern void timer(int arg);

/* OpenGL default Input capture function Used to read KEY press from keyboard*/
extern void specialInput(int key,int x,int y);
#endif
