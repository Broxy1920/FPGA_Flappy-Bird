
//include needed headers
#include<string.h>
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int timeout;	//global timeout varibale for synchronization

int y_dir;	//set it to 1 to move the line up one pixel at a time (y direction)

//initalize pointers
int num[10] = {0x000000BF, 0x000000006, 0x0000005B, 0x0000004F, 0x00000066, 0x0000006D, 0x0000007D, 0x00000007, 0x0000007F, 0x0000006F};
//			     	0			1			2			3			w4			5			6			7			w8			9


int resolution_x = 320;		//max x
int resolution_y = 240;		//max y
char levels[10] = {0x213232};		//level numbers in HEX to display on 7-seg board

//Functions
void clearScreen();

void drawBox(int, int, int, int, short);

void VGA_text(int x, int y, char* text_ptr);

//Belinda's Code Start

volatile int pixel_buffer_start = FPGA_ONCHIP_BASE;

void bird(int, int, int, int, short);	//x1 y1, x2, y2, color

//Belinda's Code End

int main()
{
	clearScreen();
	volatile int * interval_timer_ptr = (int *) TIMER_BASE;
	volatile int* Hex3_0 = (int*)HEX3_HEX0_BASE;
	
	timeout = 0;

	//high score variables
	int hx1, hx2, hy1, hy2;
	//end of highscore variables
	
	int x1, x2, y1, y2, bx1, bx2, by1, by2, tx1, tx2, ty1, ty2, tbx1, tbx2, tby1, tby2;		//Tube 1 variabes
	int sx1, sx2, sy1, sy2, sbx1, sbx2, sby1, sby2, stx1, stx2, sty1, sty2, stbx1, stbx2, stby1, stby2;		//Tube 2 variabes
	int inc_y, sinc_y;	//first and second tube increments
	int level;	//level system

	y_dir = 1;			//increment by 1

	int counter = 0x960000;		//approx 200ms
	*(interval_timer_ptr + 2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 3) = (counter >> 16) & 0xFFFF;
	*(interval_timer_ptr + 1) = 0x7;
	
	NIOS2_WRITE_IENABLE(0x1);
	NIOS2_WRITE_STATUS(1);
	
	//Belinda's Start Code

	volatile int* key_ptr = (int*)KEY_BASE;

	//Variables
	int bdx1, bdy1;			//initial position
	int bdx2, bdy2;			//final postion
	int inc_bx, inc_by;		//increment in the x & y direction
	int key;				//pushbutton

	//increment
	inc_bx = 2;
	inc_by = 2;

	//Setting the thicc bird
	bdx1 = 0;
	bdx2 = 14;

	bdy1 = 120;
	bdy2 = 134;
	
	//Belinda's End Code


	clearScreen();

	//Tube 1
	
	x1 = 70;		//Coordinates for top tube
	x2 = 110;		
	y1 = -100;
	y2 = 160;

	tx1 = 64;		//Coordinates for tip of the top tube
	tx2 = 116;		
	ty1 = 160;
	ty2 = 170;

	tbx1 = 64;		//Coordinates for tip of the bottom tube
	tbx2 = 116;		
	tby1 = 200;
	tby2 = 210;
	
	bx1 = 70;		//Coordinates for bottom tube
	bx2 = 110;
	by1 = 208;
	by2 = 400;



	//Tube 2

	sx1 = 200;		//Coordinates for top tube
	sx2 = 240;
	sy1 = -200;
	sy2 = 60;

	stx1 = 194;		//Coordinates for tip of the top tube
	stx2 = 246;
	sty1 = 60;
	sty2 = 70;

	stbx1 = 194;		//Coordinates for tip of the bottom tube
	stbx2 = 246;
	stby1 = 100;
	stby2 = 110;

	sbx1 = 200;		//Coordinates for bottom tube
	sbx2 = 240;
	sby1 = 108;
	sby2 = 300;



	//Tube 1 incrementor
	inc_y = -1;	//move accross one at a time

	//Tube 1 incrementor
	sinc_y = 1;	//move accross one at a time


	//Drawing Tube 1
	drawBox(x1, y1, x2, y2, 0x0812);
	drawBox(tx1, ty1, tx2, ty2, 0x0A0F);
	drawBox(tbx1, tby1, tbx2, tby2, 0x0A0F);
	drawBox(bx1, by1, bx2, by2, 0x0812);

	//Drawing Tube 2
	drawBox(sx1, sy1, sx2, sy2, 0x0812);
	drawBox(stx1, sty1, stx2, sty2, 0x0A0F);
	drawBox(stbx1, stby1, stbx2, stby2, 0x0A0F);
	drawBox(sbx1, sby1, sbx2, sby2, 0x0812);

	//Belinda's Birdy :D

	bird(bdx1, bdy1, bdx2, bdy1, 0xFF00);		//Bird :D

	
	//Setting level on DE10 board
	level = 0;
	*(Hex3_0) = num[level];

	//Highscore Section
	char gameOver[40] = "Games Over!";
	char ptrs[40] = "Scoreboard";
	char line[40] = "----------------";
	char ptrs1[40] = "Marisa 7";
	char ptrs3[40] = "Alexandra 2";
	char ptrs2[40] = "Belinda 5";
	char ptrs4[40] = "New Player ";
	char lev[40];

	//End of Highscore Section

	while(1)
	{
		while(!timeout);	//barrier, while timeout == 0, do nothing
		//eventaully, timer gets to 0, send out interrupt
		//calls ISR which sets timeout = 1 so you can move past the barrier


		//moving Tube 1
		y1 += inc_y;
		y2 += inc_y;
		ty1 += inc_y;
		ty2 += inc_y;
		tby1 += inc_y;
		tby2 += inc_y;
		by1 += inc_y;
		by2 += inc_y;

		//moving Tube 2
		sy1 += sinc_y;
		sy2 += sinc_y;
		sty1 += sinc_y;
		sty2 += sinc_y;
		stby1 += sinc_y;
		stby2 += sinc_y;
		sby1 += sinc_y;
		sby2 += sinc_y;

		//if you get to a boundary
		if (	(y2 - 10 <= 0) || (by1 + 10 >= 240)		)
		{
			inc_y = -(inc_y);		//change direction of 1st Tube
		}
		if (	(sy2 - 10 <= 0) || (sby1 + 10 >= 240)	)
		{
			sinc_y = -(sinc_y);		//change direction of 2st Tube
		}


		//Belinda's Code Begin

		key = *(key_ptr);	//Read if user presses the button

		//Bird Move Right
		bdx1 += inc_bx;
		bdx2 += inc_bx;

		if (key > 0)		//if key = 1, 2, 3 UP
		{
			bdy1 -= inc_by;	//Up
			bdy2 -= inc_by;

			key = *(key_ptr);
			*(Hex3_0) = num[level];
		}
		else
		{
			bdy1 += inc_by;	//Down
			bdy2 += inc_by;
		}
		if ((bdx2 >= 320) || (bdx1 <= 0))	//If bird reach bounds, go opposite direction
		{
			inc_bx = -(inc_bx);
		}
		//If bird comes back to the left side, that is a level
		if (bdx1 <= 0) {
			level++;
			if (inc_y < 0) {
				inc_y -= 1;
			}
			else {
				inc_y += 1;
			}
			if (sinc_y < 0) {
				sinc_y -= 1;
			}
			else {
				sinc_y += 1;
			}
		}

		//Belinda's End of Code :)

		clearScreen();
		*(Hex3_0) = num[level];

		//Drawing Tube 1
		drawBox(x1, y1, x2, y2, 0x0812);
		drawBox(tx1, ty1, tx2, ty2, 0x0A0F);
		drawBox(tbx1, tby1, tbx2, tby2, 0x0A0F);
		drawBox(bx1, by1, bx2, by2, 0x0812);

		//Drawing Tube 2
		drawBox(sx1, sy1, sx2, sy2, 0x0812);
		drawBox(stx1, sty1, stx2, sty2, 0x0A0F);
		drawBox(stbx1, stby1, stbx2, stby2, 0x0A0F);
		drawBox(sbx1, sby1, sbx2, sby2, 0x0812);
		
		//Draw Bird
		bird(bdx1, bdy1, bdx2, bdy2, 0xFF00);

		//Bird - Tube Collision
		//Bird touches ground
		if (bdy2 >= 240 || level > 9) {
			clearScreen();
			char* a = itoa(level, lev, 10);
			//scoreboard screen
			drawBox(120, 30, 200, 210, 0x0000);
			
			VGA_text(34, 15, gameOver);

			//Checking if new player beat any of the scores already on the scoreboard
			if (level > 2) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 35, ptrs4);
			}
			else if (level > 5) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 30, ptrs4);
			}
			else if (level > 7) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 30, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 25, ptrs4);
			}
			else {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 35, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 40, ptrs4);
			}
			//end of highscore screen
			return 0;
		}
		//while Bird is in between Tube 1 x-values AND NOT between the top Tube1 and bottom Tube 1 (not in the safe space)
		else if (	((bdx1 >= tx1 && bdx1 <= tx2) || (bdx2 >= tx1 && bdx2 <= tx2))	&&	!(	(bdy1 >= ty2 && bdy1 <= tby1)	&& (bdy2 >= ty2 && bdy2 <= tby1)	)		) {
			//highscore screen
			clearScreen();
			char* a = itoa(level, lev, 10);
			drawBox(120, 30, 200, 210, 0x0000);
			VGA_text(34, 15, gameOver);
			//Checking if new player beat any of the scores already on the scoreboard
			if (level > 2) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 35, ptrs4);
			}
			else if (level > 5) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 30, ptrs4);
			}
			else if (level > 7) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 30, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 25, ptrs4);
			}
			else {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 35, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 40, ptrs4);
			}
			//end of highscore screen
			return 0;
		}
		//while Bird is in between Tube 2 x-values AND NOT between the top Tube1 and bottom Tube 2 (not in the safe space)
		else if (((bdx1 >= stx1 && bdx1 <= stx2) || (bdx2 >= stx1 && bdx2 <= stx2)) && !((bdy1 >= sty2 && bdy1 <= stby1) && (bdy2 >= sty2 && bdy2 <= stby1))) {
			//highscore screen
			clearScreen();
			char* a = itoa(level, lev, 10);
			drawBox(120, 30, 200, 210, 0x0000);
			VGA_text(34, 15, gameOver);
			//Checking if new player beat any of the scores already on the scoreboard
			if (level > 2) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 35, ptrs4);
			}
			else if (level > 5) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 30, ptrs4);
			}
			else if (level > 7) {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 30, ptrs1);
				VGA_text(34, 40, ptrs2);
				VGA_text(34, 40, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 25, ptrs4);
			}
			else {
				VGA_text(34, 20, ptrs);
				VGA_text(31, 22, line);
				VGA_text(34, 25, ptrs1);
				VGA_text(34, 30, ptrs2);
				VGA_text(34, 35, ptrs3);
				strcat(ptrs4, lev);
				VGA_text(34, 40, ptrs4);
			}
			//end of highscore screen
			return 0;
		}

		timeout = 0;		//DO NOT FORGET TO RESET THIS
		
	}
	return 0;
}
//Clear screen
void clearScreen()
{
	int clearColor = 0x187F;	//Blue Sky
	
	int pixel_ptr, row, col;
	
	for(row = 0; row < resolution_y; row++)
	{
		for(col = 0; col < resolution_x; col++)
		{
			pixel_ptr = pixel_buffer_start + (row << 10) + (col << 1);
			*(short *)pixel_ptr = clearColor;
		}
	}
	//Clears the scoreboard screen
	VGA_text(34, 15, "                                ");
	VGA_text(34, 20, "                                ");
	VGA_text(31, 22, "                                ");
	VGA_text(34, 25, "                                ");
	VGA_text(34, 30, "                                ");
	VGA_text(34, 35, "                                ");
	VGA_text(34, 40, "                                ");

}

//Tubes
void drawBox(int x1, int y1, int x2, int y2, short pixel_color) {
	int pixel_ptr, row, col;

	for (row = y1; row <= y2; row++) {
		for (col = x1; col <= x2; col++) {
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short*)(pixel_ptr) = pixel_color;
		}
	}
}

//Bird
void bird(int x1, int y1, int x2, int y2, short pixel_color) {
	int pixel_ptr, row, col;

	for (row = y1; row <= y2; row++) {
		for (col = x1; col <= x2; col++) {
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short*)(pixel_ptr) = pixel_color;
		}
	}
}

//Scoreboard Text
void VGA_text(int x, int y, char* text_ptr)
{
	int offset;
	volatile char* character_buffer = (char*)FPGA_CHAR_BASE;

	offset = (y << 7) + x;

	while (*(text_ptr))
	{
		*(character_buffer + offset) = *(text_ptr);
		++text_ptr;
		++offset;
	}
}