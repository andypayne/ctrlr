#ifndef CTRLR_H
#define CTRLR_H

#include <SPI.h>
#include <Wire.h>

// Adafruit SSD1306
// https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PJRC Bounce Library
// https://www.pjrc.com/teensy/td_libs_Bounce.html
#include <Bounce.h>

// PJRC Encoder Library
// https://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

// https://playground.arduino.cc/Code/Metro/
#include <Metro.h>

// SSD1306 128x32 panel
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define VEL_NOTE_ON   99
#define VEL_NOTE_OFF  0

#define MIDI_HIGH 127
#define MIDI_LOW    0

#define MODE_CHG_CTL_NUM  28

#define BPM_TO_MILLIS(bpm) ((60.0 / bpm) * 1000)
#define METRO_BPM 120

class Ctrlr
{
  public:
    enum btnMode {
      mnote,
      mcchg,
      mcchg2,  // Send MIDI_LOW on release
      mcchg3   // Repeat mode
    };

    btnMode defBtnMode = mnote;

    struct btnBehavior {
      btnMode mode;
      int noteVal;
      int ctlNum;
      int ctlVal;
    };

    using btnCallback = void (*)(Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall);

    Ctrlr(
      int midiChannel,
      int in0Pin,
      int in1Pin,
      int in2Pin,
      int in3Pin,
      int in4Pin,
      int in5Pin,
      int in6Pin,
      int in7Pin,
      int inRencSwitch,
      int inRencDT,
      int inRencClk
    );

    void setup();
    void update();
    void displayBootScreen();
    void drawBootRect(const int x, const int y, const int delayMs);
    void displayDebugView();
    void drawRencIndicator(const float indX, const float indY, const float rencX, const float rencY);
    void displayControllerView();
    void displayButtonStatus(const int pinVal, const int btnTog, const int btnX, const int btnY, const float btnSz, const float btnRad);
    void btnOnEdge(Bounce &btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, btnCallback cb);

  private:
    // the MIDI channel number to send messages
    int _midiChannel;

    int _in0Pin;
    int _in1Pin;
    int _in2Pin;
    int _in3Pin;
    int _in4Pin;
    int _in5Pin;
    int _in6Pin;
    int _in7Pin;
    int _inRencSwitch;
    int _inRencDT;
    int _inRencCLK;

    int _pin0_val;
    int _pin1_val;
    int _pin2_val;
    int _pin3_val;
    int _pin4_val;
    int _pin5_val;
    int _pin6_val;
    int _pin7_val;
    int _renc_sw_val;
    long _renc_val;
    long _renc_pitch_val;
    long _renc_sw_time;
    long _sw_millis;

    Bounce _btn0;
    Bounce _btn1;
    Bounce _btn2;
    Bounce _btn3;
    Bounce _btn4;
    Bounce _btn5;
    Bounce _btn6;
    Bounce _btn7;
    btnBehavior _bb0;
    btnBehavior _bb1;
    btnBehavior _bb2;
    btnBehavior _bb3;
    btnBehavior _bb4;
    btnBehavior _bb5;
    btnBehavior _bb6;
    btnBehavior _bb7;
    int _btn0_tog;
    int _btn1_tog;
    int _btn2_tog;
    int _btn3_tog;
    int _btn4_tog;
    int _btn5_tog;
    int _btn6_tog;
    int _btn7_tog;
    long _btn0_time;
    long _btn1_time;
    long _btn2_time;
    long _btn3_time;
    long _btn4_time;
    long _btn5_time;
    long _btn6_time;
    long _btn7_time;
    Encoder _renc;
    Adafruit_SSD1306 _display;
    int _metroBpm;
    Metro _metro;
};

#endif
