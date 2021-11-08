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
    virtual void drawPixelBorderCheck(int16_t x, int16_t y, uint16_t color); //  Flag 에 따라 제한적으로 그림

    void show();

    void begin(void);
    void stop(void);
    uint8_t color444(uint8_t r, uint8_t g, uint8_t b) { return (r+g+b) / 3; }

    uint8_t color555(uint8_t r, uint8_t g, uint8_t b) { return (r+g+b) / 3; }

    uint8_t drawPixelRGB565(uint16_t color);

    uint16_t colorHSV(long hue, uint8_t sat, uint8_t val){ return 15;};

    void clearMatrix();
    void drawPixelColorCheck(int16_t x, int16_t y, uint16_t color, uint8_t flag); //  사용중인 LED에 덮어쓰지 않음
    void copyRGBBitmapRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* _bitmap);
    
    uint8_t* matrix;    /// 매트릭스 버퍼 (0~31)
    int16_t panel_width = 24, panel_height = 12;        ///< 디스플레이 도트 사이즈
    uint8_t pin = 14;                                   ///< 데이터 핀

    void draw();

  private:
    bool isDraw = false;
};

inline uint8_t GlassLedMatrix::drawPixelRGB565(uint16_t color) 
{
  uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;

  r /= 8;
  g /= 8;
  b /= 8;
  // Serial.printf("red : %d, green : %d, blue : %d \n", r, g, b);
  return color555(r, g, b);
}

inline void GlassLedMatrix::clearMatrix() {
  /* case : all dot clear */
  memset(matrix, 0, sizeof(uint8_t) * (panel_width * panel_height));
  isDraw = true;
}

#endif