#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

#include "KS0108.h"
#include "font32x48-numerals.h"

#include "skull.h"

unsigned char hours_tens;
unsigned char hours_ones;
unsigned char minutes_tens;
unsigned char minutes_ones;
unsigned char seconds_tens;
unsigned char seconds_ones;

void WriteBigChar(char num, char x_start, char y_start);
void ShowBigTime(char hten, char hone, char mten, char mone);
void IncSecond(void);

void Delay_ms(int cnt) {
	while (cnt-->0) {
		_delay_ms(1);
	}
}

int main(void)
{
  hours_tens = 1;
  hours_ones = 3;
  minutes_tens = 3;
  minutes_ones = 6;
  seconds_tens = 4;
  seconds_ones = 9;
	
  GLCD_Initalize();
  GLCD_ClearScreen();

/*//Draw the Hackaday Logo:
  GLCD_GoTo(0,0);
  for (unsigned char j=0; j<8; j++)
  {
    GLCD_GoTo(0,j);
    for (unsigned char i=0; i<64; i++)
    {
      GLCD_WriteData(skull[i+(j*64)]);
    }
  }
*/

  while(1)
  {
    //FIXME: Optimize so we don't redraw the entire screen every second.
    ShowBigTime(hours_tens, hours_ones, minutes_tens, minutes_ones);
    GLCD_GoTo(0,7);

    //Easter Egg:
    if ((hours_tens == 1) && (hours_ones == 3) && (minutes_tens == 3) && (minutes_ones == 7))
    {
      //Display Hackaday URL when time is 13:37
      GLCD_WriteString(" http://hackaday.com ");
      
    }
    else
    {
      //Display Day, Date, and Seconds
      GLCD_WriteString("Tuesday 12/3/2013  ");
      GLCD_WriteChar(seconds_tens+48); //ASCII offset
      GLCD_WriteChar(seconds_ones+48); //ASCII offset
    }
    IncSecond();

    //FIXME: This should be a hardware timer for accuracy
    //FIXME: This should use an RTC for accuracy
    Delay_ms(1000);
    
    
  }
}

void IncSecond(void)
{
  if (++seconds_ones > 9)
  {
    seconds_ones = 0;
    if (++seconds_tens > 5)
    {
      seconds_tens = 0;
  if (++minutes_ones > 9)
  {
    minutes_ones = 0;
    if (++minutes_tens > 5)
    {
      minutes_tens = 0;
      if ((hours_ones == 9) && (hours_tens < 2))
      {
	// gone past 09:59 or gone past 19:59
	hours_ones = 0;
	++hours_tens;
      }
      else if ((hours_ones == 3) && (hours_tens == 2))
      {
	// gone past 23:59
	hours_ones = 0;
	hours_tens = 0;
      }
      else 
      {
	// increment won't cause us to "go round the bend"
	++hours_ones;
      }
    }
  }
  }
  }
}
void ShowBigTime(char hten, char hone, char mten, char mone)
{
  WriteBigChar(hten,0,0);
  WriteBigChar(hone,32,0);
  WriteBigChar(mten,64,0);
  WriteBigChar(mone,96,0);
  GLCD_GoTo(63,4);
  GLCD_WriteData(0b0001100);      
  GLCD_WriteData(0b0001100);
  GLCD_GoTo(63,2);
  GLCD_WriteData(0b0001100);      
  GLCD_WriteData(0b0001100);
}

void WriteBigChar(char num, char x_start, char y_start)
{
  for (char i = 0; i < 6; i++)
  {
    GLCD_GoTo(x_start,(y_start + i));
    for (char col_marker = 0; col_marker < 32; col_marker++)
    {
      GLCD_WriteData(
        GLCD_ReadByteFromROMMemory(
                                  (char *)((int)big_font + ((col_marker * 6) + (5 - i) + (num * 192)))
        )
      );
    }

  }
}
