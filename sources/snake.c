/*** Header file inclusion ***/
#include"../includes/snake.h"
#include"../includes/screen.h"
#include<stdio.h>
#include<stdlib.h>

/*** Local symbols ***/
#define SNAKE_SPEED_DEFAULT (0.2)
#define SNAKE_LENGTH_DEFAULT (1)

/* Egg symbols */
#define EGG_X_DEFAULT (0.8)
#define EGG_Y_DEFAULT (0.8)
#define EGG_POS_X_DEFAULT (20.0)
#define EGG_POS_Y_DEFAULT (20.0)

/* Head symbols */
#define HEAD_EYE_X (0.5)
#define HEAD_EYE_Y (0.5)

#define HEAD_NOSE_X (0.0)
#define HEAD_NOSE_Y (1.0)

#define HEAD_SKULL_X (0.0)
#define HEAD_SKULL_Y (1.5)

#define HEAD_DIRECTION_DEFAULT RIGHT


/* Body symbols */
#define BODY_X (3.0)
#define BODY_Y (1.5)

#define YES (1)
#define NO (0)

/*** Local function declarations ***/
static void egg_init(void);

/* Global Variable definition */
float snake_speed;
unsigned int snake_length;

snake_head head;
snake_body *body = NULL;

obstacle egg;
float egg_size;
int egg_hit;

int dead;

void snake_init(void)
{
	egg_init();

	snake_speed = SNAKE_SPEED_DEFAULT;
	snake_length = SNAKE_LENGTH_DEFAULT;

	head.eye_size=2.0;
	head.eye[0].x=0.5;
	head.eye[0].y=0.5;
	head.eye[1].x=0.5;
	head.eye[1].y=-0.5;

	head.nose.point[0].x = HEAD_NOSE_X;
	head.nose.point[0].y = HEAD_NOSE_Y;
	head.nose.point[1].x = HEAD_NOSE_X;
	head.nose.point[1].y = -HEAD_NOSE_Y;
	head.nose.point[2].x = HEAD_NOSE_X+1;
	head.nose.point[2].y = -HEAD_NOSE_Y;
	head.nose.point[3].x = HEAD_NOSE_X+1;
	head.nose.point[3].y = HEAD_NOSE_Y;

	head.skull.point[0].x = HEAD_SKULL_X;
	head.skull.point[0].y = HEAD_SKULL_Y;
	head.skull.point[1].x = HEAD_SKULL_X-1;
	head.skull.point[1].y = HEAD_SKULL_Y;
	head.skull.point[2].x = HEAD_SKULL_X-1;
	head.skull.point[2].y = -HEAD_SKULL_Y;
	head.skull.point[3].x = HEAD_SKULL_X;
	head.skull.point[3].y = -HEAD_SKULL_Y;

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


	dead = NO;

}

static void egg_init(void)
{
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
	egg_hit = NO;
}
