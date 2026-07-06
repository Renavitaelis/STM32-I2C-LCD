#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include <string.h>

//--- Which STM32 Boards It Is Ready To Function With, Add As Necessary

#if __has_include("stm32f1xx_hal.h")
	#include "stm32f1xx_hal.h"
#elif __has_include("stm32f4xx_hal.h")
	#include "stm32f4xx_hal.h"
#endif

//--- LCD Handler Struct & Defines

typedef enum
{
    LCD_Command,
    LCD_Text
} LCD_Type_Of_Data;

//--- Defines For Functions

//-- Modifies Expected Data

//- 0010 (M)(L)(F)0

#define LCD_Mod_Expected_Data 0x20 // 0010 0000

//- M
#define LCD_8bits_Mode 0x10 // 0001 0000
#define LCD_4bits_Mode 0x00 // 0000 0000

//- L
#define LCD_2Line_Mode 0x08 // 0000 1000
#define LCD_1Line_Mode 0x00 // 0000 0000

//- F
#define LCD_5x10_Font 0x04 // 0000 0100
#define LCD_5x8_Font 0x00 // 0000 0000

//-- Modifies User Interface Behavior

//- 0000 1(D)(C)(B)

#define LCD_Mod_UI 0x08 // 0000 1000

//- D
#define LCD_Display_On 0x04 // 0000 0100
#define LCD_Display_Off 0x00 // 0000 0000

//- C
#define LCD_Cursor_On 0x02 // 0000 0010
#define LCD_Cursor_Off 0x00 // 0000 0000

//- B
#define LCD_Blink_On 0x01 // 0000 0001
#define LCD_Blink_Off 0x00 // 0000 0000

//-- Modifies Entry Behavior

//- 0000 01(D)(S)

#define LCD_Mod_Entry 0x04 // 0000 0100

//- D
#define LCD_Cursor_Move_Right 0x02 // 0000 0010
#define LCD_Cursor_Move_Left 0x00 // 0000 0000

//- S
#define LCD_Display_Shift_Enable 0x01 // 0000 0001
#define LCD_Display_Shift_Disable 0x00 // 0000 0000

//-- Shifts Display ^ Cursor

//- 0001 (E)(D)XX

#define LCD_Shift_Control 0x10 // 0001 0000

//- E
#define LCD_Shift_Display 0x08 // 0000 1000
#define LCD_Shift_Cursor 0x00 // 0000 0000

//- D
#define LCD_Shift_Right 0x04 // 0000 0100
#define LCD_Shift_Left 0x00 // 0000 0000

//-- Control Bits (0000 (BL)(EN)(RW)(RS))

//- BL
#define LCD_BackLight_On   0x08		   // 0000 1000
#define LCD_BackLight_Off  0x00		   // 0000 0000

//- EN
#define LCD_EN_On 0x04 		   		   // 0000 0100
#define LCD_EN_Off 0x00		   		   // 0000 0000

//- RW
#define LCD_RW_On 0x02		   		   // 0000 0010
#define LCD_RW_Off 0x00		   		   // 0000 0000

//- RS
#define LCD_RS_On 0x01         		   // 0000 0001
#define LCD_RS_Off 0x00        		   // 0000 0000

//-- Others

#define LCD_Clear_Display 0x01 		   // 0000 0001

#define LCD_Return_Home 0x02		   // 0000 0010

#define LCD_Set_CGRAM_Address 0x40	   // 0100 0000

#define LCD_Set_DDRAM_Address 0x80	   // 1000 0000

#define LCD_Max_Internal_DDRAM_nColumns 40 // 0010 1000

#define LCD_SCROLL_FULL  LCD_Max_Internal_DDRAM_nColumns // 0010 1000
#define LCD_SCROLL_HALF  (LCD_Max_Internal_DDRAM_nColumns >> 1) // 0001 0100

#define LCD_CMD_DELAY() HAL_Delay(1)

//-- LCD Masks

#define LCD_Line_Mask   0x03 
#define LCD_Column_Mask 0xFC

#define LCD_Address_Mask 0xFE
#define LCD_Backlight_Mask 0x01

//-- LCD Handler Gets

#define LCD_Get_CurrentLine(pLCD)   ((pLCD)->cColsAndLines & LCD_Line_Mask)
#define LCD_Get_MaxLines(pLCD)      ((pLCD)->nColsAndLines & LCD_Line_Mask)

#define LCD_Get_CurrentColumn(pLCD) (((pLCD)->cColsAndLines & LCD_Column_Mask) >> 2)
#define LCD_Get_MaxColumns(pLCD)    (((pLCD)->nColsAndLines & LCD_Column_Mask) >> 2)

//-- LCD Handler Sets

#define LCD_Set_CurrentColumn(pLCD, Column) ((pLCD)->cColsAndLines = ((pLCD)->cColsAndLines & LCD_Line_Mask) | (((Column) << 2) & LCD_Column_Mask))
#define LCD_Set_CurrentLine(pLCD, Line)     ((pLCD)->cColsAndLines = ((pLCD)->cColsAndLines & LCD_Column_Mask) | ((Line) & LCD_Line_Mask))

#define LCD_Set_MaxColumns(pLCD, Columns)   ((pLCD)->nColsAndLines = ((pLCD)->nColsAndLines & LCD_Line_Mask) | (((Columns) << 2) & LCD_Column_Mask))
#define LCD_Set_MaxLines(pLCD, Lines)       ((pLCD)->nColsAndLines = ((pLCD)->nColsAndLines & LCD_Column_Mask) | ((Lines) & LCD_Line_Mask))

//--- LCD Handler Struct

typedef struct
{
    I2C_HandleTypeDef *STM_I2C_Handler;     // I2C Handler For Communication

    uint8_t nColsAndLines,				//Stores (nColumns) and (nLines) As 0xCCCCCCLL
    		cColsAndLines,				//Stores (CurrentColumn) and (CurrentLine) As 0xCCCCCCLL 
			AddressAndBl;            	//Stores (I2C Address Of The LCD) And (BackLightState) As 0xAAAAAAAB

}I2C_LCD_Handler;

//--- LCD Hardware Focused Functions

//- STM32 Electrical Translation Of Data To I2C
void LCD_Write_Bus(I2C_LCD_Handler *lcd, uint8_t *data, uint16_t size);

//- Used To Send (Commands ^ Text) To LCD
void LCD_Send(I2C_LCD_Handler *lcd, uint8_t CMD_Or_Txt, LCD_Type_Of_Data type);

void LCD_Set_Backlight(I2C_LCD_Handler *lcd, uint8_t mode);

void LCD_Set_Expected_Data(I2C_LCD_Handler *lcd, uint8_t Mode, uint8_t Lines, uint8_t Font);

void LCD_Set_UI(I2C_LCD_Handler *lcd, uint8_t Display, uint8_t Cursor, uint8_t Blink);

void LCD_Set_Entry_Behavior(I2C_LCD_Handler *lcd, uint8_t Direction, uint8_t Shift);

void LCD_Default_Init_Sequence(I2C_LCD_Handler *lcd);

//- Useful For Most Cases, Create More As Needed
void LCD_Default_Init(I2C_LCD_Handler *lcd, I2C_HandleTypeDef *STM_H_I2C, uint8_t nColumns, uint8_t nLines);

//--- User Focused Functions

//-- LCD Handler Sets

void LCD_Set_Pos(I2C_LCD_Handler *lcd, uint8_t column, uint8_t line);

//- LCD_Set_Pos(0,0) & Undoes Any Shifts Made On The Screen
void LCD_Set_Pos_Home(I2C_LCD_Handler *lcd);

void LCD_Set_Pos_Column(I2C_LCD_Handler *lcd, uint8_t column);

//- Sets Cursor Position To The Center Of A Given String Length
void LCD_Set_Pos_Centered(I2C_LCD_Handler *lcd, const char *str);

void LCD_Set_Pos_Line(I2C_LCD_Handler *lcd, uint8_t line);

//-- Writing Functions

void LCD_Write_Char(I2C_LCD_Handler *lcd, char ch);

void LCD_Write_String(I2C_LCD_Handler *lcd, const char *str);

//- Centralize & Writes A String
void LCD_Write_C_String(I2C_LCD_Handler *lcd, const char *str);

//- Converts Numbers Into Text For Exhibition
void LCD_Write_Number(I2C_LCD_Handler *lcd, double num, uint8_t DecimalPrecision);

//- Fills A Specific Line Starting At A Specific Column With A Specific Character
void LCD_Fill_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start, char ch);

//- Fills A Specific Line With A Specific Character
void LCD_Fill_Line(I2C_LCD_Handler *lcd, uint8_t line, char ch);

//- Fills The Entire Screen With A Specific Character
void LCD_Fill_All(I2C_LCD_Handler *lcd, char ch);

//- Clears A Line Starting At A Specific Column & Set_Pos(start,line)
void LCD_Clear_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start);

//- Clears A Line & Set_Pos(0,line)
void LCD_Clear_Line(I2C_LCD_Handler *lcd, uint8_t line);

//- Clears ALL Text & Set_Pos(0,0)
void LCD_Clear_All(I2C_LCD_Handler *lcd);

//-- LCD Advanced Screen Manipulation Functions

//- Any Funtion That Cannot Be Executed SingleHandedly By LCD_Send() (Needs MCU To Happen Properly)

//- Scrolls The Screen With Custom Steps & Optional Auto-Clear
void LCD_Scroll_Shift(I2C_LCD_Handler *lcd, uint8_t Direction, uint8_t ShiftsCount, uint32_t ShiftInterval, uint8_t AutoClear);

//- Writes A String With Delay In Each Char
void LCD_Write_T_String(I2C_LCD_Handler *lcd, const char *str, uint32_t PlacingTime);

//- Centralize & Writes A T_String
void LCD_Write_CT_String(I2C_LCD_Handler *lcd, const char *str, uint32_t PlacingTime);

#endif /* LCD_I2C_H */
