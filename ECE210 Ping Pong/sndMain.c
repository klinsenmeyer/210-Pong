//*****************************************************************************
// main.c
// Author: klinsenmeyer@wisc.edu
// Modified: Kai
//*****************************************************************************

#include "lab_buttons.h"
#undef DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#define LOCAL_ID     0x01
#define REMOTE_ID    0x00


/******************************************************************************
* Global Constants and Variables
*****************************************************************************/

#define LCD_SIZE_X 240 // X Size of LCD screen in pixels
#define LCD_SIZE_Y 320 // Y Size of LCD screen in pixels

//*****************************************************************************
//*****************************************************************************
int main(void)
{
	ece210_initialize_board();
	ece210_wireless_init(LOCAL_ID, REMOTE_ID);
	
	char msg[80];
	int score0 = 0;
	int score1 = 0;

	uint32_t rx_data;

	sprintf(msg, "Score: \n\r%d %d", score0, score1);
	ece210_lcd_add_msg(msg ,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);

	while(true)
	{

		if(ece210_wireless_data_avaiable())
		{

			rx_data = ece210_wireless_get();

			if(rx_data == 0)
			{
				score0 = score0 + 1;
				
				if (score0 == 5)
				{
					ece210_lcd_add_msg("PLAYER 2 WINS\n\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_RED);
					break;
				}
			}

			else if(rx_data == 1)
			{
				score1 = score1 + 1; 

				if(score1 == 5)
				{
					ece210_lcd_add_msg("PLAYER 1 WINS\n\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_RED);
					break;
				}
			}
				sprintf(msg,"Score: \n\r%d %d",score0, score1);
				ece210_lcd_add_msg(msg ,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);

		}

	}

}