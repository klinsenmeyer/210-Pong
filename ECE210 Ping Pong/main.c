//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include "lab_buttons.h"

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
#define PADDLE_X1 LCD_SIZE_X-PADDLE_WIDTH
#define PADDLE_MAX_Y LCD_SIZE_Y-PADDLE_HEIGHT
#define PADDLE_MIN_Y 0
#define PADDLE_INIT_Y (PADDLE_MAX_Y-PADDLE_MIN_Y)/2



//ece210_lcd_draw_rectangle  (uint16_t x_start, uint16_t x_len, uint16_t y_start, uint16_t y_len, uint16_t color);

/******************************************************************************
 * Main
 *****************************************************************************/
int 
main(void)
{
	uint8_t btn_dir;
	bool start = 0;
	
	uint16_t ball_x = BALL_INIT_X;	
  uint16_t ball_y = BALL_INIT_Y;
	int x_inc = 1;
	int y_inc = 1;
	
  
  ece210_initialize_board();
  ece210_lcd_add_msg("WELCOME TO PONG\nPRESS UP TO START\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	
	while(start == 0) {start = btn_up_pressed();}
	
	while(1){
		
		// Check if game over
		
		
		// Check if hit wall
		
		
		// Increment ball position

		// Read Joystick Position and Move Paddle 0
		btn_dir = ece210_ps2_read_position();		
		switch (btn_dir)											
    {
      case UP_BUTTON:
      {
        
        break;
      }
      case DOWN_BUTTON:
      {
        
        break;
      }
      default:
      {
        break;
      }
		}
		
		// Read Buttons and Move Paddle 1
	}