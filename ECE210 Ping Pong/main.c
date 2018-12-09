//*****************************************************************************
// main.c
// Author: Mostafa and Kai
//*****************************************************************************
#include "lab_buttons.h"
#include <stdio.h>
#include <stdlib.h>

// Turn Debuggin off
#undef DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif
	
/******************************************************************************
 * Global Variables
 *****************************************************************************/

#define UP_BUTTON 0x01
#define DOWN_BUTTON 0x02

#define LOCAL_ID     0x00
#define REMOTE_ID    0x01
	
#define LCD_SIZE_X			239				
#define LCD_SIZE_Y			319		
#define LCD_HALF_SIZE_X	LCD_SIZE_X/2	
#define LCD_HALF_SIZE_Y	LCD_SIZE_Y/2
	
#define	BALL_INIT_X			LCD_HALF_SIZE_X 	
#define	BALL_INIT_Y			LCD_HALF_SIZE_Y
#define BALL_RADIUS 10

#define PADDLE_LENGTH 80
#define PADDLE_WIDTH 3
#define PADDLE_X0 0
#define PADDLE_X1 LCD_SIZE_X - PADDLE_WIDTH
#define PADDLE_INIT_Y (LCD_SIZE_Y - PADDLE_LENGTH)/2

#define PADDLE_SPEED 5
#define BALL_SPEED 2
#define BALL_GAIN 1.05

#define MAX_SCORE 5

/******************************************************************************
 * Increment Function
 *****************************************************************************/
// This function is used to prevent the paddle moving off the screen
int paddle(y)
{
	if (0 <= y && y <= LCD_SIZE_Y - PADDLE_LENGTH)
	{
		return y;
	}
	else if (y < 0)
	{
		return 0;
	}
	else
	{
		return LCD_SIZE_Y - PADDLE_LENGTH;
	}
}

/******************************************************************************
 * Main
 *****************************************************************************/
int 
main(void)
{
	uint8_t ps2_dir;
	uint8_t score0 = 0;
	uint8_t score1 = 0;
	
	uint16_t ball_x = BALL_INIT_X;	
  uint16_t ball_y = BALL_INIT_Y;
	uint16_t paddle_y0 = PADDLE_INIT_Y;
	uint16_t paddle_y1 = PADDLE_INIT_Y;
	
	uint16_t ball_x_old = ball_x;
	uint16_t ball_y_old = ball_y;
	uint16_t paddle_y0_old = paddle_y0;
	uint16_t paddle_y1_old = paddle_y1;
	
	int x_inc = BALL_SPEED;
	int y_inc = BALL_SPEED;
	
	ece210_initialize_board();
  ece210_lcd_add_msg("WELCOME TO PONG\rPRESS A BUTTON TO START\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	
	ece210_lcd_draw_rectangle (PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_RED);
	ece210_lcd_draw_rectangle (PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_RED);
	ece210_lcd_draw_circle(ball_x, ball_y, BALL_RADIUS, LCD_COLOR_YELLOW);
	
	while(!AlertButtons) {} // wait and clear message
	ece210_lcd_draw_rectangle (0, LCD_SIZE_X, 0, 40, LCD_COLOR_BLACK);
		
	ece210_wireless_init(LOCAL_ID, REMOTE_ID);
		
	while(score0 < MAX_SCORE && score1 < MAX_SCORE){
		
		// Save old parameters
		ball_x_old = ball_x;
		ball_y_old = ball_y;
		paddle_y0_old = paddle_y0;
		paddle_y1_old = paddle_y1;
		
		// Move ball
		ball_x = ball_x + x_inc;
		ball_y = ball_y + y_inc;
		
		// Check if hit paddle or scored
		if (abs(ball_x - PADDLE_X1) <= BALL_RADIUS + PADDLE_WIDTH && ball_y >= paddle_y1 && ball_y <= paddle_y1 + PADDLE_LENGTH)
		{
			x_inc = -BALL_GAIN*x_inc;
		}
		else if (abs(ball_x - LCD_SIZE_X) <= BALL_RADIUS)
		{
			score0 = score0 + 1;
			ball_x = BALL_INIT_X;
			ball_y = BALL_INIT_Y;
			ece210_wireless_send(0);
		}
		if (abs(ball_x - PADDLE_X0) <= BALL_RADIUS + PADDLE_WIDTH && ball_y >= paddle_y0 && ball_y <= paddle_y0 + PADDLE_LENGTH)
		{
			x_inc = -BALL_GAIN*x_inc;
		}
		else if (ball_x <= BALL_RADIUS)
		{
			score1 = score1 + 1;
			ball_x = BALL_INIT_X;
			ball_y = BALL_INIT_Y;
			ece210_wireless_send(1);
		}
		
		// Check if hit wall
		if (ball_y <= BALL_RADIUS || abs(ball_y - LCD_SIZE_Y) <= BALL_RADIUS)
		{
			y_inc = -y_inc;
		}

		// Read Joystick Position and Move Paddle 0
		ps2_dir = ece210_ps2_read_position();
		if (ps2_dir == PS2_DOWN)
		{
      paddle_y0 = paddle(paddle_y0 + PADDLE_SPEED);
		}
		else if (ps2_dir == PS2_UP)
		{
			paddle_y0 = paddle(paddle_y0 - PADDLE_SPEED);
		}
		
		// Read Buttons and Move Paddle 1
		if (AlertButtons)
		{
			AlertButtons;
			if (btn_down_pressed())
			{
				paddle_y1 = paddle(paddle_y1 + PADDLE_SPEED);
			}
			else if (btn_up_pressed())
			{
				paddle_y1 = paddle(paddle_y1 - PADDLE_SPEED);
			}
		}
		
		// Update positions
		ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_RED);
		ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_RED);
		ece210_lcd_draw_circle(ball_x, ball_y, BALL_RADIUS, LCD_COLOR_YELLOW);
		
		ece210_wait_mSec(10);
		
		ece210_lcd_draw_circle(ball_x_old, ball_y_old, BALL_RADIUS, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0_old, PADDLE_LENGTH, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1_old, PADDLE_LENGTH, LCD_COLOR_BLACK);

	}

}