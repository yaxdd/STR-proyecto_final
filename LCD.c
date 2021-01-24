/*
  size is 1*16
  if do not need to read busy, then you can tie R/W=ground
  ground = pin 1    Vss
  power  = pin 2    Vdd   +3.3V or +5V depending on the device
  ground = pin 3    Vlc   grounded for highest contrast
  PM2    = pin 4    RS    (1 for data, 0 for control/status)
  PM1		 = pin 5    R/W   (1 for read, 0 for write)
  PM0    = pin 6    E     (enable)
  PK0    = pin 7    DB0   (8-bit data)
  PK1    = pin 8    DB1
  PK2    = pin 9    DB2
  PK3    = pin 10   DB3
  PK4    = pin 11   DB4
  PK5    = pin 12   DB5
  PK6    = pin 13   DB6
  PK7    = pin 14   DB7
16 characters are configured as 1 row of 16
addr  00 01 02 03 04 05 ... 0F
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "LCD.h"
#include <TM4C129.h>
//#include "SYSTICK.h"

//INCIALIZACION DE LA LCD
// Puertos K y M
//Pines K0-K7,M0-M2
void DefDelayUs(int ms)
{
    volatile int i;
    for (i = 0; i < ms; i++)
		__NOP();
//        for (j = 0; j <= 1; j++) {}   // // Do nothing for 1ms
}
void SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}
int DelayUs(uint32_t ui32DelayUs)
{
 ui32DelayUs*=5;
	// Limit the delay to max. 1 seconds.
    if (ui32DelayUs > 1e7) ui32DelayUs = 1e7;
    // CAUTION: Calling SysCtlDelay(0) will hang the system.
    if (ui32DelayUs > 0)
        // Note: The SysCtlDelay executes a simple 3 instruction cycle loop.
        SysCtlDelay((SystemCoreClock / 3e6) * ui32DelayUs);

    return 0;
}
void OutCmd(unsigned char command){
	GPIOK->DATA = command;
  GPIOM->DATA = 0x00;           // E=0, R/W=0, RS=0
	DelayUs(6);
  GPIOM->DATA = 0x01;           // E=1, R/W=0, RS=0
	DelayUs(6);
  GPIOM->DATA = 0x00;           // E=0, R/W=0, RS=0
	DelayUs(6);
}
void LCD_Init(){
	//	//INICIALIZACION DE PINES LCD
	SYSCTL->RCGCGPIO |= (1UL << 9) | (1UL <<11);
	// esperamos a que carguen los modulos
	GPIOK->DIR     |=  0xFF; 
  GPIOK->DEN     |=  0xFF; 
	GPIOK->DR8R |= 0xFF; 
	GPIOM->DIR|=0x07;
	GPIOM->DEN|=0x07;
	GPIOM->DR8R |= 0x07; 
	GPIOM->DATA =0x00;
	DelayUs(15000);
	OutCmd(0x30);         // command 0x30 = Wake up
	DelayUs(5000);   // must wait 5ms, busy flag not available
  OutCmd(0x30);         // command 0x30 = Wake up #2
  DelayUs(160);
  OutCmd(0x30);         // command 0x30 = Wake up #3
  DelayUs(160);
  OutCmd(0x38);         // Function set: 8-bit/2-line
  OutCmd(0x10);         // Set cursor
  OutCmd(0x0C);         // Display ON; Cursor ON
  OutCmd(0x06);         // Entry mode set
}
void LCD_Clear(void){
  OutCmd(0x01);          // Clear Display
  DelayUs(1600); // wait 1.6ms
  OutCmd(0x02);          // Cursor to home
  DelayUs(1600); // wait 1.6ms
}
void LCD_OutChar(unsigned char letter){
  GPIOK->DATA = letter;
  GPIOM->DATA= 0x04;          // E=0, R/W=0, RS=1
  DelayUs(6);
  GPIOM->DATA= 0x05;        // E=1, R/W=0, RS=1
  DelayUs(6);
  GPIOM->DATA= 0x04;          // E=0, R/W=0, RS=1
  DelayUs(40);
}

void LCD_OutString(char *pt){
	__NOP();
  while(*pt){
    LCD_OutChar(*pt);
    pt++;
  }
}

//-----------------------LCD_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void LCD_OutUDec(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    LCD_OutUDec(n/10);
    n = n%10;
  }
	
  LCD_OutChar(n+'0'); /* n is between 0 and 9 */
}
void LCD_OutUTime(uint32_t n){
	if (n<10){
		LCD_OutChar('0'); /* n is between 0 and 9 */
	}
	LCD_OutUDec(n);
}
//--------------------------LCD_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void LCD_OutUHex(uint32_t number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    LCD_OutUHex(number/0x10);
    LCD_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      LCD_OutChar(number+'0');
     }
    else{
      LCD_OutChar((number-0x0A)+'A');
    }
  }
}

void LCD_ReturnHome(){
  OutCmd(0x02);          // Cursor to home
  DelayUs(1600); // wait 1.6ms
}
void LCD_setCursor(int f, int k){
	OutCmd(0x02);          // Cursor to home
  DelayUs(1600); // wait 1.6ms
	for (int i = 0;i<(40*k)+f;i++){
		OutCmd(0x14);          // Cursor to home
		DelayUs(1600); // wait 1.6ms
	}
	
}