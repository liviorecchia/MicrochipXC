#define LCD16x2SET_TRIS    0x01
#define LCD16x2SET_LAT     0x02
#define LCD16x2GET_PORT    0x03
#define LCD16x2GET_BUT     0x05
#define LCD16x2GET_ID      0x20
#define LCD16x2GET_FRM     0x21
#define LCD16x2LCD_CLR     0x60
#define LCD16x2LCD_WR      0x61
#define LCD16x2SET_BL      0x62
#define LCD16x2UART_EN     0x10

#define LCD16x2BOARD_ID    0x65
#define LCD16x2ADDRESS     0x30

void LCD16x2lcdSetBacklight(char);
void LCD16x2uartEnable(char);
void LCD16x2pinMode(char, char);
void LCD16x2digitalWrite(char, char);
void LCD16x2lcdClear();
char strlen(char*);
void LCD16x2lcdWrite(char*);
void LCD16x2lcdGoToxy(char, char);
void LCD16x2putChar(char);
char LCD16x2getID();
char LCD16x2digitalRead(char);