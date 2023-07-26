//Belinda Truong 3/28/2023
//Bird: Apply gravity, pushbutton 1=Up 0=Down
//Background: Stays the same, darker every level

//Due: April 11 11:50 AM

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int timeout;	//global timeout varibale for synchronization

volatile int pixel_buffer_start = FPGA_ONCHIP_BASE;
//volatile int *key_ptr = (int*)KEY_BASE;

int resolution_x = 320;		//max x
int resolution_y = 240;		//max y

//function prototypes
void clrScreen();	//reset screen
void bird(int, int, int, int, short);	//x1 y1, x2 y2, color

int main()
{
	volatile int* interval_timer_ptr = (int*)TIMER_BASE; //Timer
	timeout = 0;
	//timer
	int counter = 0x9600;		//approx 200ms
	*(interval_timer_ptr + 2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 3) = (counter >> 16) & 0xFFFF;
	*(interval_timer_ptr + 1) = 0x7;

	volatile int* key_ptr = (int*)KEY_BASE;

	NIOS2_WRITE_IENABLE(0x1);
	NIOS2_WRITE_STATUS(1);

	clrScreen();		//Clearing any previous code/animation

	//Variables
	int x1, y1;			//inital position
	int x2, y2;			//final position
	int inc_x, inc_y;	//increment in the x & y direction
	int key;			//pushbutton

	//increment by 1
	inc_x = 1;	
	inc_y = 2;

	//Setting the position of the bird/setting the thicccccc bird
	x1 = 0;
	x2 = 14;

	y1 = 120;
	y2 = 134;

	bird(x1, y1, x2, y2, 0xFF00);	//Draws the yellow bird in the OG position

	while (1)
	{
		while (!timeout);	//while timeout == 0, do nothing
		//if key = 1, bird go up; otherwise key = 0, bird go down

		key = *(key_ptr);	//reads if user pressed the button or not
		
		x1 += inc_x;		//Right
		x2 += inc_x;		

			if (key > 0)		//if key = 1,2,3, Up
			{
				y1 -= inc_y;	//Up
				y2 -= inc_y;	

				//If bird reach boundary, go opposit direction
				if ((x2 == 319) || (x1 == 0))
				{
					inc_x = -(inc_x);
				}
				/*
				//GAMEOVER
				if ((y2 == 239)||(y1 == 0))		//if ((y2 == tubey2) || (y1 ==tubey1))
				{
					inc_y = -(inc_y);
				}*/
				key = *(key_ptr);
			}
			else	//Down
			{
				y1 += inc_y;
				y2 += inc_y;

				//If bird reach boundary, go opposit direction
				if ((x2 == 319) || (x1 == 0))
				{
					inc_x = -(inc_x);
				}
				/*
				//GAMEOVER
				if ((y2 == 239)||(y1 == 0))
				{
					inc_y = -(inc_y);
				}*/
				key = *(key_ptr);
			}
		
		//Redraws bird w/new position & Background (Function Calls)
		clrScreen();
		bird(x1, y1, x2, y2, 0xFF00);
		timeout = 0;
	}
	return 0;
}

//function stuff
//Draw Bird
void bird(int x1, int y1, int x2, int y2, short pixel_color)
{
	int pixel_ptr, row, col;

	for (row = y1; row <= y2; row++)
	{
		for (col = x1; col <= x2; col++)
		{
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short*)(pixel_ptr) = pixel_color;
		}
	}
}

//Clear Screen
void clrScreen()
{
	int clearColor = 0x187F;

	int pixel_ptr, row, col;

	for (row = 0; row < resolution_y; row++)
	{
		for (col = 0; col < resolution_x; col++)
		{
			pixel_ptr = pixel_buffer_start + (row << 10) + (col << 1);
			*(short*)pixel_ptr = clearColor;
		}
	}
}