#include "TransparentMatrix.h"

extern "C" void espShow(uint16_t pin, uint8_t *pixels, uint32_t numBytes, uint8_t type);

void TransparentMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x >= _width || y >= _height) return;
  int postion = (x * _height) + y;

  if (color != 0) color = 5;

  matrix[postion] = (uint8_t)color;
}

void TransparentMatrix::show() {
  espShow(pin, matrix, _width * _height, false);
}

void TransparentMatrix::setPixelColor(uint16_t n, uint8_t color) {
  if (n < 288) {
    matrix[n] = color;
  }
}

void TransparentMatrix::setPixelColor(uint16_t x, uint16_t y, uint8_t color) {
  int postion = (x * _height) + y;
  if (postion < 288) {
    matrix[postion] = color;
  }
}

void TransparentMatrix::clear()
{
  memset(matrix, 0, _width * _height * 3);
}