#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

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

#define LCD_Set_DDRAM_Address 0x80

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

//- Useful For Most Cases, Create More As Needed
void LCD_Default_Init(I2C_LCD_Handler *lcd, I2C_HandleTypeDef *STM_H_I2C, uint8_t nColumns, uint8_t nLines);

//--- User Focused Functions

//-- LCD Positioning Functions

void LCD_Set_Pos(I2C_LCD_Handler *lcd, uint8_t column, uint8_t line);

//-- Writing Functions

void LCD_Write_Char(I2C_LCD_Handler *lcd, char ch);

void LCD_Write_String(I2C_LCD_Handler *lcd, const char *str);

//- Converts Unsigned Integers Into Text For Exhibition
void LCD_Write_Number(I2C_LCD_Handler *lcd, uint32_t Number);

//- Clears A Line Starting At A Specific Column & Set_Pos(start,line)
void LCD_Clear_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start);

//- Clears A Line & Set_Pos(0,line)
void LCD_Clear_Line(I2C_LCD_Handler *lcd, uint8_t line);

//- Clears ALL Text & Set_Pos(0,0)
void LCD_Clear_All(I2C_LCD_Handler *lcd);

#endif /* LCD_I2C_H */
