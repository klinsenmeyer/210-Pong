//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
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
	
#define LCD_SIZE_X			240				
#define LCD_SIZE_Y			320		
#define LCD_HALF_SIZE_X	LCD_SIZE_X/2	
#define LCD_HALF_SIZE_Y	LCD_SIZE_Y/2
	
#define	BALL_INIT_X			LCD_HALF_SIZE_X 	
#define	BALL_INIT_Y			LCD_HALF_SIZE_Y
#define BALL_RADIUS 10

#define PADDLE_LENGTH 80
#define PADDLE_WIDTH 3
#define PADDLE_X0 PADDLE_WIDTH
#define PADDLE_X1 LCD_SIZE_X
#define PADDLE_MAX_Y LCD_SIZE_Y-PADDLE_LENGTH
#define PADDLE_MIN_Y 0
#define PADDLE_INIT_Y (PADDLE_MAX_Y-PADDLE_MIN_Y)/2

/******************************************************************************
 * Main
 *****************************************************************************/
int 
main(void)
{
	uint8_t btn_dir;
	uint8_t ps2_dir;
	bool start = 0;
	uint8_t score0 = 0;
	uint8_t score1 = 0;
	
	uint16_t ball_x = BALL_INIT_X;	
  uint16_t ball_y = BALL_INIT_Y;
	uint16_t paddle_y0 = PADDLE_MIN_Y;
	uint16_t paddle_y1 = PADDLE_MIN_Y;
	int x_inc = 1;
	int y_inc = 1;
	
	ece210_initialize_board();
  ece210_lcd_add_msg("WELCOME TO PONG\nPRESS UP TO START\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	
	ece210_lcd_draw_rectangle (PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_RED);
	ece210_lcd_draw_rectangle (PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_RED);
	ece210_lcd_draw_circle(ball_x, ball_y, BALL_RADIUS, LCD_COLOR_YELLOW);
	
	//while(start == 0) {start = btn_up_pressed();}
	
	while(1){
		
		// Increment ball position
		ece210_lcd_draw_circle(ball_x, ball_y, BALL_RADIUS, LCD_COLOR_BLACK);
		ball_x = ball_x + x_inc;
		ball_y = ball_y + y_inc;
		ece210_lcd_draw_circle(ball_x, ball_y, BALL_RADIUS, LCD_COLOR_YELLOW);
		
		// Check if hit paddle or scored
		if (abs(ball_x - PADDLE_X1) <= BALL_RADIUS && ball_y >= paddle_y1 && ball_y <= paddle_y1 + PADDLE_LENGTH)
		{
			x_inc = -x_inc;
		}
		else if (abs(ball_x - LCD_SIZE_X) <= BALL_RADIUS)
		{
			score0 = score0 + 1;
			ball_x = BALL_INIT_X;
			ball_y = BALL_INIT_Y;
		}
		if (abs(ball_x - PADDLE_X0) <= BALL_RADIUS && ball_y >= paddle_y0 && ball_y <= paddle_y0 + PADDLE_LENGTH)
		{
			x_inc = -x_inc;
		}
		else if (ball_x <= BALL_RADIUS)
		{
			score1 = score1 + 1;
			ball_x = BALL_INIT_X;
			ball_y = BALL_INIT_Y;
		}
		
		// Check if hit wall
		if (ball_y <= BALL_RADIUS || abs(ball_y - LCD_SIZE_Y))
		{
			ball_y = -ball_y;
		}

		// Read Joystick Position and Move Paddle 0
		ps2_dir = ece210_ps2_read_position();		
		switch (ps2_dir)											
    {
      case PS2_UP:
      {
				ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_BLACK);
        if (paddle_y0 < PADDLE_MAX_Y)
				{
					paddle_y0 = paddle_y0 + 1;
				}
				ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_RED);
        break;
      }
      case PS2_DOWN:
      {
				ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_BLACK);
        if (paddle_y0 > 0)
				{
					paddle_y0 = paddle_y0 - 1;
				}
				ece210_lcd_draw_rectangle(PADDLE_X0, PADDLE_WIDTH, paddle_y0, PADDLE_LENGTH, LCD_COLOR_RED);
        break;
      }
      default:
      {
        break;
      }
		}
		
		// Read Buttons and Move Paddle 1
		if (btn_up_pressed())
		{
			ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_BLACK);
			if (paddle_y1 < PADDLE_MAX_Y)
			{
				paddle_y1 = paddle_y1 + 1;
			}
			ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_RED);
		}
		else if (btn_down_pressed())
		{
			ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_BLACK);
			if (paddle_y1 > 0)
			{
				paddle_y1 = paddle_y1 - 1;
			}
			ece210_lcd_draw_rectangle(PADDLE_X1, PADDLE_WIDTH, paddle_y1, PADDLE_LENGTH, LCD_COLOR_RED);
		}

	}
}