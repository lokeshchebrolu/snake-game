/******  Header file inclusion ******/
#include"../includes/screen.h"
#include"../includes/snake.h"
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>

/****** Local variables ******/
/* Current movement direction */
static int move_dir=RIGHT;

/* Current rotation angle to be done */
static float snake_rot_angle=0.0;

/* Translation co-ordinates for movement of snake */
static float x_pos,y_pos;

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


/****** Globale variable definitions ******/
/* Variable to hold the created window id value
   This is used for closing window */
int window_id;
/* Game boundary vertices variable */
block game_boundary;

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
	
	/* Set initial position of snake */
	x_pos = -BOUNDARY_X;
	y_pos = 0;
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
	
	/* Increment position of snake based on current movement direction */
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
			
		/* If END is key by USER pressed then close window */
		case END:
			glutDestroyWindow(window_id);
			break;
	}


	/* Update snake hit status */
	update_hit_status();
}

/* OpenGL default Input capture function Used to read KEY press from keyboard*/
void specialInput(int key,int x,int y)
{
	/* Update new direction based on key pressed by user */
	switch(key)
	{
		case GLUT_KEY_UP: /* UP arrow key */
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
					snake_rot_angle += -90.0;
				else if(!(move_dir^RIGHT))
					snake_rot_angle += 90.0;
				move_dir=UP;
			}
			break;

		case GLUT_KEY_DOWN: /* DOWN arrow key */
			if(!(move_dir^LEFT) || !(move_dir^RIGHT))
			{
				if(!(move_dir^LEFT))
					snake_rot_angle += 90.0;
				else if(!(move_dir^RIGHT))
					snake_rot_angle += -90.0;
				move_dir=DOWN;
			}
			break;

		case GLUT_KEY_LEFT: /* LEFT arrow key */
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
					snake_rot_angle += 90.0;
				else if(!(move_dir^DOWN))
					snake_rot_angle += -90.0;
				move_dir=LEFT;
			}
			break;

		case GLUT_KEY_RIGHT: /* RIGHT arrow key */
			if(!(move_dir^UP) || !(move_dir^DOWN))
			{
				if(!(move_dir^UP))
					snake_rot_angle += -90.0;
				else if(!(move_dir^DOWN))
					snake_rot_angle += 90.0;
				move_dir=RIGHT;
			}
			break;

		case GLUT_KEY_END: /* END key */
			move_dir=END;
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
	game_boundary.point[2].y=-BOUNDARY_Y;
	
	game_boundary.point[3].x=BOUNDARY_X;
	game_boundary.point[3].y=-BOUNDARY_Y;

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
	/* draw GAME_BOUNDARY */
	glPushMatrix();	
	glBegin(GL_LINE_LOOP);
	block_print(game_boundary);
	glEnd();
	glPopMatrix();
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
	glPushMatrix();
	
	/****** Draw Head ******/
	/* Keep head position */
	glTranslatef(x_pos,y_pos,0.0);
	
	/* Rotate head with current angle if needed to rotate */
	glRotatef(snake_rot_angle,0.0,0.0,1.0);
	
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
	glRectf(-rect_index,5,rect_index,-5);
	
	float color[3]={BLACK};

	/* Print 'game over' on rectangle */
	output_string(-(MAX_X/2),0.0,"GAME OVER",color);
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

	typedef struct hit_status
	{
		unsigned int head_border_hit:1;
		unsigned int head_left_edge_hit:1;
		unsigned int head_right_edge_hit:1;
		unsigned int border_hit:1;
	}hit_status;

	hit_status hit;

	switch(move_dir)
	{
		case UP:
			head_border = y_pos+(head.nose.point[0].x);
			head_left_edge = x_pos-(head.nose.point[0].y);
			head_right_edge = x_pos+(-(head.nose.point[3].y));

			egg_border = (egg.position.y)-(-(egg.obst.point[3].y));
			egg_left_edge = (egg.position.x)-(-(egg.obst.point[2].x));
			egg_right_edge = (egg.position.x)+(egg.obst.point[3].x);

			hit.head_border_hit = (head_border >= egg_border);
			hit.head_left_edge_hit = ((head_left_edge >= egg_left_edge) && (head_left_edge <= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge >= egg_left_edge) && (head_right_edge <= egg_right_edge));
			hit.border_hit = (head_border >= (game_boundary.point[0].y));

			break;

		case DOWN:
			head_border = y_pos-(head.nose.point[0].x);
			head_left_edge = x_pos+(head.nose.point[0].y);
			head_right_edge = x_pos-(-(head.nose.point[3].y));

			egg_border = (egg.position.y)+(egg.obst.point[0].y);
			egg_left_edge = (egg.position.x)+(egg.obst.point[0].x);
			egg_right_edge = (egg.position.x)-(-(egg.obst.point[1].x));

			hit.head_border_hit = (head_border <= egg_border);
			hit.head_left_edge_hit = ((head_left_edge <= egg_left_edge) && (head_left_edge >= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge <= egg_left_edge) && (head_right_edge >= egg_right_edge));
			hit.border_hit = (head_border <= (game_boundary.point[3].y));
			
			break;

		case LEFT:
			head_border = x_pos-(head.nose.point[0].x);
			head_left_edge = y_pos-(head.nose.point[0].y);
			head_right_edge = y_pos+(-(head.nose.point[3].y));

			egg_border = (egg.position.x)+(egg.obst.point[0].x);
			egg_left_edge = (egg.position.y)-(-(egg.obst.point[3].y));
			egg_right_edge = (egg.position.y)+(egg.obst.point[0].y);

			hit.head_border_hit = (head_border <= egg_border);
			hit.head_left_edge_hit = ((head_left_edge >= egg_left_edge) && (head_left_edge <= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge >= egg_left_edge) && (head_right_edge <= egg_right_edge));
			hit.border_hit = (head_border <= (game_boundary.point[1].x));
			
			break;

		case RIGHT:
			head_border = x_pos+(head.nose.point[0].x);
			head_left_edge = y_pos+(head.nose.point[0].y);
			head_right_edge = y_pos-(-(head.nose.point[3].y));

			egg_border = (egg.position.x)-(-(egg.obst.point[1].x));
			egg_left_edge = (egg.position.y)+(egg.obst.point[1].y);
			egg_right_edge = (egg.position.y)-(-(egg.obst.point[2].y));

			hit.head_border_hit = (head_border >= egg_border);
			hit.head_left_edge_hit = ((head_left_edge <= egg_left_edge) && (head_left_edge >= egg_right_edge));
			hit.head_right_edge_hit = ((head_right_edge <= egg_left_edge) && (head_right_edge >= egg_right_edge));
			hit.border_hit = (head_border >= (game_boundary.point[0].x));
			
			break;
	}

	egg_hit = (hit.head_border_hit && (hit.head_left_edge_hit || hit.head_right_edge_hit));
	
	dead = (hit.border_hit);
}


void new_egg(void)
{
	egg.position.x = rand();
	egg.position.y = rand();

	int sign = rand();
	if(sign&0)
		sign = 1;
	else
		sign = -1;

	egg.position.x = fmod(((sign)*(egg.position.x)),(MAX_X-egg_size));
	egg.position.y = fmod(((sign)*(egg.position.y)),(MAX_Y-egg_size));
}




