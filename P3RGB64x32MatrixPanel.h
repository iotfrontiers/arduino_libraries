#ifndef _ESP32_P3_RGB_64_32_MATRIX_PANEL
#define _ESP32_P3_RGB_64_32_MATRIX_PANEL

#include <vector>
#include <array>
#include "Adafruit_GFX.h"

class P3RGB64x32MatrixPanel : public Adafruit_GFX {
  public:
    P3RGB64x32MatrixPanel(uint8_t _pinR1, uint8_t _pinG1, uint8_t _pinB1, uint8_t _pinR2, uint8_t _pinG2, uint8_t _pinB2, uint8_t _pinCLK, uint8_t _pinLAT, uint8_t _pinOE, uint8_t _pinA, uint8_t _pinB, uint8_t _pinC, uint8_t _pinD, uint8_t _width, uint8_t _height, bool _doubleBuffer = false)
      : Adafruit_GFX(_width, 32), pinR1(_pinR1), pinG1(_pinG1), pinB1(_pinB1), pinR2(_pinR2), pinG2(_pinG2), pinB2(_pinB2), pinCLK(_pinCLK), pinLAT(_pinLAT), pinOE(_pinOE), pinA(_pinA), pinB(_pinB), pinC(_pinC), pinD(_pinD), doubleBuffer(_doubleBuffer) {
      this->panel_width  = _width;
      this->panel_height = _height;

      _matrixbuff = (uint16_t*) calloc (_width * _height, sizeof(uint16_t));
      initMatrixBuff();
    }
    P3RGB64x32MatrixPanel(bool _doubleBuffer = false)
      : Adafruit_GFX(64, 32), doubleBuffer(_doubleBuffer) {
      initMatrixBuff();
    }
    void begin(void);
    void stop(void);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);   //  제한 없이 그림
    virtual void drawPixelBorderCheck(int16_t x, int16_t y, uint16_t color); //  Flag 에 따라 제한적으로 그림
    uint16_t color444(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xf) << 1) | ((uint16_t)(g & 0xf) << 6) | ((uint16_t)(b & 0xf) << 11); }

    uint16_t color555(uint8_t r, uint8_t g, uint8_t b) { return (r&0x1f) | ((uint16_t)(g & 0x1f) << 5) | ((uint16_t)(b & 0x1f) << 10); }
    uint16_t drawPixelRGB565(uint16_t color);

    uint16_t colorHSV(long hue, uint8_t sat, uint8_t val);

    void clearMatrix();
    void setDrawTimer(int timer);
    int getDrawTimer();
    void drawPixelColorCheck(int16_t x, int16_t y, uint16_t color, uint8_t flag); //  사용중인 LED에 덮어쓰지 않음
    
    bool ittrTimerCrashCheck();
    void copyRGBBitmapRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* _bitmap);
    
    void swapBuffer() {
      matrixbuff = drawBuffer();
    }

    uint16_t* matrixbuff;   //  Direct Matrix Buffer
    uint16_t* _matrixbuff;  //  Temp Matrix Buffer

    int16_t panel_width = 64, panel_height = 32;        ///< Display Pix Dot Size

    void copyBuffer() {
    }
    void IRAM_ATTR draw();

  private:
    void initMatrixBuff() {
      matrixbuff = _matrixbuff;
    }
    static void IRAM_ATTR onTimer(void);

    uint16_t* drawBuffer() {
      if (!doubleBuffer) return _matrixbuff;
    }

    hw_timer_t* timer;

    uint8_t pinR1 = 25;
    uint8_t pinG1 = 26;
    uint8_t pinB1 = 27;
    uint8_t pinR2 = 21;
    uint8_t pinG2 = 22;
    uint8_t pinB2 = 23;

    uint8_t pinCLK = 15;
    uint8_t pinLAT = 32;
    uint8_t pinOE = 33;

    uint8_t pinA = 12;
    uint8_t pinB = 16;
    uint8_t pinC = 17;
    uint8_t pinD = 18;

    bool doubleBuffer;

    int timer_period = 30;
    bool matrixStatus = false;
    
    static volatile SemaphoreHandle_t timerSemaphore;
    static P3RGB64x32MatrixPanel *singleton;
};

inline uint16_t P3RGB64x32MatrixPanel::drawPixelRGB565(uint16_t color) 
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

inline void P3RGB64x32MatrixPanel::clearMatrix() {
  /* case : all dot clear */
  memset( _matrixbuff, 0, sizeof(uint16_t) * (panel_width * 32)); 
}

inline bool P3RGB64x32MatrixPanel::ittrTimerCrashCheck() {
  if (matrixStatus == false || timer_period < 40)  return true;
  else  return false;
}

#endif