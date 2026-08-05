//major_lcd_defines.h
//lcd_defines.h
#define LCD_DATA 8
#define RW 18
#define RS 16
#define EN 17
//LCD COMMANDS
#define CLEAR_LCD 0x01
#define RET_CUR_HOME 0X02
#define DISP_OFF 0X08
#define DISP_ON 0X0C
#define DISP_ON_CUR_ON 0X0E
#define DISP_ON_CUR_ON_BLINK 0X0F
#define MODE_8BIT_1LINE 0X30
#define MODE_8BIT_2LINE 0X38
#define MODE_4BIT_1LINE 0X20
#define MODE_4BIT_2LINE 0X28
#define GOTO_LINE1_POS0 0X80
#define GOTO_LINE2_POS0 0XC0
#define GOTO_LINE3_POS0 0X94
#define GOTO_LINE4_POS0 0XD4
#define SHIFT_CUR_RIGHT 0X06
#define SHIFT_DISP_LEFT 0X10
#define SHIFT_DISP_RIGHT 0X14
#define GOTO_CGRAM 0X40
