/****** Header file inclusion ******/
#include"../includes/snake.h"
#include"../includes/screen.h"
#include<stdio.h>
#include<stdlib.h>

/****** Local symbols ******/
#define SNAKE_SPEED_DEFAULT (0.25)
#define SNAKE_LENGTH_DEFAULT (1)

/* 1st quadrant co-ordinates of EGG in 2D Plan */
#define EGG_X_DEFAULT (1.0)
#define EGG_Y_DEFAULT (1.0)
/* Egg initial position of origin */
#define EGG_POS_X_DEFAULT (10.0)
#define EGG_POS_Y_DEFAULT (10.0)

/* Head symbols */
/* Eye point positions */
#define HEAD_EYE_X (0.5)
#define HEAD_EYE_Y (0.5)

/* 1st quadrant co-ordinates of NOSE in 2D Plan */
#define HEAD_NOSE_X (1.0)
#define HEAD_NOSE_Y (1.0)

/* 1st quadrant co-ordinates of SKULL in 2D Plan */
#define HEAD_SKULL_X (0.0)
#define HEAD_SKULL_Y (1.5)

/* Head default direction */
#define HEAD_DIRECTION_DEFAULT RIGHT


/* Body symbols */
/* 1st quadrant co-ordinates of body in 2D Plan */
#define BODY_X (3.0)
#define BODY_Y (1.5)

#define YES (1)
#define NO (0)

/****** Local function declarations ******/
/* Egg data initialization */
static void egg_init(void);

/****** Global Variable definition ******/
/* Speed of snake in no.of co-ordinates it
   will be added for next position */
float snake_speed;
/* Snake body block count excluding head */
unsigned int snake_length;

/* Snake head variable */
snake_head head;
/* Snake body pointer 
   A linkedlist shall be used to implement body blocks*/
snake_body *body = NULL;

/* Egg variable */
obstacle egg;
/* Egg size variable */
float egg_size;
/* Egg hit status variable 
   1 : Hit 
   0 : not hit*/
int egg_hit;

/* Snake dead status 
   1 : dead
   0 : alive*/
int dead;

/*** Global function definitions ***/
/* Snake data initialization function */
void snake_init(void)
{
	/* Initialize egg data */
	egg_init();
	
	/* Set default snake speed and legnth */
	snake_speed = SNAKE_SPEED_DEFAULT;
	snake_length = SNAKE_LENGTH_DEFAULT;
	
	/* Initialize co-ordinates of head */
	head.eye_size=2.0;
	head.eye[0].x=0.5;
	head.eye[0].y=0.5;
	head.eye[1].x=0.5;
	head.eye[1].y=-0.5;

	head.nose.point[0].x = HEAD_NOSE_X;
	head.nose.point[0].y = HEAD_NOSE_Y;
	head.nose.point[1].x = 0.0;
	head.nose.point[1].y = HEAD_NOSE_Y;
	head.nose.point[2].x = 0.0;
	head.nose.point[2].y = -HEAD_NOSE_Y;
	head.nose.point[3].x = HEAD_NOSE_X;
	head.nose.point[3].y = -HEAD_NOSE_Y;

	head.skull.point[0].x = HEAD_SKULL_X;
	head.skull.point[0].y = HEAD_SKULL_Y;
	head.skull.point[1].x = HEAD_SKULL_X-1;
	head.skull.point[1].y = HEAD_SKULL_Y;
	head.skull.point[2].x = HEAD_SKULL_X-1;
	head.skull.point[2].y = -HEAD_SKULL_Y;
	head.skull.point[3].x = HEAD_SKULL_X;
	head.skull.point[3].y = -HEAD_SKULL_Y;
	
	/* Initialize co-ordinates of body with default length */
	body = (snake_body*)realloc(body,snake_length*sizeof(snake_body));
	if(!body)
	{
		perror("snake_init(): realloc() error:");
		exit(1);
	}

	body[0].number = 0;
	body[0].direction = RIGHT;
	body[0].rotate = NO;
	body[0].up.point[0].x = BODY_X;
	body[0].up.point[0].y = BODY_Y;
	body[0].up.point[1].x = -BODY_X;
	body[0].up.point[1].y = BODY_Y;
	body[0].up.point[2].x = -BODY_X;
	body[0].up.point[2].y = -BODY_Y;
	body[0].up.point[3].x = BODY_X;
	body[0].up.point[3].y = -BODY_Y;

	body[0].down.point[0].x = 0.0;
	body[0].down.point[0].y = 0.0;
	body[0].down.point[1].x = -(BODY_X*2.0);
	body[0].down.point[1].y = 0.0;
	body[0].down.point[2].x = -(BODY_X*2.0);
	body[0].down.point[2].y = -(BODY_Y*2.0);
	body[0].down.point[3].x = 0.0;
	body[0].down.point[3].y = -(BODY_Y*2.0);
	
	/* Set snake as alive */
	dead = NO;

}

/* Egg data initialization */
static void egg_init(void)
{
	/* Initialize co-ordinates of egg */
	egg.obst.point[0].x = EGG_X_DEFAULT;
	egg.obst.point[0].y = EGG_Y_DEFAULT;
	egg.obst.point[1].x = -EGG_X_DEFAULT;
	egg.obst.point[1].y = EGG_Y_DEFAULT;
	egg.obst.point[2].x = -EGG_X_DEFAULT;
	egg.obst.point[2].y = -EGG_Y_DEFAULT;
	egg.obst.point[3].x = EGG_X_DEFAULT;
	egg.obst.point[3].y = -EGG_Y_DEFAULT;

	egg.position.x = EGG_POS_X_DEFAULT;
	egg.position.y = EGG_POS_Y_DEFAULT;
	
	egg_size = 4.0;
	/* Set egg as not HIT */
	egg_hit = NO;
}
