# STM32-I2C-LCD
## A Library for communicating with one or multiple LCDs of any size using I2C protocol on any STM32 board that supports I2C communication.


# Preparing it for usage:

## 1: Download whichever version you are most interested in using.
There are three possible versions: Control, Standard, Lite;

### 1.1 Use Control If:
You want to be able to customize all aspects of exhibition, that meaning:

#### Expected Data
- 8 or 4 bits mode;
- 2 or 1 line mode;
- 5x10 or 5x8 font;

#### UI Behavior
- Display On or Off;
- Cursor On or Off;
- Blinking On or Off;

#### Entry Behavior
- Cursor Moves Right or Left after each input;
- Display Shift On or Off;

#### Heavier Header
- To accommodate all these options, multiple defines will be created for easier access while coding;

### 1.2 Use Standard If:
You want to exhibit text or data while having access to the following funtions:


### LCD_Clear_Partial_Line( )
Responsible for clearing all the contents in one line after a certain column;


### LCD_Write_C_String( )
Responsible for centralizing any string automatically;


### LCD_Write_T_String( )
Responsible for timing the placement of each individual character in any string given;


### LCD_Write_CT_String( )
Combines behavior from LCD_Write_C_String() & LCD_Write_T_String() in one funcion;


### LCD_Scroll_Shift( )
Responsible for scrolling the screen either to the left or to the right until it arrives in its initial position;


### LCD_Scroll_Disappear( )
Responsible for using LCD_Scroll_Shift() only until the text is no longer on the screen
(Only works in 16x2 screens due to hardware configurations on bigger screens)

### 1.3 Use Lite If:
You just want raw exhibition, your writing funtions being only:

### LCD_Write_Char( )

### LCD_Write_String( )

### LCD_Write_Number( )

### LCD_Clear_Line( )

## 2: Make sure to switch I2C connectivity on your STM32 Board, main.c must have I2C handler
### If you did, it will appear on the generated code's private variables,as in:

```
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
```

## 3: Move the files to "Scr" folder on the project you intend to use it;

## 4: Include header in main.c or main.h:

```
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_I2C.h"
/* USER CODE END Includes */
```

## 5: Include Handler :

```
/* USER CODE BEGIN PV */
I2C_LCD_Handler lcd1;
/* USER CODE END PV */
```

## 6: Initialize LCD by informing:
### 6.1: Which I2C_LCD_Handler will be responsible for the LCD you're working on;
### 6.2: Which I2C_HandleTypeDef will be used for communicating with the STM32 board;
### 6.3: Number of columns the LCD has;
### 6.4: Number of Lines the LCD has;

```
/* USER CODE BEGIN 2 */
LCD_Default_Init(&lcd1, &hi2c1, 20, 4);
/* USER CODE END 2 */
```

### Note: Items 6.3 and 6.4 are the maximum numbers of any given LCD.
#### Meaning you may configure it to work with less than that.
#### For example
#### If your code only initializes an 16x2 LCD, and later on you substitute it for a 20x4 without changing the code.
#### It will work normally, writting only up to column 16 and line 2, considering extra columns and lines as non-existing.

## 7: It is ready for usage;
