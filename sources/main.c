#include"../includes/snake.h"
#include"../includes/screen.h"
#include<stdio.h>
#include<string.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>


int main(int argc,char *argv[])
{
	snake_init();
	screen_init(&argc,argv);

	glutMainLoop();
	return 0;
}
