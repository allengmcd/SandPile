//Allen McDermott
//This code is based off of code that was given to us in class for CSci 364 for the heat diffusion simulation
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>

//These are for the audio
#include <Mmsystem.h>
#include <mciapi.h>

//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

using std::cout;
using std::ofstream;
using std::vector;

void init();
void display(void);
void centerOnScreen();
void drawObjectRedPile();
void drawPile();

//  define the window position on screen
int window_x;
int window_y;
int total_Sand;

float *pixels_current, *pixels_next;

//  variables representing the window size
int window_width = 500;
int window_height = 500;


//Output CSV file
ofstream out("fourGraphData.csv", std::fstream::app);


//	variables that determine how big the sand pile is 
int count1, count2;

//  variable representing the window title
char *window_title = "SAND PILES AND AVALANCHES";


//Struct for pixel location
struct pixel_position {
	int y;
	int x;
};

//Vector of pixel_position
vector<pixel_position> pixel_array;


//every 3rd pixel value is the red channel for that pixel
static int POSITION(int x, int y) {
	return ((x * window_width) + y) * 3;
}



//-------------------------------------------------------------------------
//  Program Main method.
//-------------------------------------------------------------------------
void main(int argc, char **argv)
{

	//	Initialize the sand pile size
	count1 = 2;
	count2 = 0;
	total_Sand = 0;

	//	Play music
	/*mciSendString("open music1.mp3 type mpegvideo alias song1", NULL, 0, NULL);
	mciSendString("play song1 repeat", NULL, 0, NULL);*/


	//	Define the window_size
	int window_size = window_height * window_width;

	//	Initialize the array of pixels
	pixels_next = new float[window_size * 3];
	pixels_current = new float[window_size * 3];


	//	Set the values in Pixels_next
	for (int i = window_size/2; i < window_size * 3; i++) {
		pixels_next[i] = 0;
		pixels_current[i] = 0;
	}

	//  Connect to the windowing system + create a window
	//  with the specified dimensions and position
	//  + set the display mode + specify the window title.
	glutInit(&argc, argv);
	centerOnScreen();
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(window_x, window_y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(window_title);

	//  Set OpenGL program initial state.
	init();

	// Set the callback functions
	glutDisplayFunc(display);

	//  Start GLUT event processing loop
	glutMainLoop();
}

//-------------------------------------------------------------------------
//  Set OpenGL program initial state.
//-------------------------------------------------------------------------
void init()
{
	//  Set the frame buffer clear color to black. 
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

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

	//  Draw object
	drawPile();

	//  Swap contents of backward and forward frame buffers
	glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels_next);
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}


//-------------------------------------------------------------------------
//  This function sets the color for a pixel given its coordinates
//	If the pixel has 4 sand grains in it then the function will push the pixel position onto a vector
//-------------------------------------------------------------------------
void color_pixel(int x, int y) {
	if (pixels_current[POSITION(x, y)] == .25) {
		pixels_next[POSITION(x, y)] = 1;
		pixels_next[POSITION(x, y) + 1] = 0;
		pixels_next[POSITION(x, y) + 2] = 0;
	}
	else if (pixels_current[POSITION(x, y)] == .5) {
		pixels_next[POSITION(x, y)] = 0;
		pixels_next[POSITION(x, y) + 1] = 1;
		pixels_next[POSITION(x, y) + 2] = 0;
	}
	else if (pixels_current[POSITION(x, y)] > .5) {
		pixels_next[POSITION(x, y)] = 0;
		pixels_next[POSITION(x, y) + 1] = 0;
		pixels_next[POSITION(x, y) + 2] = 1;
	}
	else {
		pixels_next[POSITION(x, y)] = 0;
		pixels_next[POSITION(x, y) + 1] = 0;
		pixels_next[POSITION(x, y) + 2] = 0;
	}

	if (pixels_current[POSITION(x, y)] == 1) {
		struct pixel_position temp;
		temp.x = x;
		temp.y = y;
		pixel_array.push_back(temp);
	}

	return;
}



//-------------------------------------------------------------------------
//  Sets the values for when a avalanch occurs 
//	When the sand reaches the edge of the display it vanishes
//-------------------------------------------------------------------------
void avalanch(struct pixel_position temp) {

	int x, y;
	x = temp.x;
	y = temp.y;

	//pop the first item in the vector
	pixel_array.erase(pixel_array.begin());

	//Subtract the pixel by 1 because the sand goes up, down, left, right
	pixels_current[POSITION(x, y)] -= 1;
	color_pixel(x, y);


	//Sand goes left
	if ((x + 1)<window_width) {
		pixels_current[POSITION(x + 1, y)] += .25;
		color_pixel(x+1,y);
	}

	//Sand goes right
	if ((x - 1)>0) {
		pixels_current[POSITION(x - 1, y)] += .25;
		color_pixel(x - 1, y);
	}

	//Sand goes down
	if ((y + 1)<window_height) {
		pixels_current[POSITION(x, y + 1)] += .25;		
		color_pixel(x, y+1);
	}

	//Sand goes up
	if ((y - 1) > 0) {
		pixels_current[POSITION(x, y - 1)] += .25;		
		color_pixel(x, y-1);
	}

	return;
}



//-------------------------------------------------------------------------
//  Draws our object.
//-------------------------------------------------------------------------
void drawPile()
{
	int cc = 0;
	/*while (cc < 10000) {*/
		//Pour sand

		// 1 pile
		pixels_current[POSITION(window_width / 2, window_height / 2)] += .25;
		color_pixel(window_width / 2, window_height / 2);
		total_Sand++;


		// 2 piles
		pixels_current[POSITION(window_width / 2 + 10, window_height / 2)] += .25;
		color_pixel(window_width / 2 + 10, window_height / 2);
		total_Sand++;

		pixels_current[POSITION(window_width / 2 - 10, window_height / 2)] += .25;
		color_pixel(window_width / 2 - 10, window_height / 2);
		total_Sand++;


		// 4 piles
		pixels_current[POSITION(window_width / 2 + 10, window_height / 2 + 10)] += .25;
		color_pixel(window_width / 2 + 10, window_height / 2 + 10);
		total_Sand++;

		pixels_current[POSITION(window_width / 2 - 10, window_height / 2 - 10)] += .25;
		color_pixel(window_width / 2 - 10, window_height / 2 - 10);
		total_Sand++;

		pixels_current[POSITION(window_width / 2 + 10, window_height / 2 - 10)] += .25;
		color_pixel(window_width / 2 + 10, window_height / 2 - 10);
		total_Sand++;

		pixels_current[POSITION(window_width / 2 - 10, window_height / 2 + 10)] += .25;
		color_pixel(window_width / 2 - 10, window_height / 2 + 10);
		total_Sand++;


		// random spots in a square
		int xLower = window_width / 4 + 100;
		int xUpper = window_width / 16;
		int yLower = window_height / 4 + 100;
		int yUpper = window_height / 16;

		int xRand = rand() % xUpper + xLower;
		int yRand = rand() % yUpper + yLower;


		pixels_current[POSITION(xRand, yRand)] += .25;
		color_pixel(xRand, yRand);
		total_Sand++;

		cout << "Hello" << endl;
		int avalanchCount = 0;

		//Check to see if there are any avalanches
		while (pixel_array.size() > 0) {
			avalanch(pixel_array[0]);
			avalanchCount++;
		}

		//Print the data and output to a CSV file when 1 or more avalanches occur
		if (avalanchCount != 0){
			printf("Avalanche Count: %d\nTotal Sand: %d\nSeverity: %f\n\n", avalanchCount, total_Sand, (double)avalanchCount / (double)total_Sand, total_Sand);
			/*out << (double)avalanchCount / (double)total_Sand << "," << avalanchCount << "," << total_Sand << "\n";*/
		}
		cc++;
	/*}*/
	
		if (total_Sand % 1000 == 0) {
			glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels_next);
		}

}


//-------------------------------------------------------------------------
//  This function sets the window x and y coordinates
//  such that the window becomes centered
//-------------------------------------------------------------------------
void centerOnScreen()
{
	window_x = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
	window_y = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
}