#include "LCD_I2C.h"

//--- LCD Hardware Focused Functions

//- STM32 Electrical Translation Of Data To I2C
void LCD_Write_Bus(I2C_LCD_Handler *lcd, uint8_t *data, uint16_t size) {HAL_I2C_Master_Transmit(lcd->STM_I2C_Handler, (lcd->AddressAndBl & LCD_Address_Mask), data, size, 100);}

//- Used To Send (Commands ^ Text) To LCD
void LCD_Send(I2C_LCD_Handler *lcd, uint8_t CMD_Or_Txt, LCD_Type_Of_Data Type)
{
    uint8_t Data_Buffer[4];

    for(uint8_t Data_Pulse = 0; Data_Pulse < 4 ; Data_Pulse++)
    	Data_Buffer[Data_Pulse] =
        ( ( (CMD_Or_Txt << (0x04 & Data_Pulse<<1) ) & 0xF0 ) | //- Changes Between The High ^ Low Nibble Of The CMD_Or_Txt
        ((lcd->AddressAndBl&LCD_Backlight_Mask)<<3) | //- Backlight State
        (((Data_Pulse&0x01)^0x01)<<2) | //- Changes En Pin Between High ^ Low
        (Type&LCD_RS_On) ); //- Defines If The CMD_Or_Txt Is A Command Or A Text

    LCD_Write_Bus(lcd, Data_Buffer, sizeof(Data_Buffer));
}

//- Useful For Most Cases, Create More As Needed
void LCD_Default_Init(I2C_LCD_Handler *lcd, I2C_HandleTypeDef *STM_H_I2C, uint8_t nColumns, uint8_t nLines)
{
//- Struct Building
	lcd->STM_I2C_Handler = STM_H_I2C;
	for(uint8_t address = 0x08; address <= 0x77; address++) // Valid 7bits I2C Addresses
	    if(HAL_I2C_IsDeviceReady(STM_H_I2C, address << 1, 3, 50) == HAL_OK)
			lcd->AddressAndBl = (address << 1) | LCD_Backlight_Mask;
    LCD_Set_MaxColumns(lcd, --nColumns);
    LCD_Set_MaxLines(lcd, --nLines);
	lcd->cColsAndLines = 0x00;

//- Wake Up Sequence
	HAL_Delay(50); // Wait For LCD To Power-Up

	for(uint8_t CPR = 0; CPR < 3; CPR++) // Does "CPR" On The LCD Hoping It Will Begin Receiving Data
	{
		LCD_Send(lcd, 0x30, LCD_Command); //LCD_8bits_Mode, LCD_1Line_Mode, LCD_5x8_Font
		HAL_Delay(1); // "Compression Rhythm"
	}

	LCD_Send(lcd, 0x20, LCD_Command); //LCD_4bits_Mode, LCD_1Line_Mode, LCD_5x8_Font
	HAL_Delay(1);

//- Function Set
	LCD_Send(lcd, 0x28, LCD_Command); // LCD_8bits_Mode, LCD_1Line_Mode, LCD_5x8_Font
	HAL_Delay(1);

//- Display Off
	LCD_Send(lcd, 0x08, LCD_Command); //LCD_Display_Off, LCD_Cursor_Off, LCD_Blink_Off)

//- Clear Display
	LCD_Send(lcd, LCD_Clear_Display, LCD_Command);
	HAL_Delay(2); //Required By DataSheet (max ~2ms)

//- Entry Mode Set
	LCD_Send(lcd, 0x06, LCD_Command); // LCD_Display_On, LCD_Cursor_Off, LCD_Blink_Off)
	HAL_Delay(1);

//- Display ON
	LCD_Send(lcd, 0x0C, LCD_Command);
	HAL_Delay(1);
}

//--- User Focused Functions

//-- LCD Handler Sets

void LCD_Set_Pos(I2C_LCD_Handler *lcd, uint8_t Column, uint8_t Line)
{
	if(Column > (LCD_Get_MaxColumns(lcd)) || Line > (LCD_Get_MaxLines(lcd)))
	    return; //Invalid Column | Line

    uint8_t address;

    switch (Line) // Address Values May Differ, But This Ought To Work With Most LCDs
    {
        case 0: address = 0x00 + Column; break;  // First line
        case 1: address = 0x40 + Column; break;  // Second line
        case 2: address = 0x14 + Column; break;  // Third line
        case 3: address = 0x54 + Column; break;  // Fourth line
        default: return; // Invalid Line
    }

    LCD_Set_CurrentColumn(lcd, Column);
    LCD_Set_CurrentLine(lcd, Line);

    LCD_Send(lcd, (LCD_Set_DDRAM_Address | address), LCD_Command);
}

//-- Writing Functions

void LCD_Write_Char(I2C_LCD_Handler *lcd, char ch)
{
    LCD_Send(lcd, ch, LCD_Text);

    if (LCD_Get_CurrentColumn(lcd) >= LCD_Get_MaxColumns(lcd))
    {
        if (LCD_Get_CurrentLine(lcd) >= LCD_Get_MaxLines(lcd)) LCD_Set_CurrentLine(lcd, 0);
        else LCD_Set_CurrentLine(lcd, (1 + LCD_Get_CurrentLine(lcd)));

        LCD_Set_CurrentColumn(lcd, 0);

        LCD_Set_Pos(lcd, LCD_Get_CurrentColumn(lcd), LCD_Get_CurrentLine(lcd));
    }
    else LCD_Set_CurrentColumn(lcd, (1 + LCD_Get_CurrentColumn(lcd)));
}

void LCD_Write_String(I2C_LCD_Handler *lcd, const char *str) {while (*str) LCD_Write_Char(lcd, *str++);}

//- Converts Unsigned Integers Into Text For Exhibition
void LCD_Write_Number(I2C_LCD_Handler *lcd, uint32_t Number)
{
    uint8_t NumDigits = (Number < 10) ? 1 :
                        (Number < 100) ? 2 :
                        (Number < 1000) ? 3 :
                        (Number < 10000) ? 4 :
                        (Number < 100000) ? 5 :
                        (Number < 1000000) ? 6 :
                        (Number < 10000000) ? 7 :
                        (Number < 100000000) ? 8 :
                        (Number < 1000000000) ? 9 : 10;

    uint8_t TxtSize = NumDigits + 1; //+1 For '\0'

    char Txt[TxtSize];

    uint8_t index = sizeof(Txt) - 1;

    if(index>(((LCD_Get_MaxColumns(lcd))-(LCD_Get_CurrentColumn(lcd))))) return; //Num Is Too Big To Exhibit

    Txt[index] = '\0';

    do
    {
        Txt[--index] = '0' + (Number % 10);
        Number /= 10;
    }
    while(Number);

    LCD_Write_String(lcd, &Txt[index]);
}

//- Clears A Line Starting At A Specific Column & Set_Pos(start,line)
void LCD_Clear_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start)
{
    LCD_Set_Pos(lcd, start, line);
    while ((LCD_Get_CurrentLine(lcd)) == line) LCD_Write_Char(lcd, ' ');
    LCD_Set_Pos(lcd, start, line);
}

//- Clears A Line & Set_Pos(0,line)
void LCD_Clear_Line(I2C_LCD_Handler *lcd, uint8_t line) {LCD_Clear_Partial_Line(lcd, line, 0);}

//- Clears ALL Text & Set_Pos(0,0)
void LCD_Clear_All(I2C_LCD_Handler *lcd)
{
    LCD_Send(lcd, LCD_Clear_Display, LCD_Command);

    HAL_Delay(2); //Required By DataSheet (Max ~2ms)

    lcd->cColsAndLines = 0x00;
}
