/******  Header file inclusion ******/
#include"../includes/screen.h"
#include"../includes/snake.h"
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>

/* Local symbols */
#define SCREEN_MATRIX_X (((int)(MAX_X/SNAKE_SPEED_DEFAULT)*2)+1)
#define SCREEN_MATRIX_Y (((int)(MAX_Y/SNAKE_SPEED_DEFAULT)*2)+1)

#define CURRENT_HEAD_X (head.position.x)
#define CURRENT_HEAD_Y (head.position.y)

/* Local types */
typedef struct screen_matrix_cell
{
	unsigned int dir:3;
	float angle;
}screen_matrix_cell;

/****** Local variables ******/
/* Current movement direction */
static int move_dir=RIGHT;
static screen_matrix_cell screen_matrix[SCREEN_MATRIX_X][SCREEN_MATRIX_Y];

/* Current rotation angle to be done */
static float snake_rot_angle=0.0;


/****** Local function declaration ******/
/* Initialize boundary data */
void boundary_init(void);
/* Draws boundary on screen */
void boundary(void);
/* Draws next egg on screen */
void next_egg(void);
/* Draws a given block on screen */
void block_print(block shape);
/* Draws whole snake on screen */
void snake_print(void);
/* Draws string on screen */
void output_string(int x,int y,char *string,float color[3]);
/* Handles game when snake is dead */
void dead_fun(void);
/* Updates snake hit status
	 1. Egg hit : 1 - hit;0 - Not hit
	 2. dead : 1 - hit boundary;0 - inside boundary   */
void update_hit_status(void);
/* Updated new position of egg */
void new_egg(void);
/* Quadrant type of vertex */
int quadrant_of_vertex(vertex ver);
/* Make a number positive */
float make_positive(float val);
/* Make a number negative */
float make_negative(float val);
/* Generates two randome values less than given modulo_val */
void rand_val(int *rand,int modulo_val);
/* screen_matrix initialisation */
void screen_matrix_init(void);
/* screen_matrix update */
void screen_matrix_update(float x,float y,int dir,float angle);
/* screen_matrix angle at co-ordinates */
float screen_matrix_angle(float x,float y);
/* screen_matrix direction at co-ordinates */
int screen_matrix_dir(float x,float y);
/* Update next transition position of head */	
void head_next_pos(void);
/* Update next transition position of body */	
void body_next_pos(int i);
/* Add one body part */
void extend_body(void);

/****** Global variable definitions ******/
/* Variable to hold the created window id value
	 This is used for closing window */
int window_id;
/* Game boundary vertices variable */
block game_boundary;
/* Score board boundary vertices variable */
block score_board_boundary;

/****** Global function definitions ******/
/* Function for screen data initialization */
void screen_init(int *argc,char *argv[])
{
	/* OpenGL Init functions */
	glutInit(argc,argv);

	/* Screen display mode set to RGB colour and double buffer mode 
		 Double buffer mode is used for animations */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	/* Set Window Initial position */
	glutInitWindowPosition(WINDOW_POS_X,WINDOW_POS_Y);

	/* Set Window size in pixels */
	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);

	/* Create window */
	window_id=glutCreateWindow("Snake Game");

	/* Set background colour to black */
	glClearColor(0,0,0,1);

	/* Set display function pointer */
	glutDisplayFunc(display);

	/* Set reshape function pointer */
	glutReshapeFunc(reshape);

	/* Set timer function pointer 
0 : refresh time set to 0 initially
timer : timer function pointer
0 : argument to timer function */
	glutTimerFunc(0,timer,0);

	/* Set specialfunction function pointer */
	glutSpecialFunc(specialInput);

	/* Initialize bounday data */
	boundary_init();

}

/* OpenGL default display function which is used for drawing each frame */
void display(void)
{
	/* Clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Reset matrix to identity matrix */
	glLoadIdentity();

	/* if snake is not dead */
	if(!dead)
	{
		/* if current egg is not hit */
		if(!egg_hit)
		{
			/* Draw egg */
			next_egg();
		}
		else
		{
			egg_hit=0;
			snake_length++;
			extend_body();
			new_egg();
		}

		/* Draw boundary */
		boundary();

		/* Draw snake */
		snake_print();

	}
	/* if snake is dead */
	else
	{
		/* handle dead snake by calling dead handle function */
		glPushMatrix();
		dead_fun();
		glPopMatrix();
	}

	/* Displays frame buffer on screen */
	glFlush();

	/* Swap double buffers back and forth */
	glutSwapBuffers();
	update_hit_status();
}

/* OpenGL default function used when window is resized.
	 Windows all values will be recalibrated after this function called */
void reshape(int width,int height)
{
	/* Set viewport to new width,height of window */
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	/* Change matrix to projection */
	glMatrixMode(GL_PROJECTION);
	/* Reset matrix to identity matrix */
	glLoadIdentity();
	/* Set 2D Plane vertices */
	gluOrtho2D(-MAX_X,MAX_Y,-MAX_X,MAX_Y);
	/* Switch back to modelview matrix */
	glMatrixMode(GL_MODELVIEW);
}

/* OpenGL default timer function as per REFRESH_TIME */
void timer(int arg)
{
	/* Sets display flag to call display function in next glutmainloop */
	glutPostRedisplay();

	/* Set timer function to itself with 60FPS so it calls itself every 1/60th of second */
	glutTimerFunc(1000/60,timer,0);

}

void timer_end(int arg)
{
	;
}

/* OpenGL default Input capture function Used to read KEY press from keyboard*/
void specialInput(int key,int x,int y)
{
	if(dead) /* Snake Dead */
	{
		if(!(key^GLUT_KEY_END))
			glutDestroyWindow(window_id);
		return;
	}

	/* Update new direction based on key pressed by user */
	switch(key)
	{
		case GLUT_KEY_UP: /* UP arrow key */
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
				{
					snake_rot_angle += -90.0;
				}
				else if(!(move_dir^RIGHT))
				{
					snake_rot_angle += 90.0;
				}
				move_dir=UP;	
			}
			break;

		case GLUT_KEY_DOWN: /* DOWN arrow key */
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
				{
					snake_rot_angle += 90.0;
				}
				else if(!(move_dir^RIGHT))
				{
					snake_rot_angle += -90.0;
				}
				move_dir=DOWN;
			}
			break;

		case GLUT_KEY_LEFT: /* LEFT arrow key */
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
				{
					snake_rot_angle += 90.0;
				}
				else if(!(move_dir^DOWN))
				{
					snake_rot_angle += -90.0;
				}
				move_dir=LEFT;
			}
			break;

		case GLUT_KEY_RIGHT: /* RIGHT arrow key */
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
				{
					snake_rot_angle += -90.0;
				}
				else if(!(move_dir^DOWN))
				{
					snake_rot_angle += 90.0;
				}
				move_dir=RIGHT;
			}
			break;

		/* If END is key by USER pressed then close window */
		case GLUT_KEY_END: /* END key */
			move_dir=END;
			glutDestroyWindow(window_id);
			break;
	}


}

/****** Local function definitions ******/
/* Initialize boundary data */
void boundary_init(void)
{
	/* Empty square formed with 4 lines */
	/* Initialize boundary vertices*/
	game_boundary.point[0].x=BOUNDARY_X;
	game_boundary.point[0].y=BOUNDARY_Y;

	game_boundary.point[1].x=-BOUNDARY_X;
	game_boundary.point[1].y=BOUNDARY_Y;

	game_boundary.point[2].x=-BOUNDARY_X;
	game_boundary.point[2].y=-(BOUNDARY_Y+SCORE_BOARD_HEIGHT);

	game_boundary.point[3].x=BOUNDARY_X;
	game_boundary.point[3].y=-(BOUNDARY_Y+SCORE_BOARD_HEIGHT);

	score_board_boundary.point[0].x = SCORE_BOARD_BOUNDARY_X;
	score_board_boundary.point[0].y = SCORE_BOARD_BOUNDARY_Y;

	score_board_boundary.point[1].x = -SCORE_BOARD_BOUNDARY_X;
	score_board_boundary.point[1].y = SCORE_BOARD_BOUNDARY_Y;

	score_board_boundary.point[2].x = -SCORE_BOARD_BOUNDARY_X;
	score_board_boundary.point[2].y = SCORE_BOARD_BOUNDARY_Y-SCORE_BOARD_HEIGHT;

	score_board_boundary.point[3].x = SCORE_BOARD_BOUNDARY_X;
	score_board_boundary.point[3].y = SCORE_BOARD_BOUNDARY_Y-SCORE_BOARD_HEIGHT;

}

/* Draws a given block on screen */
void block_print(block shape)
{
	/* Set draw type to polysgon */
	glBegin(GL_POLYGON);
	for(int i=0;i<4;i++) /* Loop through 4 vertices and draw */
		glVertex2f(shape.point[i].x,shape.point[i].y);
	/* End drawing */
	glEnd();
}

/* Draws boundary on screen */
void boundary(void)
{
	/* draw SCORE_BOARD BOUNDARY */
	glPushMatrix();	
	glBegin(GL_LINE_LOOP);
	block_print(score_board_boundary);
	glEnd();
	glPopMatrix();

	/* draw GAME_BOUNDARY */
	glPushMatrix();	
	glBegin(GL_LINE_LOOP);
	block_print(game_boundary);
	glEnd();
	glPopMatrix();

	char score_string[12];
	sprintf(score_string,"SCORE : %d",snake_length);
	float color[3]={WHITE};
	output_string(MAX_X-15,MAX_Y-3,score_string,color);
}

/* Draws next egg on screen */
void next_egg(void)
{
	glPushMatrix();
	/* Move egg to it's position */
	glTranslatef(egg.position.x,egg.position.y,0);
	glBegin(GL_POLYGON);
	/* Draw egg */
	block_print(egg.obst);
	glEnd();
	glPopMatrix();
}

/* Draws whole snake on screen */
void snake_print(void)
{
	float rot_angle=0.0;
	/****** Draw Head ******/
	glPushMatrix();

	/* Get next head position */
	head_next_pos();

	/* Keep head at it's origin */
	glTranslatef(CURRENT_HEAD_X,CURRENT_HEAD_Y,0);

	/* Rotate head with current angle if needed to rotate */
	rot_angle = screen_matrix_angle(CURRENT_HEAD_X,CURRENT_HEAD_Y);
	glRotatef(rot_angle,0.0,0.0,1.0);


	/* Set colour to NOSE,SKULL as white */
	glColor3f(WHITE);
	/* Draw Nose */
	block_print(head.nose);
	/* Draw Skull */
	block_print(head.skull);
	/* Draw Eyes */
	/* Set colour to eyes as black */
	glColor3f(BLACK);
	/* Set pixel size of eye points */
	glPointSize(head.eye_size);
	/* Set draw type to Points */
	glBegin(GL_POINTS);
	glVertex2f(head.eye[0].x,head.eye[0].y);/* Left Eye */
	glVertex2f(head.eye[1].x,head.eye[1].y);/* Right Eye */
	/* Set back foreground colour to white */
	glColor3f(WHITE);
	glEnd();
	glPopMatrix();
	/************************/

	/******* Draw Body *******/
	for(int i=0;i<snake_length;i++)
	{
		body_next_pos(i);
		glPushMatrix();	
		/*Keep body at it's origin */
		glTranslatef(body[i].position.x,body[i].position.y,0);

		glColor3f(WHITE);
		block_print(body[i].part);

		glPopMatrix();
	}
	/*************************/
}

/* Draws string on screen */
void output_string(int x,int y,char *string,float color[3])
{
	int length=strlen(string);
	/* Set text colour to black */
	glColor3f(color[0],color[1],color[2]);

	/* Raster sub pixel level for text writing */
	glRasterPos2f(x,y);

	/* Loop through each letter and print it in bitmap format */
	for(int i=0;i<length;i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
	}
	glPopMatrix();
}

/* Handles game when snake is dead */
void dead_fun(void)
{
	char *msg="GAME OVER";
	int msg_len=strlen(msg);
	float rect_index=-msg_len*2;

	/* Set text background rectangle colour to white */
	glColor3f(1.0f,1.0f,1.0f);	
	/* Draw rectangle */
	glRectf(-rect_index,4,rect_index,-4);
	glRectf(-rect_index,14,rect_index,5);

	float color[3]={BLACK};

	/* Print 'game over' on rectangle */
	output_string(-7.0,0.0,"GAME OVER",color);
	
	char score_string[12];
	sprintf(score_string,"SCORE : %d",snake_length);
	output_string(-7.0,9.5,score_string,color);
	
	glutTimerFunc(0,timer_end,0);
}

/* Updates snake hit status
	 1. Egg hit : 1 - hit;0 - Not hit
	 2. dead : 1 - hit boundary;0 - inside boundary   */
void update_hit_status(void)
{
	float head_border = 0.0;
	float head_left_edge = 0.0;
	float head_right_edge = 0.0;

	float egg_border = 0.0;
	float egg_left_edge = 0.0;
	float egg_right_edge = 0.0;

	float body_part_border = 0.0;
	float body_part_left_edge = 0.0;
	float body_part_right_edge = 0.0;

	typedef struct hit_status
	{
		unsigned int head_border_hit:1;
		unsigned int head_left_edge_hit:1;
		unsigned int head_right_edge_hit:1;
		unsigned int border_hit:1;
		unsigned int pos_in_range:1;
		unsigned int body_hit:1;
	}hit_status;

	typedef struct body_hit
	{
		unsigned int head_border_hit:1;
		unsigned int head_left_edge_hit:1;
		unsigned int head_right_edge_hit:1;
		unsigned int pos_in_range:1;
	}body_hit;

	body_hit body_part;

	body_part.head_border_hit = 0;
	body_part.head_left_edge_hit = 0;
	body_part.head_right_edge_hit = 0;
	body_part.pos_in_range = 0;

	hit_status hit;

	hit.head_border_hit = 0;
	hit.head_left_edge_hit = 0;
	hit.head_right_edge_hit = 0;
	hit.border_hit = 0;
	hit.body_hit = 0;

	switch(move_dir)
	{
		case UP:
			head_border = CURRENT_HEAD_Y+(head.nose.point[0].x);
			head_left_edge = CURRENT_HEAD_X-(head.nose.point[0].y);
			head_right_edge = CURRENT_HEAD_X+(-(head.nose.point[3].y));

			egg_border = (egg.position.y)-(-(egg.obst.point[3].y));
			egg_left_edge = (egg.position.x)-(-(egg.obst.point[2].x));
			egg_right_edge = (egg.position.x)+(egg.obst.point[3].x);

			hit.head_border_hit = (head_border >= egg_border);
			hit.head_left_edge_hit = ((head_left_edge >= egg_left_edge) && (head_left_edge <= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge >= egg_left_edge) && (head_right_edge <= egg_right_edge));
			hit.pos_in_range = (CURRENT_HEAD_Y <= egg.position.y);
			hit.border_hit = (head_border >= (game_boundary.point[0].y));


			/* Body part hit check */
			for(int i=0;(i<snake_length && (!hit.body_hit));i++)
			{
				if(body[i].position.y <= (CURRENT_HEAD_Y+(SNAKE_SPEED_DEFAULT*4)))
					continue;
				if((!(body[i].direction^LEFT)) || (!(body[i].direction^RIGHT)))
				{
					body_part_border = (body[i].position.y)-(-(body[i].part.point[3].y));
					body_part_left_edge = (body[i].position.x)-(-(body[i].part.point[2].x));
					body_part_right_edge = (body[i].position.x)+(body[i].part.point[3].x);

					body_part.head_border_hit = (head_border >= body_part_border);
					body_part.head_left_edge_hit = ((head_left_edge >= body_part_left_edge) && (head_left_edge <= body_part_right_edge));
					body_part.head_right_edge_hit = ((head_right_edge >= body_part_left_edge) && (head_right_edge <= body_part_right_edge));
					body_part.pos_in_range = (CURRENT_HEAD_Y <= body[i].position.y);

					hit.body_hit = (body_part.head_border_hit && (body_part.head_left_edge_hit || body_part.head_right_edge_hit))&& (body_part.pos_in_range);
					/*printf("UP  : body[%d] : %d\n",i,hit.body_hit);

						if(hit.body_hit)
						{
						printf("head_border : %d\n",body_part.head_border_hit);
						printf("left_edge   : %d\n",body_part.head_left_edge_hit);
						printf("right_edge  : %d\n",body_part.head_right_edge_hit);
						printf("pos_range   : %d\n",body_part.pos_in_range);
						getchar();
						}*/
				}
			}
			break;

		case DOWN:
			head_border = CURRENT_HEAD_Y-(head.nose.point[0].x);
			head_left_edge = CURRENT_HEAD_X+(head.nose.point[0].y);
			head_right_edge = CURRENT_HEAD_X-(-(head.nose.point[3].y));

			egg_border = (egg.position.y)+(egg.obst.point[0].y);
			egg_left_edge = (egg.position.x)+(egg.obst.point[0].x);
			egg_right_edge = (egg.position.x)-(-(egg.obst.point[1].x));

			hit.head_border_hit = (head_border <= egg_border);
			hit.head_left_edge_hit = ((head_left_edge <= egg_left_edge) && (head_left_edge >= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge <= egg_left_edge) && (head_right_edge >= egg_right_edge));
			hit.pos_in_range = (CURRENT_HEAD_Y >= egg.position.y);
			hit.border_hit = (head_border <= (game_boundary.point[3].y));

			/* Body part hit check */
			for(int i=0;(i<snake_length && (!hit.body_hit));i++)
			{
				if(body[i].position.y >= (CURRENT_HEAD_Y-(SNAKE_SPEED_DEFAULT*4)))
					continue;
				if((!(body[i].direction^LEFT)) || (!(body[i].direction^RIGHT)))
				{
					body_part_border = (body[i].position.y)+(body[i].part.point[0].y);
					body_part_left_edge = (body[i].position.x)+(body[i].part.point[0].x);
					body_part_right_edge = (body[i].position.x)-(-(body[i].part.point[1].x));

					body_part.head_border_hit = (head_border <= body_part_border);
					body_part.head_left_edge_hit = ((head_left_edge <= body_part_left_edge) && (head_left_edge >= body_part_right_edge));
					body_part.head_right_edge_hit = ((head_right_edge <= body_part_left_edge) && (head_right_edge >= body_part_right_edge));
					body_part.pos_in_range = (CURRENT_HEAD_Y >= body[i].position.y);

					hit.body_hit = (body_part.head_border_hit && (body_part.head_left_edge_hit || body_part.head_right_edge_hit))&& (body_part.pos_in_range);
					/*printf("DOWN : body[%d] : %d\n",i,hit.body_hit);

						if(hit.body_hit)
						{
						printf("head_border : %d\n",body_part.head_border_hit);
						printf("left_edge   : %d\n",body_part.head_left_edge_hit);
						printf("right_edge  : %d\n",body_part.head_right_edge_hit);
						printf("pos_range   : %d\n",body_part.pos_in_range);
						getchar();
						}
					 */
				}
			}
			break;

		case LEFT:
			head_border = CURRENT_HEAD_X-(head.nose.point[0].x);
			head_left_edge = CURRENT_HEAD_Y-(head.nose.point[0].y);
			head_right_edge = CURRENT_HEAD_Y+(-(head.nose.point[3].y));

			egg_border = (egg.position.x)+(egg.obst.point[0].x);
			egg_left_edge = (egg.position.y)-(-(egg.obst.point[3].y));
			egg_right_edge = (egg.position.y)+(egg.obst.point[0].y);

			hit.head_border_hit = (head_border <= egg_border);
			hit.head_left_edge_hit = ((head_left_edge >= egg_left_edge) && (head_left_edge <= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge >= egg_left_edge) && (head_right_edge <= egg_right_edge));
			hit.pos_in_range = (CURRENT_HEAD_X >= egg.position.x);
			hit.border_hit = (head_border <= (game_boundary.point[1].x));

			/* Body part hit check */
			for(int i=0;(i<snake_length && (!hit.body_hit));i++)
			{
				if(body[i].position.x >= (CURRENT_HEAD_X-(SNAKE_SPEED_DEFAULT*4)))
					continue;
				if((!(body[i].direction^UP)) || (!(body[i].direction^DOWN)))
				{
					body_part_border = (body[i].position.x)+(body[i].part.point[0].x);
					body_part_left_edge = (body[i].position.y)-(-(body[i].part.point[3].y));
					body_part_right_edge = (body[i].position.y)+(body[i].part.point[0].y);

					body_part.head_border_hit = (head_border <= body_part_border);
					body_part.head_left_edge_hit = ((head_left_edge >= body_part_left_edge) && (head_left_edge <= body_part_right_edge));
					body_part.head_right_edge_hit = ((head_right_edge >= body_part_left_edge) && (head_right_edge <= body_part_right_edge));
					body_part.pos_in_range = (CURRENT_HEAD_X >= body[i].position.x);

					hit.body_hit = (body_part.head_border_hit && (body_part.head_left_edge_hit || body_part.head_right_edge_hit))&& (body_part.pos_in_range);
					/*printf("LEFT : body[%d] : %d\n",i,hit.body_hit);

						if(hit.body_hit)
						{
						printf("head_border : %d\n",body_part.head_border_hit);
						printf("left_edge   : %d\n",body_part.head_left_edge_hit);
						printf("right_edge  : %d\n",body_part.head_right_edge_hit);
						printf("pos_range   : %d\n",body_part.pos_in_range);
						getchar();
						}*/
				}
			}
			break;

		case RIGHT:
			head_border = CURRENT_HEAD_X+(head.nose.point[0].x);
			head_left_edge = CURRENT_HEAD_Y+(head.nose.point[0].y);
			head_right_edge = CURRENT_HEAD_Y-(-(head.nose.point[3].y));

			egg_border = (egg.position.x)-(-(egg.obst.point[1].x));
			egg_left_edge = (egg.position.y)+(egg.obst.point[1].y);
			egg_right_edge = (egg.position.y)-(-(egg.obst.point[2].y));

			hit.head_border_hit = (head_border >= egg_border);
			hit.head_left_edge_hit = ((head_left_edge <= egg_left_edge) && (head_left_edge >= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge <= egg_left_edge) && (head_right_edge >= egg_right_edge));
			hit.pos_in_range = (CURRENT_HEAD_X <= egg.position.x);
			hit.border_hit = (head_border >= (game_boundary.point[0].x));

			/* Body part hit check */
			for(int i=0;(i<snake_length && (!hit.body_hit));i++)
			{
				if(body[i].position.x <= (CURRENT_HEAD_X+(SNAKE_SPEED_DEFAULT*4)))
					continue;
				if((!(body[i].direction^UP)) || (!(body[i].direction^DOWN)))
				{
					body_part_border = (body[i].position.x)-(-(body[i].part.point[1].x));
					body_part_left_edge = (body[i].position.y)+(body[i].part.point[1].y);
					body_part_right_edge = (body[i].position.y)-(-(body[i].part.point[2].y));

					body_part.head_border_hit = (head_border >= body_part_border);
					body_part.head_left_edge_hit = ((head_left_edge <= body_part_left_edge) && (head_left_edge >= body_part_right_edge));
					body_part.head_right_edge_hit = ((head_right_edge <= body_part_left_edge) && (head_right_edge >= body_part_right_edge));
					body_part.pos_in_range = (CURRENT_HEAD_X <= body[i].position.x);

					hit.body_hit = (body_part.head_border_hit && (body_part.head_left_edge_hit || body_part.head_right_edge_hit))&& (body_part.pos_in_range);
					/*	printf("RIGHT : body[%d] : %d\n",i,hit.body_hit);

							if(hit.body_hit)
							{
							printf("head_border : %d\n",body_part.head_border_hit);
							printf("left_edge   : %d\n",body_part.head_left_edge_hit);
							printf("right_edge  : %d\n",body_part.head_right_edge_hit);
							printf("pos_range   : %d\n",body_part.pos_in_range);
							getchar();
							}*/
				}
			}

			break;
	}

	egg_hit = (hit.head_border_hit && (hit.head_left_edge_hit || hit.head_right_edge_hit))&& (hit.pos_in_range);

	dead = (hit.border_hit || hit.body_hit);

}


void new_egg(void)
{
	int next_quadrant = 0;
	int rand[2];

	rand_val(&rand,4);
	next_quadrant = rand[0]+1;

	switch(quadrant_of_vertex(egg.position))
	{
		case	FIRST_QUADRANT :
			if(!(next_quadrant^FIRST_QUADRANT))
				next_quadrant = THIRD_QUADRANT;
			break;

		case	SECOND_QUADRANT :
			if(!(next_quadrant^SECOND_QUADRANT))
				next_quadrant = FOURTH_QUADRANT;
			break;

		case	THIRD_QUADRANT :
			if(!(next_quadrant^THIRD_QUADRANT))
				next_quadrant = FIRST_QUADRANT;
			break;

		case	FOURTH_QUADRANT :
			if(!(next_quadrant^FOURTH_QUADRANT))
				next_quadrant = SECOND_QUADRANT;
			break;
	}

	rand_val(&rand,(BOUNDARY_X-2)-egg_size);
	egg.position.x=(float)rand[0];
	egg.position.y=(float)rand[1];
	switch(next_quadrant)
	{
		case	FIRST_QUADRANT :
			egg.position.x = make_positive(egg.position.x);
			egg.position.y = make_positive(egg.position.y);
			break;

		case	SECOND_QUADRANT :
			egg.position.x = make_negative(egg.position.x);
			egg.position.y = make_positive(egg.position.y);
			break;

		case	THIRD_QUADRANT :
			egg.position.x = make_negative(egg.position.x);
			egg.position.y = make_negative(egg.position.y);
			break;

		case	FOURTH_QUADRANT :
			egg.position.x = make_positive(egg.position.x);
			egg.position.y = make_negative(egg.position.y);
			break;
	}

}

int quadrant_of_vertex(vertex ver)
{
	float x = ver.x;
	float y = ver.y;

	if((x>=0) && (y>=0))
		return FIRST_QUADRANT;
	if((x<0) && (y>0))
		return SECOND_QUADRANT;
	if((x<0) && (y<0))
		return THIRD_QUADRANT;
	if((x>=0) && (y<0))
		return FOURTH_QUADRANT;

	return 0;
}

float make_positive(float val)
{
	if(val<0)
		val *= -1;
	return val;
}

float make_negative(float val)
{
	if(val>0)
		val *= -1;
	return val;
}

void rand_val(int *rand,int modulo_val)
{
	FILE *fp;
	char *command="date";
	char output[29];
	char val[5];

	fp = popen(command,"r");
	fscanf(fp,"%s %s %s %s",output,output+3,output+6,output+8);
	pclose(fp);

	sprintf(val,"%c%c%c%c",output[11],output[12],output[14],output[15]);
	rand[0]=atoi(val)%modulo_val;
	for(int i=i,j=3;i<j;i++,j--)
	{
		char temp = val[i];
		val[i] = val[j];
		val[j] = temp;
	}
	rand[1]=atoi(val)%modulo_val;
}

/* screen_matrix initialisation */
void screen_matrix_init(void)
{
	for(int i=0;i<SCREEN_MATRIX_X;i++)
	{
		for(int j=0;j<SCREEN_MATRIX_Y;j++)
		{
			screen_matrix[i][j].dir = 0;
			screen_matrix[i][j].angle = 0;
		}
	}
	screen_matrix_update(CURRENT_HEAD_X,CURRENT_HEAD_Y,RIGHT,0.0);
	screen_matrix_update(body[0].position.x,body[0].position.y,RIGHT,0.0);
}

/* screen_matrix update */
void screen_matrix_update(float x,float y,int dir,float angle)
{
	int i=0,j=0;

	x = x/SNAKE_SPEED_DEFAULT;
	y = y/SNAKE_SPEED_DEFAULT;

	if(x>=0)
		j = (SCREEN_MATRIX_Y/2)+x;
	if(x<0)
		j = (SCREEN_MATRIX_Y/2)-(-x);

	if(y>=0)
		i = (SCREEN_MATRIX_X/2)-y;
	if(y<0)
		i = (SCREEN_MATRIX_X/2)+(-y);

	screen_matrix[i][j].dir = dir;
	screen_matrix[i][j].angle = angle;
}


/* screen_matrix angle at co-ordinates */
float screen_matrix_angle(float x,float y)
{
	int i,j;

	x = x/SNAKE_SPEED_DEFAULT;
	y = y/SNAKE_SPEED_DEFAULT;

	if(x>=0)
		j = (SCREEN_MATRIX_Y/2)+x;
	if(x<0)
		j = (SCREEN_MATRIX_Y/2)-(-x);

	if(y>=0)
		i = (SCREEN_MATRIX_X/2)-y;
	if(y<0)
		i = (SCREEN_MATRIX_X/2)+(-y);

	return screen_matrix[i][j].angle;
}

/* screen_matrix dir at co-ordinates */
int screen_matrix_dir(float x,float y)
{
	int i,j;

	x = x/SNAKE_SPEED_DEFAULT;
	y = y/SNAKE_SPEED_DEFAULT;

	if(x>=0)
		j = (SCREEN_MATRIX_Y/2)+x;
	if(x<0)
		j = (SCREEN_MATRIX_Y/2)-(-x);

	if(y>=0)
		i = (SCREEN_MATRIX_X/2)-y;
	if(y<0)
		i = (SCREEN_MATRIX_X/2)+(-y);

	return screen_matrix[i][j].dir;
}

void head_next_pos(void)
{
	screen_matrix_update(CURRENT_HEAD_X,CURRENT_HEAD_Y,move_dir,snake_rot_angle);

	/* Increment position of snake based on current movement direction */
	switch(move_dir)
	{
		case	UP:
			head.position.y += snake_speed;
			break;

		case DOWN:
			head.position.y -= snake_speed;
			break;

		case LEFT:
			head.position.x -= snake_speed;
			break;

		case RIGHT:
			head.position.x += snake_speed;
			break;

			/* If END is key by USER pressed then close window */
		case END:
			glutDestroyWindow(window_id);
			break;
	}
	screen_matrix_update(CURRENT_HEAD_X,CURRENT_HEAD_Y,0,snake_rot_angle);


	/* Update snake hit status */

}

/* Update next transition position of body */	
void body_next_pos(int i)
{
	int old_dir = body[i].direction;
	switch(old_dir)
	{
		case	UP:
			body[i].position.y += snake_speed;
			break;

		case DOWN:
			body[i].position.y -= snake_speed;
			break;

		case LEFT:
			body[i].position.x -= snake_speed;
			break;

		case RIGHT:
			body[i].position.x += snake_speed;
			break;
      
		case END:
			break;
	}

	int new_dir = screen_matrix_dir(body[i].position.x,body[i].position.y);
	if((old_dir^new_dir) && (new_dir)) /* Change in direction */
		body[i].direction = new_dir;
}

/* Add one body part */
void extend_body(void)
{
	body = (snake_body*)realloc(body,snake_length*sizeof(snake_body));
	if(!body)
	{
		perror("snake_init(): realloc() error:");
		exit(1);
	}

	int i = snake_length-1;

	body[i].part.point[0].x = body[0].part.point[0].x;
	body[i].part.point[0].y = body[0].part.point[0].y;
	body[i].part.point[1].x = body[0].part.point[1].x;
	body[i].part.point[1].y = body[0].part.point[1].y;
	body[i].part.point[2].x = body[0].part.point[2].x;
	body[i].part.point[2].y = body[0].part.point[2].y;
	body[i].part.point[3].x = body[0].part.point[3].x;
	body[i].part.point[3].y = body[0].part.point[3].y;

	int prev_part_dir = body[i-1].direction;
	switch(prev_part_dir)
	{
		case UP:
			body[i].position.x = body[i-1].position.x;
			body[i].position.y = body[i-1].position.y - (SNAKE_SPEED_DEFAULT*9);
			break;

		case DOWN:
			body[i].position.x = body[i-1].position.x;
			body[i].position.y = body[i-1].position.y + (SNAKE_SPEED_DEFAULT*9);
			break;

		case LEFT:
			body[i].position.x = body[i-1].position.x + (SNAKE_SPEED_DEFAULT*9);
			body[i].position.y = body[i-1].position.y;
			break;

		case RIGHT:
			body[i].position.x = body[i-1].position.x - (SNAKE_SPEED_DEFAULT*9);
			body[i].position.y = body[i-1].position.y;
			break;
	}


	body[i].number = i;
	body[i].direction = prev_part_dir;

	body[i-1].next = body+i;
	body[i].next = NULL;
}

