#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

unsigned int width = 700;
unsigned int height = 700;
unsigned int *bitArray;
float *pixelArray, *pixels_next;

void pushAvalanche(unsigned int, unsigned int);
void checkAvalanche(unsigned int, unsigned int);
void Avalanche(unsigned int, unsigned int);

struct node {
   unsigned int x, y;
   struct node *next;
};

struct node *head = NULL;
struct node *tail = NULL;

unsigned int PixelPosition(unsigned int x, unsigned int  y)
{
    return (x*width + y) * 3;
}

unsigned int Position(unsigned int x, unsigned int y)
{
    return x*width + y;
}

void pushAvalanche(unsigned int x, unsigned int y)
{             
    struct node *temp = (struct node*) malloc(sizeof(struct node));
    temp->x = x;
    temp->y = y;
    tail->next = temp;
    tail = temp;
}

void checkAvalanche(unsigned int x, unsigned int y)
{
    if(bitArray[Position(x,y)] >= 4)
    {   
        bitArray[Position(x,y)] -= 4;   
        pushAvalanche(x, y);
        Avalanche(x, y);
    }
}

void Avalanche(unsigned int x, unsigned int y)
{
    if(x < width)
    {
        bitArray[Position(x+1,y)] += 1; // Right     
        checkAvalanche(x+1, y);
    }

    if(x > 0)
    {
        bitArray[Position(x-1,y)] += 1; // Left
        checkAvalanche(x-1, y);
    }

    if(y < height)
    {
        bitArray[Position(x,y+1)] += 1; // Down
        checkAvalanche(x, y+1);
    }

    if(y > 0)
    {
        bitArray[Position(x,y-1)] += 1; // Up
        checkAvalanche(x, y-1);
    }
}

void Simulate()
{
    do
    {
        struct node *temp = head;
        checkAvalanche(head->x, head->y);

        if(head != tail)
        {
            head = head->next;
        }

        free(temp);
    } while(head != tail);
}

void displayPile()
{
    for(unsigned int i = 0; i < width; ++i)
    {
        for(unsigned int j = 0; j < height; ++j)
        {
            if(bitArray[Position(i,j)] == 0)
            {
                pixelArray[PixelPosition(i,j)] = 0.0f;
                pixelArray[PixelPosition(i,j)+1] = 0.0f;
                pixelArray[PixelPosition(i,j)+2] = 0.0f;
            }
            else if(bitArray[Position(i,j)] == 1)
            {
                pixelArray[PixelPosition(i,j)] = 1.0f;
                pixelArray[PixelPosition(i,j)+1] = 0.0f;
                pixelArray[PixelPosition(i,j)+2] = 0.0f;
            }
            else if(bitArray[Position(i,j)] == 2)
            {
                pixelArray[PixelPosition(i,j)] = 0.0f;
                pixelArray[PixelPosition(i,j)+1] = 1.0f;
                pixelArray[PixelPosition(i,j)+2] = 0.0f;
            }
            else{
                pixelArray[PixelPosition(i,j)] = 0.0f;
                pixelArray[PixelPosition(i,j)+1] = 0.0f;
                pixelArray[PixelPosition(i,j)+2] = 1.0f;
            }
        }
    }
}

//-------------------------------------------------------------------------
//  Set OpenGL program initial state.
//-------------------------------------------------------------------------
void init()
{
	//  Set the frame buffer clear color to black. 
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

unsigned long counter = 0l;
unsigned int count = 0;
//-------------------------------------------------------------------------
//  This function is passed to glutDisplayFunc in order to display 
//  OpenGL contents on the window.
//-------------------------------------------------------------------------
void display(void)
{
	//  Clear the window or more specifically the frame buffer...
	//  This happens by replacing all the contents of the frame
	//  buffer by the clear color (black in our case)
	/*glClear(GL_COLOR_BUFFER_BIT);*/


    // Start pouring here
    bitArray[Position(width/2, height/2)] += 1;

    if(bitArray[Position(width/2, height/2)] >= 4)
    {
        struct node *temp = (struct node*) malloc(sizeof(struct node));
        temp->x = width/2;
        temp->y = height/2;

        head = temp;
        tail = head;

        Simulate();
    }
    


    count++;
    counter++;
    if(count ==10)
    {
        displayPile();
        count=0;
        printf("%lu\n", counter);
    }
    
	//  Swap contents of backward and forward frame buffers
	glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixelArray);
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}


void main(unsigned int argc, char **argv)
{
     
    unsigned int newBitArray[width*height];
    bitArray = newBitArray;


    float newPixelArray[width*height*3];
    pixelArray = newPixelArray;


    for(unsigned int i = 0; i < width; ++i)
    {
        for(unsigned int j = 0; j < height; ++j)
        {
            bitArray[Position(i,j)] = 0;
            pixelArray[PixelPosition(i,j)] = 0.0f;
            pixelArray[PixelPosition(i,j)+1] =0.0f;
            pixelArray[PixelPosition(i,j)+2] = 0.0f;
        }
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a triangle");
    glutDisplayFunc(display);
    glutMainLoop();
}