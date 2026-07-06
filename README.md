# STM32-I2C-LCD

## A library for communicating with one or multiple HD44780-compatible I2C LCDs using any STM32 microcontroller with I2C support.

Supported display sizes include **8×1, 8×2, 16×2, 20×2, 20×4, 24×2 and 40×2**.

> **Note**
>
> This library targets standard single-controller HD44780-compatible LCDs.
>
> Some **16×1** displays may not behave correctly due to their internal DDRAM mapping.
>
> Although the library can theoretically address a **40×4** display, most commercially available 40×4 modules use two **Enable (EN)** pins and are therefore **not supported**.

## Features

- Supports multiple LCDs simultaneously
- Automatic I2C address detection
- Automatic cursor position tracking
- Bounds checking for every cursor operation
- Compatible with multiple display sizes
- Modular versions (Lite, Standard and Control)

# Getting Started

## 1. Choose the version that best fits your project

Three versions are available.

---

### Lite

Choose this version if you only need basic text output with the smallest possible API.

Available functions:

* `LCD_Write_Bus()`
* `LCD_Send()`
* `LCD_Default_Init()`
* `LCD_Write_Char()`
* `LCD_Write_String()`
* `LCD_Write_Number()`
* `LCD_Clear_Partial_Line()`
* `LCD_Clear_Line()`
* `LCD_Clear_All()`

#### Additional Notes

This Version's LCD_Write_Number() only supports integer values.

---

### Standard (Recommended)

Recommended for most applications.

In addition to every function available in the Lite version, it also provides several high-level utilities.

Available functions:

* `LCD_Write_Bus()`
* `LCD_Send()`
* `LCD_Set_Backlight()`
* `LCD_Default_Init()`
* `LCD_Set_Pos()`
* `LCD_Set_Pos_Home()`
* `LCD_Set_Pos_Column()`
* `LCD_Set_Pos_Centered()`
* `LCD_Set_Pos_Line()`
* `LCD_Write_Char()`
* `LCD_Write_String()`
* `LCD_Write_C_String()`
* `LCD_Write_T_String()`
* `LCD_Write_CT_String()`
* `LCD_Write_Number()`
* `LCD_Fill_Partial_Line()`
* `LCD_Fill_Line()`
* `LCD_Fill_All()`
* `LCD_Clear_Partial_Line()`
* `LCD_Clear_Line()`
* `LCD_Clear_All()`
* `LCD_Scroll_Shift()`

---

### Control

Choose this version if you want complete control over the HD44780 configuration.

This version differs itself from standard by exposing the HD44780 configuration through multiple `#define` values, making the initialization sequence fully customizable.

```c
//-- Modifies Expected Data -----------------------------------------------------------------------

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

```

---

# Installation

## 2. Enable the I2C peripheral

Create your STM32 project with I2C enabled using STM32CubeMX or STM32CubeIDE and generate the code.

## 3. Copy the library files

* Copy **LCD_I2C.c** into your project's **Src** folder.
* Copy **LCD_I2C.h** into your project's **Inc** folder.

## 4. Include the header

```c
/* USER CODE BEGIN Includes */
#include "LCD_I2C.h"
/* USER CODE END Includes */
```

## 5. Verify and add desired STM32 board

Inside the LCD_I2C.h You'll find this section for adding your STM32 board

```c
//--- Which STM32 Boards It Is Ready To Function With, Add As Necessary

#if __has_include("stm32f1xx_hal.h")
	#include "stm32f1xx_hal.h"
#elif __has_include("stm32f4xx_hal.h")
	#include "stm32f4xx_hal.h"
#endif
```

## 6. Declare the LCD handler and initialize it

Your project should look similar to the following:

```c
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
...
...
...
/* USER CODE BEGIN PV */
I2C_LCD_Handler lcd1;
/* USER CODE END PV */
...
...
...

int main(void)
{
    ...

    /* USER CODE BEGIN 2 */
    LCD_Default_Init(&lcd1, &hi2c1, 20, 4);
    /* USER CODE END 2 */

    /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        ...
    }
/* USER CODE END WHILE */
}
```

`LCD_Default_Init()` receives:

* The `I2C_LCD_Handler` associated with the display.
* The STM32 `I2C_HandleTypeDef`.
* The display width (columns).
* The display height (lines).

Example:

```c
LCD_Default_Init(&lcd1, &hi2c1, 20, 4);
```

## Done!

Your LCD is now ready to use.

## Details

### Coordinates

The initialization function receives the **physical dimensions** of the LCD, but every function in the library uses **zero-based indexing**, following the C language convention.

For a **20×4** display:

* Valid columns are **0** through **19**.
* Valid lines are **0** through **3**.

Examples:

```c
LCD_Set_Pos(&lcd1, 0, 0);    // Upper-left corner
LCD_Set_Pos(&lcd1, 19, 3);   // Bottom-right corner
```
> Invalid coordinates are automatically ignored, preventing accidental writes outside the configured display area.

### Example Using Standard And Multiple LCDs

All Others Will Work Similarly In Other Versions.

Once initialized, the LCD is ready to be used inside your application's main loop.

```c
/* USER CODE BEGIN PV */
I2C_LCD_Handler lcd1;
I2C_LCD_Handler lcd2; //Doesn't Necessarily Needs Another I2C_HandleTypeDef unless it has the same address as lcd1
/* USER CODE END PV */
...
...
...
int main(void)
{
    ...

/* USER CODE BEGIN 2 */

LCD_Default_Init(&lcd1, &hi2c1, 20, 4);
LCD_Default_Init(&lcd2, &hi2c1, 16, 2);

/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        LCD_Write_CT_String(&lcd1, "Example", 75);
        LCD_Write_Number(&lcd2, 123.45, 3);
        LCD_Fill_Line(&lcd2, 1, '=');
    
        HAL_Delay(1000);
    
        LCD_Set_Pos(&lcd1, 0, 1);
        LCD_Fill_Line(&lcd1, 1, '#');
        LCD_Clear_Partial_Line(&lcd2, 0, 3);
    
        HAL_Delay(1000);
    
        LCD_Clear_All(&lcd1);
        LCD_Clear_Line(&lcd2, 0);
    }
/* USER CODE END WHILE */
```

### Display Size

The configured width and height define the maximum area the library is allowed to access.

This means you may intentionally configure smaller dimensions than the physical display.

For example, if your firmware is configured for a **16×2** LCD and the hardware is later replaced with a **20×4** LCD, the application will continue working without modifications. The library will simply limit all writing operations to the configured 16 columns and 2 lines, treating the remaining display area as unavailable.

The Theoretical Limit is a 64x4 Display, however, as mentioned earlier, hardware limitations will make this size non-usable for most cases.
