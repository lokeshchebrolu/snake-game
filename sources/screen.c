/***  Header file inclusion ***/
#include"../includes/screen.h"
#include"../includes/snake.h"
#include<stdio.h>
#include<string.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>

static int move_dir=RIGHT;
static float snake_rot_angle=0.0;

static float x_pos,y_pos;

/*** Local function declaration ***/
void boundary_init(void);
void boundary(void);
void next_egg(void);
void block_print(block shape);
void snake_print(void);
void output_string(char *string);
void dead_fun(void);
void update_hit_status(void);

/*** Globale variable definitions ***/
int window_id;
block game_boundary;

/*** Global function definitions ***/
void screen_init(int *argc,char *argv[])
{
	/* OpenGL Init functions */
	glutInit(argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(WINDOW_POS_X,WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);

	window_id=glutCreateWindow("Snake Game");
	
	glClearColor(0,0,0,1);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutSpecialFunc(specialInput);

	boundary_init();

	x_pos = -BOUNDARY_X;
	y_pos = 0;
}


void display(void)
{
	/* Clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	boundary();

	if(!dead)
	{
		if(!egg_hit)
		{
			next_egg();
		}
		else
		{
		}
		snake_print();
	}
	else
	{
		glPushMatrix();
		dead_fun();
		glPopMatrix();
	}
	/* Displays frame buffer on screen */
	glFlush();
	glutSwapBuffers();
}

void reshape(int width,int height)
{
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-MAX_X,MAX_Y,-MAX_X,MAX_Y);
	glMatrixMode(GL_MODELVIEW);
}

void timer(int arg)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60,timer,0);

	switch(move_dir)
	{
		case	UP:
			y_pos += snake_speed;
			break;

		case DOWN:
			y_pos -= snake_speed;
			break;

		case LEFT:
			x_pos -= snake_speed;
			break;

		case RIGHT:
			x_pos += snake_speed;
			break;

		case END:
			glutDestroyWindow(window_id);
			break;
	}
	update_hit_status();
}

void specialInput(int key,int x,int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
					snake_rot_angle += -90.0;
				else if(!(move_dir^RIGHT))
					snake_rot_angle += 90.0;
				move_dir=UP;
			}
			break;

		case GLUT_KEY_DOWN:
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
					snake_rot_angle += 90.0;
				else if(!(move_dir^RIGHT))
					snake_rot_angle += -90.0;
				move_dir=DOWN;
			}
			break;

		case GLUT_KEY_LEFT:
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
					snake_rot_angle += 90.0;
				else if(!(move_dir^DOWN))
					snake_rot_angle += -90.0;
				move_dir=LEFT;
			}
			break;

		case GLUT_KEY_RIGHT:
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
					snake_rot_angle += -90.0;
				else if(!(move_dir^DOWN))
					snake_rot_angle += 90.0;
				move_dir=RIGHT;
			}
			break;

		case GLUT_KEY_END:
			move_dir=END;
			break;
	}
}

/*** Local function definitions ***/
void boundary_init(void)
{
	/* Empty square formed with 4 lines */
	/* Initialize boundary vertices*/
	game_boundary.point[0].x=BOUNDARY_X;
	game_boundary.point[0].y=BOUNDARY_Y;
	
	game_boundary.point[1].x=-BOUNDARY_X;
	game_boundary.point[1].y=BOUNDARY_Y;
	
	game_boundary.point[2].x=-BOUNDARY_X;
	game_boundary.point[2].y=-BOUNDARY_Y;
	
	game_boundary.point[3].x=BOUNDARY_X;
	game_boundary.point[3].y=-BOUNDARY_Y;
}

void block_print(block shape)
{
	glBegin(GL_POLYGON);
	for(int i=0;i<4;i++)
		glVertex2f(shape.point[i].x,shape.point[i].y);
	glEnd();
}

void boundary(void)
{
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	block_print(game_boundary);
	glEnd();
	glPopMatrix();
}

void next_egg(void)
{
	glPushMatrix();
	/* Keep egg at it's position */
	glTranslatef(egg.position.x,egg.position.y,0);
	glBegin(GL_POLYGON);
	block_print(egg.obst);
	glEnd();
	glPopMatrix();
}

void snake_print(void)
{
	glPushMatrix();
	glTranslatef(x_pos,y_pos,0.0);
	glRotatef(snake_rot_angle,0.0,0.0,1.0);
	glColor3f(WHITE);
	/*** Draw Head ***/
	/* Nose */
	block_print(head.nose);
	/*Skull*/
	block_print(head.skull);
	/* Eyes */
	glColor3f(BLACK);
	glPointSize(head.eye_size);
	glBegin(GL_POINTS);
	glVertex2f(head.eye[0].x,head.eye[0].y);/* Left Eye */
	glVertex2f(head.eye[1].x,head.eye[1].y);/* Right Eye */
	glColor3f(WHITE);
	glEnd();
	glPopMatrix();
}

void output_string(char *string)
{
	int x=0,length=strlen(string);
	x=length/2;
	glColor3f(0.0f,0.0f,0.0f);
	glRasterPos2f(-x*2,0);
	for(int i=0;i<length;i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
	}
}

void dead_fun(void)
{
	char *msg="GAME OVER";
	int msg_len=strlen(msg);
	float rect_index=-msg_len*2;
	glColor3f(1.0f,1.0f,1.0f);	
	glRectf(-rect_index,5,rect_index,-5);
	output_string("GAME OVER");
}


void update_hit_status(void)
{
	float head_border = 0.0;
	float head_left_edge = 0.0;
	float head_right_edge = 0.0;

	float egg_border = 0.0;
	float egg_left_edge = 0.0;
	float egg_right_edge = 0.0;

	switch(move_dir)
	{
		case UP:
			head_border = y_pos+(head.nose.point[0].x);
			head_left_edge = x_pos-(head.nose.point[0].y);
			head_right_edge = x_pos+(-head.nose.point[3].y);

			egg_border = (egg.position.y)-(-egg.obst.point[3].y);
			egg_left_edge = (egg.position.x)-(-egg.obst.point[2].x);
			egg_right_edge = (egg.position.x)+(egg.obst.point[3].x);

			if((head_border >= egg_border) &&
					(((head_left_edge>= egg_left_edge) && (head_left_edge<= egg_right_edge)) ||
					((head_right_edge>= egg_left_edge) && (head_right_edge<= egg_right_edge))))
				egg_hit=1;

			if(head_border >= (game_boundary.point[0].y))
				dead=1;
			break;

		case DOWN:
			head_border = y_pos-(head.nose.point[0].x);
			head_left_edge = x_pos+(head.nose.point[0].y);
			head_right_edge = x_pos-(-head.nose.point[3].y);

			egg_border = (egg.position.y)+(egg.obst.point[0].y);
			egg_left_edge = (egg.position.x)+(egg.obst.point[0].x);
			egg_right_edge = (egg.position.x)-(-egg.obst.point[1].x);

			if((head_border <= egg_border) &&
					(((head_left_edge<= egg_left_edge) && (head_left_edge>= egg_right_edge)) ||
					((head_right_edge<= egg_left_edge) && (head_right_edge>= egg_right_edge))))
				egg_hit=1;

			if(head_border <= (game_boundary.point[3].y))
				dead=1;
			break;

		case LEFT:
			head_border = x_pos-(head.nose.point[0].x);
			head_left_edge = y_pos-(head.nose.point[0].y);
			head_right_edge = y_pos+(-head.nose.point[3].y);

			egg_border = (egg.position.x)+(egg.obst.point[0].x);
			egg_left_edge = (egg.position.y)-(-egg.obst.point[3].y);
			egg_right_edge = (egg.position.y)+(egg.obst.point[0].y);

			if((head_border <= egg_border) &&
					(((head_left_edge>= egg_left_edge) && (head_left_edge<= egg_right_edge)) ||
					((head_right_edge>= egg_left_edge) && (head_right_edge<= egg_right_edge))))
				egg_hit=1;		

			if(head_border <= (game_boundary.point[1].x))
				dead=1;
			break;

		case RIGHT:
			head_border = x_pos+(head.nose.point[0].x);
			head_left_edge = y_pos+(head.nose.point[0].y);
			head_right_edge = y_pos-(-head.nose.point[3].y);

			egg_border = (egg.position.x)-(-egg.obst.point[1].x);
			egg_left_edge = (egg.position.y)+(egg.obst.point[0].y);
			egg_right_edge = (egg.position.y)-(-egg.obst.point[3].y);

			if((head_border >= egg_border) &&
					(((head_left_edge<= egg_left_edge) && (head_left_edge>= egg_right_edge)) ||
					((head_right_edge>= egg_left_edge) && (head_right_edge>= egg_right_edge))))
				egg_hit=1;

			if(head_border >= (game_boundary.point[0].x))
				dead=1;			
			break;
	}
}































