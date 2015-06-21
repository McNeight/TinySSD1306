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


#ifdef __AVR_ATtiny85__
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "TinySSD1306.h"
#include "LCD_font_5x7.h"

//static uint8_t buffer[1024] = {0x00};
extern const uint8_t font_5x7[] PROGMEM;

TinySSD1306::TinySSD1306() : width_(WIDTH), height_(HEIGHT),
  cursor_x_(0), cursor_y_(0) {

  pinMode(RESET, OUTPUT);
  pinMode(DATA_COMMAND, OUTPUT);

  /* reset the display */
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(RESET, LOW);
  delay(1);
  digitalWrite(RESET, HIGH);

  /* Start initializing the display */
  write_command(DISPLAY_OFF);
  write_command(SET_DISPLAY_CLOCK_DIV);
  write_command(0x80);
  write_command(SET_MULTIPLEX);
  write_command(0x3F);
  write_command(SET_DISPLAY_OFFSET);
  write_command(0x0);
  write_command(SET_START_LINE);
  write_command(CHARGE_PUMP);
  /* We are using the internal DC/DC converter */
  write_command(0x14);

  write_command(MEMORY_MODE);
  //write_command(0x00); // Horizontal Address Mode
  write_command(0x10); // Page Address Mode n10

  write_command(SEGMENT_RE_MAP);
  write_command(COM_SCAN_DIRECTION);
  write_command(SET_COM_PINS);
  write_command(0x12);
  write_command(SET_CONTRAST);
  write_command(0x8F);
  write_command(SET_PRECHARGE);
  /* We are using the internal DC/DC converter */
  write_command(0xF1);
  write_command(SET_VCOM_DETECT);
  write_command(0x40);
  write_command(DISPLAY_ON_RESUME);
  write_command(SET_NORMAL_DISPLAY);

  /* We are done configuring the display, we can turn the display on */
  write_command(DISPLAY_ON);

  /* Construct the font handler */
  //FontHandler(get_width(), get_height());

}

void TinySSD1306::clear_display() {

  //memset(buffer, 0, 1024);
  /* Go to the start of the screen */
  write_command(SET_LOW_COLUMN);
  write_command(SET_HIGH_COLUMN);
  write_command(SET_START_LINE);

  for (uint16_t i = 0; i < 1024; ++i) {
    write_data(0x00);
  }

}
void TinySSD1306::set_pixel(char x, char y) {

  if (x > 128 || y > 64) {
    return;
  }

  uint16_t byte = 0;
  if (y / 8 == 0) {
    byte = x;
  }
  else {
    byte = x + 128 * (y / 8);
  }

  //buffer[byte] |= (0x01<<(y-8*(y/8)));

}

uint16_t TinySSD1306::get_width() {

  return WIDTH;

}
uint16_t TinySSD1306::get_height() {

  return HEIGHT;

}
void TinySSD1306::invert_display(int invert) {

  if (invert) {
    write_command(SET_INVERTED_DISPLAY);
  }
  else {
    write_command(SET_NORMAL_DISPLAY);
  }
}
void TinySSD1306::printChar(char ch) {

  uint16_t byt = 0;
  byte b, top, bot;

  if (cursor_y_ / 8 == 0) {
    byt = cursor_x_;
  }
  else {
    byt = cursor_x_ + 128 * (cursor_y_ / 8);
  }

  write_command(SET_PAGE_START | (cursor_y_ / 8));
  write_command(SET_LOW_COLUMN |  (cursor_x_ & 0x0F));
  write_command(SET_HIGH_COLUMN  |  ((cursor_x_ & 0xF0) >> 4));
  //write_command(SET_START_LINE | (cursor_y_/8));


  // top half

  for (uint8_t i = 0; i < 5; ++i)
  {
    /* For reading a byte from the font table we need to read it from flash */
    b = pgm_read_byte(&font_5x7[(ch - 0x20) * 5 + i]);
    write_data(b);
  }


  cursor_x_ += 6;
  if (cursor_x_ > width_ - 5 && cursor_y_ >= height_ - 8) {
    cursor_x_ = 0;
    cursor_y_ = height_ - 8;
  }
  else {
    if (cursor_x_ > width_ - 10) {
      cursor_x_ = 0;
      cursor_y_ += 8;
    }
    if (cursor_y_ > height_ - 8) {
      cursor_y_ = height_ - 8;
    }
  }
}




void TinySSD1306::printCharX2(char ch) {

  uint16_t byt = 0;
  byte b, top, bot;

  if (cursor_y_ / 8 == 0) {
    byt = cursor_x_;
  }
  else {
    byt = cursor_x_ + 128 * (cursor_y_ / 8);
  }

  write_command(SET_PAGE_START | (cursor_y_ / 8));
  write_command(SET_LOW_COLUMN |  (cursor_x_ & 0x0F));
  write_command(SET_HIGH_COLUMN  |  ((cursor_x_ & 0xF0) >> 4));



  // top half

  for (uint8_t i = 0; i < 5; ++i)
  {
    /* For reading a byte from the font table we need to read it from flash */
    b = pgm_read_byte(&font_5x7[(ch - 0x20) * 5 + i]);

    top = (b & 0x01) | (b & 0x02) << 1 | (b & 0x04) << 2 | (b & 0x08) << 3;
    top = top | (top << 1);

    write_data(top);
    write_data(top);
  }

  cursor_y_ += 8;
  write_command(SET_PAGE_START | (cursor_y_ / 8));
  write_command(SET_LOW_COLUMN |  (cursor_x_ & 0x0F));
  write_command(SET_HIGH_COLUMN  |  ((cursor_x_ & 0xF0) >> 4));

  for (uint8_t i = 0; i < 5; ++i)
  {
    // For reading a byte from the font table we need to read it from flash
    b = pgm_read_byte(&font_5x7[(ch - 0x20) * 5 + i]);
    bot = (b & 0x10) >> 3 | (b & 0x20) >> 2 | (b & 0x40) >> 1 | (b & 0x80);
    bot = bot | (bot >> 1);
    write_data(bot);
    write_data(bot);

  }

  cursor_y_ -= 8;

  cursor_x_ += 12;
  if (cursor_x_ > width_ - 10 && cursor_y_ >= height_ - 16) {
    cursor_x_ = 128;
    cursor_y_ = height_ - 16;
  }
  else {
    if (cursor_x_ > width_ - 10) {
      cursor_x_ = 0;
      cursor_y_ += 16;
    }
    if (cursor_y_ > height_ - 16) {
      cursor_y_ = height_ - 16;
    }
  }
}


void TinySSD1306::setCursor(uint16_t x, uint16_t y) {

  if (x > width_) {
    cursor_x_ = width_;
  }
  else {
    cursor_x_ = x;
  }
  if ( y > height_) {
    cursor_y_ = height_;
  }
  else {
    cursor_y_ = y;
  }

}
inline void TinySSD1306::write(char data) {

  PORTB &= ~_BV(2);
  SPI.transfer(data);
  PORTB |= _BV(2);

}
inline void TinySSD1306::write_command(char data) {

  digitalWrite(DATA_COMMAND, LOW);
  write(data);

}
inline void TinySSD1306::write_data(char data) {

  digitalWrite(DATA_COMMAND, HIGH);
  write(data);

}
