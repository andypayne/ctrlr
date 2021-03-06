#ifndef CTRLR_H
#define CTRLR_H

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

// Adafruit SSD1306
// https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PJRC Encoder Library
// https://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

// https://playground.arduino.cc/Code/Metro/
#include <Metro.h>

// https://github.com/andypayne/midi_defs
#include "../../midi_defs/midi_defs.h"
// https://github.com/andypayne/buttoner
#include "../../buttoner/buttoner.h"

// SSD1306 128x32 panel
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define SW_MILLIS      450
#define SW_DBL_MILLIS  200

#define VEL_NOTE_ON   99
#define VEL_NOTE_OFF  0

#define MODE_CHG_CTL_NUM  28

#define BPM_TO_MILLIS(bpm) ((60.0 / bpm) * 1000.0)
#define METRO_BPM 120

#define NUM_SEQ_STEPS  8
#define NUM_SEQ2_STEPS  16

int cycleBtnSeq(const int btnSeq);


class Ctrlr
{
  public:
    enum btnMode {
      mnote,
      mcchg,
      mcchg2,  // Send MIDI_LOW on release
      mcchg3,  // Repeat mode
      mnseq2,  // Sequencer 2
      mnseq,   // Note sequencer mode
    };
    const int NUM_BTN_MODES = 5;

    // Damping increment for the rotary encoder mode change
    const int DAMP_INCR = 5;

    // Delay repeat max
    const int MAX_REP_COUNT = 16;
    // Default delay BPM
    const int DEF_REP_BPM = 500;

    // EEPROM addresses
    const int EADDR_TEMPO = 0;
    const int EADDR_LEN = 1;
    const int EADDR_SEQ = 2;

    btnMode defBtnMode = mnseq2;

    struct btnBehavior {
      btnMode mode;
      int noteVal;
      int chordVal[3];
      int ctlNum;
      int ctlVal;
      int repCount;
      Metro repMetro;
    };

    struct noteSpec {
      int note;
      int vel;
    };

    enum seqEditMode {
      senote,
      sevel,
      setempo,
      selen,
      sesto,
      serec
    };

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
    void drawRencIndicator(const float indX, const float indY, const float rencX, const float rencY);
    void displayControllerView();
    void printBtnStatus(const char *pre, Buttoner& bBtnr);
    void displayDebugView();
    void displaySeq2();
    void displayCurrentMode();
    void displayButtonStatus(const int pinVal, const int btnTog, const int btnX, const int btnY, const float btnSz, const float btnRad);
    void processBtn(btnBehavior& bb, Buttoner& bBtnr, int& pinVal, int& btnTog, int& btnSeq, long& btnTime);
    const btnMode relativeMode(const btnMode& mode, const int relVal);
    void doRepeats(btnBehavior& bb);
    void storeSeq();
    void recallSeq();

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
    bool _modeChanging;
    btnMode _modeSel;
    long _renc_sw_time;
    long _sw_millis;

    Buttoner _rencBtnr;
    Buttoner _b0Btnr;
    Buttoner _b1Btnr;
    Buttoner _b2Btnr;
    Buttoner _b3Btnr;
    Buttoner _b4Btnr;
    Buttoner _b5Btnr;
    Buttoner _b6Btnr;
    Buttoner _b7Btnr;

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
    int _seqStep;
    int _btn0_seq;
    int _btn1_seq;
    int _btn2_seq;
    int _btn3_seq;
    int _btn4_seq;
    int _btn5_seq;
    int _btn6_seq;
    int _btn7_seq;
    int _seqNote;
    bool _delayOn;
    bool _chordsOn;
    noteSpec _seqSteps[NUM_SEQ2_STEPS];
    int _seqEditStep;
    bool _seqPlaying;
    int _numSeq2Steps;
    seqEditMode _editMode;
};

#endif
