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

//-- Defines For Functions

#define LCD_RS_On 0x01

#define LCD_Clear_Display 0x01

#define LCD_Return_Home 0x02

#define LCD_Set_DDRAM_Address 0x80

#define LCD_Max_Internal_DDRAM_nColumns 0x40

#define LCD_SCROLL_FULL  LCD_Max_Internal_DDRAM_nColumns
#define LCD_SCROLL_HALF  (LCD_Max_Internal_DDRAM_nColumns >> 1)

#define LCD_Shift_Right 0x04
#define LCD_Shift_Left 0x00

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
