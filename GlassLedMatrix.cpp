#include "GlassLedMatrix.h"

extern "C" void espShow(uint16_t pin, uint8_t *pixels, uint32_t numBytes, uint8_t type);

void GlassLedMatrix::begin() {

}

void GlassLedMatrix::stop() {

}

void GlassLedMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x >= panel_width || y >= panel_height) return;
  if (color == 0) return;
  int postion = (x * panel_height) + y;

  matrix[postion] = (uint8_t)singleColorFixValue;
  isDraw = true;
}

void GlassLedMatrix::drawPixelBorderCheck(int16_t x, int16_t y, uint16_t color) {
  if (color == 0) return;

  int border_offSet = 0;
  
  border_offSet += this->border_count;

  if (x < 0 + border_offSet || x >= panel_width - border_offSet || y < 0 + border_offSet || y >= panel_height - border_offSet) return;
  int16_t idx =(x * panel_height) + y;
  matrix[idx] = (uint8_t)singleColorFixValue;
  isDraw = true;
}

void GlassLedMatrix::show() {
  espShow(pin, matrix, _width * panel_height, false);
}

void GlassLedMatrix::drawPixelColorCheck(int16_t x, int16_t y, uint16_t color, uint8_t flag) { //  flag 0  = draw, flag 1 = clear
  if (x < 0 || x >= panel_width || y < 0  || y >= panel_height ) return;
  if (color == 0) return;

  // int16_t idx = x + y * panel_width;
  int16_t idx = (x * panel_height) + y;

  if (matrix[idx] == 0 && flag == 0) {
    matrix[idx] = color;
  } else if (matrix[idx] == color && flag == 1) {
    matrix[idx] = 0;
  }
  isDraw = true;
}

void GlassLedMatrix::copyRGBBitmapRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* _bitmap) {
  if (x >= _width || y >= panel_height) return;

  int idx = 0;    
  for (int j= 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      _bitmap[idx++] = matrix[((x + i) * panel_height) + (y + j)];
    }
  }
}

void GlassLedMatrix::draw() {
  // if (isDraw) {
    show();
    isDraw = false;
  // }
}