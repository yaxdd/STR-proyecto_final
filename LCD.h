#define E  0x02 // on PA7
#define RS 0x01 // on PA6
extern void LCD_Init(void);
extern void LCD_OutChar(unsigned char letter);
extern void LCD_Clear(void);
void LCD_OutString(char *pt);
extern void LCD_OutUDec(uint32_t);
extern void LCD_OutUHex(uint32_t);
extern void LCD_OutUTime(uint32_t);
extern void LCD_ReturnHome(void);
extern void LCD_setCursor(int,int);
extern void OutCmd(unsigned char);