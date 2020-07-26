#ifndef __TYPES_H
#define __TYPES_H

typedef struct vertix
{
	float x;
	float y;
}vertex;

typedef struct block
{
	vertex point[4];
}block;

typedef struct snake_body
{
	block up;
	block down;
	int number;
	int direction;
	int rotate;
}snake_body;

typedef struct snake_head
{
	vertex eye[2];
	int eye_size;
	block nose;
	block skull;
	int direction;
	int rotate;
}snake_head;

typedef struct obstacle
{
	block obst;
	vertex position;
}obstacle;

#endif
