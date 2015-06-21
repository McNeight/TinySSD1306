/*
    Copyright © 2012-2015 Kalle Hyvönen, Rob (pocketmoon), Neil McNeight

    This file is part of TinySSD1306.

    TinySSD1306 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    TinySSD1306 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

-------------------------------------------------------------------------------
Change Log

DATE      VER   WHO   WHAT
06/20/15  1.6.0 NEM   Code cleanup and compatibility with Arduino 1.6.*
-------------------------------------------------------------------------------

 */

 
#ifndef TinySSD1306_h
#define TinySSD1306_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM

#if ARDUINO >= 100
#define WIRE_WRITE TinyWireM.write
#else
#define WIRE_WRITE TinyWireM.send
#endif

#else
#include <Wire.h>
#endif

/* Display size in pixels */
#define WIDTH 128
#define HEIGHT 64

/* MCU connections, SPI in regular SPI pins */
#define DATA_COMMAND 9
#define RESET 8

/* Commands for the display */
#define DISPLAY_OFF 0xAE
#define SET_DISPLAY_CLOCK_DIV 0xD5
#define SET_MULTIPLEX 0xA8
#define SET_DISPLAY_OFFSET 0xD3
#define SET_START_LINE 0x40
#define CHARGE_PUMP 0x8D

//Horizontal addressing mode (A[1:0]=00b)
//Vertical addressing mode: (A[1:0]=01b)
//Page addressing mode (A[1:0]=10xb)
#define MEMORY_MODE 0x20


#define SEGMENT_RE_MAP 0xA1
#define COM_SCAN_DIRECTION 0xC8
#define SET_COM_PINS 0xDA
#define SET_CONTRAST 0x81
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DETECT 0xDB
#define DISPLAY_ON_RESUME 0xA4
#define SET_NORMAL_DISPLAY 0xA6
#define SET_INVERTED_DISPLAY 0xA7
#define DISPLAY_ON 0xAF
#define SET_LOW_COLUMN 0x00
#define SET_HIGH_COLUMN 0x10

//Set GDDRAM Page Start Address 0xB0 - OxB7
#define SET_PAGE_START 0xB0

#include "Arduino.h"

//#include "lib_font_handler.h"

/* Driver inherits functions from Font_Handler to be able to use fonts */
class TinySSD1306 {
  public:
    /* Constructor, this also initializes the screen */
    TinySSD1306();

    /* Clear the display buffer */
    void clear_display();

    /* Send buffer contents to the display */
    void display_buffer();

    /* Set one pixel on */
    void set_pixel(char x, char y);

    /* Invert the display, 0 for normal  */
    void invert_display(int invert);

    /* Return display width */
    uint16_t get_width();

    /* Return display height */
    uint16_t get_height();

    /* Print a char */
    void printChar(char ch);
    void printCharX2(char ch);

    /* Set cursor position in pixels */
    void setCursor(uint16_t x, uint16_t y);

  private:
    /* Size of the display, never changes */
    uint16_t width_, height_;
    uint16_t cursor_x_, cursor_y_;

    /* Write a byte to the display */
    void write(char data);

    /* Send a command to the display */
    void write_command(char data);

    /* Send data to the display */
    void write_data(char data);

};

#endif // TinySSD1306_h
