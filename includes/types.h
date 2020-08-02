#ifndef __TYPES_H
#define __TYPES_H
/* File Description: All object types of game*/

/* Vertex of a 2D Plane. Has (x,y) values  */
typedef struct vertix
{
	float x; /* X co-ordinate value */
	float y; /* Y co-ordinate value */
}vertex;

/* Block is a polygon with 4 vertices
   Example: Square, rectangle */
typedef struct block
{
	/*4 Points for 4 vertices */
	vertex point[4];
}block;

/**************************/
/*    Snake body shape    */
/*      _____________     */
/*     |             |    */
/*     |             |    */
/*     |             |    */
/*     |             |    */
/*     |             |    */
/*     |_____________|    */
/*                        */
/**************************/
typedef struct snake_body
{
	block part; /* Body block */
	vertex position;
	int number; /* position in whole body */
	int direction; /* Movement direction */
	struct snake_body *next; /* Pointer to next body part */
}snake_body;


/**************************/
/*    Snake Head shape    */
/*                        */
/*     _______            */
/*    |       |           */
/*    |       |_______    */
/*    |       |   .   |   */
/*    | SKULL | NOSE  |   */
/*    |       |   .   |   */
/*    |       |_______|   */
/*    |       |           */
/*    |_______|           */
/*                        */
/*   '.' -> eyes          */
/**************************/
typedef struct snake_head
{
	vertex eye[2]; /* Left,Right eye vertices */
	int eye_size; /* Eye size in pixels */
	block nose; /* NOSE block */
	block skull; /* SKULL block */
	vertex position;
	int direction; /* Movement direction of head */
}snake_head;


/**************************/
/*  Obstacle in 2D Plane  */
/*   __________________   */
/*  |                  |  */
/*  |                  |  */
/*  |       .(position)|  */
/*  |                  |  */
/*  |                  |  */
/*  |__________________|  */
/*                        */
/**************************/
typedef struct obstacle
{
	block obst; /* Block of obstacle */
	vertex position; /* Origin position of obstacle in 2D Plane */
}obstacle;

#endif
