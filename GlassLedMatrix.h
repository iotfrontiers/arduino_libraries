#ifndef _GLASS_LED_MATRIX_
#define _GLASS_LED_MATRIX_

#include <vector>
#include <array>
#include "Adafruit_GFX.h"

class GlassLedMatrix : public Adafruit_GFX {
  public:
    GlassLedMatrix(uint8_t _pin, uint16_t _width, uint16_t _height) //  생성자 -- 핀 번호, 디스플레이 넓이, 디스플레이 높이 세팅 
      : Adafruit_GFX(_width, _height) {

      matrix = (uint8_t*) calloc (_width * _height, sizeof(uint8_t)); //  높이 * 넓이 만큼 매트릭스 버퍼 동적 할당
      pin = _pin;
      panel_width = _width;
      panel_height = _height;
    }

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);   //  제한 없이 그림

    uint8_t* matrix;    /// 매트릭스 버퍼 (0~31)
    int16_t panel_width = 24, panel_height = 12;        ///< 디스플레이 도트 사이즈
    uint8_t pin = 25;                                   ///< 데이터 핀
    
    void show();
    void setPixelColor(uint16_t n, uint8_t color);
    void setPixelColor(uint16_t x, uint16_t y, uint8_t color);
    void clear();

  private:
};

#endif