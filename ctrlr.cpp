#include "ctrlr.h"

Ctrlr::Ctrlr(
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
  int inRencCLK) :
    _sw_millis(500),
    _btn0(in0Pin, 5),
    _btn1(in1Pin, 5),
    _btn2(in2Pin, 5),
    _btn3(in3Pin, 5),
    _btn4(in4Pin, 5),
    _btn5(in5Pin, 5),
    _btn6(in6Pin, 5),
    _btn7(in7Pin, 5),
    _bb0({ defBtnMode, 60, 20, MIDI_HIGH, false }),
    _bb1({ defBtnMode, 61, 21, MIDI_HIGH, false }),
    _bb2({ defBtnMode, 62, 22, MIDI_HIGH, false }),
    _bb3({ defBtnMode, 63, 23, MIDI_HIGH, false }),
    _bb4({ defBtnMode, 64, 24, MIDI_HIGH, false }),
    _bb5({ defBtnMode, 65, 25, MIDI_HIGH, false }),
    _bb6({ defBtnMode, 66, 26, MIDI_HIGH, false }),
    _bb7({ defBtnMode, 67, 27, MIDI_HIGH, false }),
    _renc(inRencDT, inRencCLK),
    _display(
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      OLED_MOSI,
      OLED_CLK,
      OLED_DC,
      OLED_RESET,
      OLED_CS)
{
  _midiChannel = midiChannel;
  _in0Pin = in0Pin;
  _in1Pin = in1Pin;
  _in2Pin = in2Pin;
  _in3Pin = in3Pin;
  _in4Pin = in4Pin;
  _in5Pin = in5Pin;
  _in6Pin = in6Pin;
  _in7Pin = in7Pin;
  _inRencSwitch = inRencSwitch;
  _inRencDT = inRencDT;
  _inRencCLK = inRencCLK;

  pinMode(_inRencSwitch, INPUT);
  pinMode(_in0Pin, INPUT_PULLUP);
  pinMode(_in1Pin, INPUT_PULLUP);
  pinMode(_in2Pin, INPUT_PULLUP);
  pinMode(_in3Pin, INPUT_PULLUP);
  pinMode(_in4Pin, INPUT_PULLUP);
  pinMode(_in5Pin, INPUT_PULLUP);
  pinMode(_in6Pin, INPUT_PULLUP);
  pinMode(_in7Pin, INPUT_PULLUP);

  _pin0_val = HIGH;
  _pin1_val = HIGH;
  _pin2_val = HIGH;
  _pin3_val = HIGH;
  _pin4_val = HIGH;
  _pin5_val = HIGH;
  _pin6_val = HIGH;
  _pin7_val = HIGH;
  _renc_sw_val = 0;
  _renc_val = 0;
  _renc_sw_time = 0;
}

void Ctrlr::setup() {
#if defined SSD1306_128_32
  Serial.print(F("SSD1306_128_32 is defined"));
#endif

  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  _display.clearDisplay();
  _display.display();
  displayBootScreen();
}

void Ctrlr::displayBootScreen() {
  _display.clearDisplay();
  for(int16_t i = _display.height() / 2 - 1; i >= 0; i -= 2) {
    drawBootRect(i, i, 30);
    _display.clearDisplay();
  }
  for(int16_t i = 0; i < _display.height() / 2; i += 2) {
    drawBootRect(i, i, 30);
    _display.clearDisplay();
  }
  for(int16_t i = _display.height() / 2 - 1; i >= 0; i -= 2) {
    drawBootRect(i, i, 30);
    _display.clearDisplay();
  }
  for(int16_t i = 0; i < _display.height() / 2; i += 2) {
    drawBootRect(i, i, 30);
    _display.clearDisplay();
  }
  for(int16_t i = _display.height() / 2 - 1; i >= 0; i -= 2) {
    drawBootRect(i, i, 30);
    if (i > 1) {
      _display.clearDisplay();
      _display.display();
    }
  }
  _display.setTextSize(2);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(36, 9);
  _display.cp437(true);
  _display.println(F("CTRLR"));
  _display.display();
  delay(1200);
  _display.clearDisplay();
  for(int16_t i = 0; i < _display.height() / 2; i += 2) {
    drawBootRect(i, i, 16);
    _display.clearDisplay();
  }
  _display.clearDisplay();
  _display.display();
  delay(200);
}

void Ctrlr::drawBootRect(const int x, const int y, const int delayMs) {
  _display.drawRect(x, y, _display.width() - 2 * x, _display.height() - 2 * y, SSD1306_WHITE);
  _display.display();
  delay(delayMs);
}

void Ctrlr::btnOnEdge(Bounce &btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, btnCallback cb) {
  if (btn.fallingEdge()) {
    cb(btn, bb, pinVal, midiChannel, true);
  } else if (btn.risingEdge()) {
    cb(btn, bb, pinVal, midiChannel, false);
  }
}

void Ctrlr::update() {
  _renc_sw_val = digitalRead(_inRencSwitch);
  _btn0.update();
  _btn1.update();
  _btn2.update();
  _btn3.update();
  _btn4.update();
  _btn5.update();
  _btn6.update();
  _btn7.update();

  btnOnEdge(_btn0, _bb0, _pin0_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn1, _bb1, _pin1_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn2, _bb2, _pin2_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn3, _bb3, _pin3_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn4, _bb4, _pin4_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn5, _bb5, _pin5_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn6, _bb6, _pin6_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });
  btnOnEdge(_btn7, _bb7, _pin7_val, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, const int& midiChannel, const bool fall) {
    if (bb.mode == mnote) {
      pinVal = fall ? LOW : HIGH;
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      int ctlVal = fall ? bb.ctlVal : (bb.midiLowOnRelease ? MIDI_LOW : bb.ctlVal);
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    }
  });

  if (_renc_sw_val == LOW && (long)(millis() - _renc_sw_time) >= _sw_millis) {
    _renc_sw_time = millis();
    _bb0.mode = (_bb0.mode == mnote) ? mcchg : mnote;
    _bb1.mode = (_bb1.mode == mnote) ? mcchg : mnote;
    _bb2.mode = (_bb2.mode == mnote) ? mcchg : mnote;
    _bb3.mode = (_bb3.mode == mnote) ? mcchg : mnote;
    _bb4.mode = (_bb4.mode == mnote) ? mcchg : mnote;
    _bb5.mode = (_bb5.mode == mnote) ? mcchg : mnote;
    _bb6.mode = (_bb6.mode == mnote) ? mcchg : mnote;
    _bb7.mode = (_bb7.mode == mnote) ? mcchg : mnote;
  }

  long new_renc_val = _renc.read();
  if (new_renc_val != _renc_val) {
    _renc_val = new_renc_val;
    usbMIDI.sendPitchBend(_renc_val, _midiChannel);
  }

  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) { }

  displayControllerView();
  //displayDebugView();
}

void Ctrlr::displayControllerView() {
  _display.clearDisplay();

  int marX = 6;
  int marY = 4;

  // Buttons
  float btnSz = 11;
  float btnRad = 2;
  float btn0X = marX;
  float btn0Y = marY;
  displayButtonStatus(_pin0_val, btn0X, btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin1_val, 1 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin2_val, 2 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin3_val, 3 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin4_val, btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin5_val, 1 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin6_val, 2 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
  displayButtonStatus(_pin7_val, 3 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);

  // Encoder
  int encRad = 10;
  //float rencX = _display.width() - encRad - marX - 24;
  float rencX = 4.5 * (marX + btnSz) + encRad;
  float rencY = _display.height() / 2;
  if (_renc_sw_val == LOW) {
    _display.fillCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  } else {
    _display.drawCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  }

  float indX = encRad * cos(2 * PI * _renc_val / 80.0 - PI / 2);
  float indY = encRad * sin(2 * PI * _renc_val / 80.0 - PI / 2);

  float ind1X = 0.9 * indX + rencX;
  float ind1Y = 0.9 * indY + rencY;
  _display.drawPixel(ind1X, ind1Y, SSD1306_WHITE);
  float ind2X = 1.0 * indX + rencX;
  float ind2Y = 1.0 * indY + rencY;
  _display.drawPixel(ind2X, ind2Y, SSD1306_WHITE);
  float ind3X = 1.1 * indX + rencX;
  float ind3Y = 1.1 * indY + rencY;
  _display.drawPixel(ind3X, ind3Y, SSD1306_WHITE);
  float ind4X = 1.2 * indX + rencX;
  float ind4Y = 1.2 * indY + rencY;
  _display.drawPixel(ind4X, ind4Y, SSD1306_WHITE);
  float ind5X = 1.3 * indX + rencX;
  float ind5Y = 1.3 * indY + rencY;
  _display.drawPixel(ind5X, ind5Y, SSD1306_WHITE);

  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 3);
  _display.print(_renc_val);

  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 15);
  _display.print(F(_bb0.mode == mnote ? "N" : "C"));

  _display.display();
}

void Ctrlr::displayButtonStatus(const int pinVal, const int btnX, const int btnY, const float btnSz, const float btnRad) {
  if (pinVal == LOW) {
    _display.fillRoundRect(btnX, btnY, btnSz, btnSz, btnRad, SSD1306_WHITE);
  } else {
    _display.drawRoundRect(btnX, btnY, btnSz, btnSz, btnRad, SSD1306_WHITE);
  }
}

void Ctrlr::displayDebugView() {
  _display.clearDisplay();
  _display.setTextSize(2);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0,0);

  _display.print(F(_pin0_val == LOW ? "*" : "o"));
  _display.print(F(_pin1_val == LOW ? "*" : "o"));
  _display.print(F(_pin2_val == LOW ? "*" : "o"));
  _display.print(F(_pin3_val == LOW ? "*" : "o"));
  _display.print(F(_pin4_val == LOW ? "*" : "o"));
  _display.print(F(_pin5_val == LOW ? "*" : "o"));
  _display.print(F(_pin6_val == LOW ? "*" : "o"));
  _display.print(F(_pin7_val == LOW ? "*" : "o"));
  _display.println(F(_renc_sw_val == LOW ? "*" : "o"));

  _display.print(F("RENC "));
  _display.println(_renc_val);
  _display.display();
}
