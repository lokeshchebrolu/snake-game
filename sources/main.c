/* File Description: main application file*/

/*** Heade file inclusion ***/
#include"../includes/snake.h"
#include"../includes/screen.h"
#include<stdio.h>
#include<string.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>

/* main function */
int main(int argc,char *argv[])
{
	/* Initialize snake module */
	snake_init(); 
	
	/* Initialize screen module */
	screen_init(&argc,argv); 
	
	/* Call GLUT mainloop for continuous running of graphics */
	glutMainLoop();
	return 0;
}
