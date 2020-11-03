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
    _bb0({ defBtnMode, 60, 20, MIDI_HIGH }),
    _bb1({ defBtnMode, 61, 21, MIDI_HIGH }),
    _bb2({ defBtnMode, 62, 22, MIDI_HIGH }),
    _bb3({ defBtnMode, 63, 23, MIDI_HIGH }),
    _bb4({ defBtnMode, 64, 24, MIDI_HIGH }),
    _bb5({ defBtnMode, 65, 25, MIDI_HIGH }),
    _bb6({ defBtnMode, 66, 26, MIDI_HIGH }),
    _bb7({ defBtnMode, 67, 27, MIDI_HIGH }),
    _btn0_tog(HIGH),
    _btn1_tog(HIGH),
    _btn2_tog(HIGH),
    _btn3_tog(HIGH),
    _btn4_tog(HIGH),
    _btn5_tog(HIGH),
    _btn6_tog(HIGH),
    _btn7_tog(HIGH),
    _renc(inRencDT, inRencCLK),
    _display(
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      OLED_MOSI,
      OLED_CLK,
      OLED_DC,
      OLED_RESET,
      OLED_CS),
    _metroBpm(METRO_BPM),
    _metro(BPM_TO_MILLIS(METRO_BPM))
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
  _renc_pitch_val = 0;
  _renc_sw_time = 0;
  _btn0_time = 0;
  _btn1_time = 0;
  _btn2_time = 0;
  _btn3_time = 0;
  _btn4_time = 0;
  _btn5_time = 0;
  _btn6_time = 0;
  _btn7_time = 0;

  _seqStep = 0;
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

void Ctrlr::btnOnEdge(Bounce &btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, btnCallback cb) {
  if (btn.fallingEdge()) {
    cb(btn, bb, pinVal, btnTog, btnTime, swMillis, midiChannel, true);
  } else if (btn.risingEdge()) {
    cb(btn, bb, pinVal, btnTog, btnTime, swMillis, midiChannel, false);
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

  if (_bb0.mode == mcchg3 && _metro.check() == 1) {
    if (_btn0_tog == LOW) {
      usbMIDI.sendControlChange(_bb0.ctlNum, _bb0.ctlVal, _midiChannel);
    }
    if (_btn1_tog == LOW) {
      usbMIDI.sendControlChange(_bb1.ctlNum, _bb1.ctlVal, _midiChannel);
    }
    if (_btn2_tog == LOW) {
      usbMIDI.sendControlChange(_bb2.ctlNum, _bb2.ctlVal, _midiChannel);
    }
    if (_btn3_tog == LOW) {
      usbMIDI.sendControlChange(_bb3.ctlNum, _bb3.ctlVal, _midiChannel);
    }
    if (_btn4_tog == LOW) {
      usbMIDI.sendControlChange(_bb4.ctlNum, _bb4.ctlVal, _midiChannel);
    }
    if (_btn5_tog == LOW) {
      usbMIDI.sendControlChange(_bb5.ctlNum, _bb5.ctlVal, _midiChannel);
    }
    if (_btn6_tog == LOW) {
      usbMIDI.sendControlChange(_bb6.ctlNum, _bb6.ctlVal, _midiChannel);
    }
    if (_btn7_tog == LOW) {
      usbMIDI.sendControlChange(_bb7.ctlNum, _bb7.ctlVal, _midiChannel);
    }
  } else if (_bb0.mode == mnseq && _metro.check() == 1) {
    _seqStep += 1;
    if (_seqStep == NUM_SEQ_STEPS) {
      _seqStep = 0;
    }
  }

  btnOnEdge(_btn0, _bb0, _pin0_val, _btn0_tog, _btn0_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn1, _bb1, _pin1_val, _btn1_tog, _btn1_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn2, _bb2, _pin2_val, _btn2_tog, _btn2_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn3, _bb3, _pin3_val, _btn3_tog, _btn3_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn4, _bb4, _pin4_val, _btn4_tog, _btn4_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn5, _bb5, _pin5_val, _btn5_tog, _btn5_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn6, _bb6, _pin6_val, _btn6_tog, _btn6_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });
  btnOnEdge(_btn7, _bb7, _pin7_val, _btn7_tog, _btn7_time, _sw_millis, _midiChannel, [](Bounce& btn, const btnBehavior& bb, int& pinVal, int& btnTog, long& btnTime, const long& swMillis, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    if (bb.mode == mnote) {
      usbMIDI.sendNoteOn(bb.noteVal, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);
    } else if (bb.mode == mcchg) {
      if (fall) {
        usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, midiChannel);
      }
    } else if (bb.mode == mcchg2) {
      int ctlVal = fall ? bb.ctlVal : MIDI_LOW;
      usbMIDI.sendControlChange(bb.ctlNum, ctlVal, midiChannel);
    } else if (bb.mode == mcchg3 && (long)(millis() - btnTime) >= swMillis) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  });

  if (_renc_sw_val == LOW && (long)(millis() - _renc_sw_time) >= _sw_millis) {
    _renc_sw_time = millis();
    switch(_bb0.mode) {
      case mnote:
        _bb0.mode = mcchg;
        _bb1.mode = mcchg;
        _bb2.mode = mcchg;
        _bb3.mode = mcchg;
        _bb4.mode = mcchg;
        _bb5.mode = mcchg;
        _bb6.mode = mcchg;
        _bb7.mode = mcchg;
        break;
      case mcchg:
        _bb0.mode = mcchg2;
        _bb1.mode = mcchg2;
        _bb2.mode = mcchg2;
        _bb3.mode = mcchg2;
        _bb4.mode = mcchg2;
        _bb5.mode = mcchg2;
        _bb6.mode = mcchg2;
        _bb7.mode = mcchg2;
        break;
      case mcchg2:
        _bb0.mode = mcchg3;
        _bb1.mode = mcchg3;
        _bb2.mode = mcchg3;
        _bb3.mode = mcchg3;
        _bb4.mode = mcchg3;
        _bb5.mode = mcchg3;
        _bb6.mode = mcchg3;
        _bb7.mode = mcchg3;
        break;
      case mcchg3:
        _bb0.mode = mnseq;
        _bb1.mode = mnseq;
        _bb2.mode = mnseq;
        _bb3.mode = mnseq;
        _bb4.mode = mnseq;
        _bb5.mode = mnseq;
        _bb6.mode = mnseq;
        _bb7.mode = mnseq;
        break;
      case mnseq:
        _bb0.mode = mnote;
        _bb1.mode = mnote;
        _bb2.mode = mnote;
        _bb3.mode = mnote;
        _bb4.mode = mnote;
        _bb5.mode = mnote;
        _bb6.mode = mnote;
        _bb7.mode = mnote;
        break;
      default:
        break;
    }

    usbMIDI.sendControlChange(MODE_CHG_CTL_NUM, _bb0.mode, _midiChannel);
  }

  long new_renc_val = _renc.read();
  if ((_bb0.mode == mcchg3 && _pin0_val == LOW) || _bb0.mode == mnseq) {
    // Tempo mode
    if (new_renc_val != _renc_val) {
      _metroBpm = _metroBpm + (new_renc_val - _renc_val);
      _metro.interval(BPM_TO_MILLIS(_metroBpm));
      _metro.reset();
      _renc_val = new_renc_val;
    }
  } else {
    // Pitch bend mode
    if (new_renc_val != _renc_val) {
      _renc_pitch_val = _renc_pitch_val + (new_renc_val - _renc_val);
      usbMIDI.sendPitchBend(_renc_pitch_val, _midiChannel);
      _renc_val = new_renc_val;
    }
  }

  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) { }

  displayControllerView();
  //displayDebugView();
}

void Ctrlr::drawRencIndicator(const float indX, const float indY, const float rencX, const float rencY) {
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
  if (_bb0.mode == mnseq) {
    displayButtonStatus(_pin0_val, _seqStep == 0 ? LOW : HIGH, btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin1_val, _seqStep == 1 ? LOW : HIGH, 1 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin2_val, _seqStep == 2 ? LOW : HIGH, 2 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin3_val, _seqStep == 3 ? LOW : HIGH, 3 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin4_val, _seqStep == 4 ? LOW : HIGH, btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin5_val, _seqStep == 5 ? LOW : HIGH, 1 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin6_val, _seqStep == 6 ? LOW : HIGH, 2 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin7_val, _seqStep == 7 ? LOW : HIGH, 3 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
  } else {
    displayButtonStatus(_pin0_val, _bb0.mode == mcchg3 ? _btn0_tog : HIGH, btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin1_val, _bb1.mode == mcchg3 ? _btn1_tog : HIGH, 1 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin2_val, _bb2.mode == mcchg3 ? _btn2_tog : HIGH, 2 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin3_val, _bb3.mode == mcchg3 ? _btn3_tog : HIGH, 3 * (marX + btnSz) + btn0X, btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin4_val, _bb4.mode == mcchg3 ? _btn4_tog : HIGH, btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin5_val, _bb5.mode == mcchg3 ? _btn5_tog : HIGH, 1 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin6_val, _bb6.mode == mcchg3 ? _btn6_tog : HIGH, 2 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
    displayButtonStatus(_pin7_val, _bb7.mode == mcchg3 ? _btn7_tog : HIGH, 3 * (marX + btnSz) + btn0X, marY + btnSz + btn0Y, btnSz, btnRad);
  }

  // Encoder
  int encRad = 10;
  float rencX = 4.5 * (marX + btnSz) + encRad;
  float rencY = _display.height() / 2;
  if (_renc_sw_val == LOW) {
    _display.fillCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  } else {
    _display.drawCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  }

  // Pitch renc
  if (_bb0.mode != mnseq) {
    float indPitchX = encRad * cos(2 * PI * _renc_pitch_val / 80.0 - PI / 2);
    float indPitchY = encRad * sin(2 * PI * _renc_pitch_val / 80.0 - PI / 2);
    drawRencIndicator(indPitchX, indPitchY, rencX, rencY);
  }

  if ((_bb0.mode == mcchg3 && _pin0_val == LOW) || _bb0.mode == mnseq) {
    // Metro renc
    float indMetroX = encRad * cos(2 * PI * _metroBpm / 80.0 - PI / 2);
    float indMetroY = encRad * sin(2 * PI * _metroBpm / 80.0 - PI / 2);
    drawRencIndicator(indMetroX, indMetroY, rencX, rencY);
  }

  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 3);
  _display.print(_renc_pitch_val);

  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 15);
  switch(_bb0.mode) {
    case mnote:
      _display.print(F("N"));
      break;
    case mcchg:
      _display.print(F("C1"));
      break;
    case mcchg2:
      _display.print(F("C2"));
      break;
    case mcchg3:
      _display.print(F("C3"));
      break;
    case mnseq:
      _display.print(F("NS"));
      break;
    default:
      break;
  }

  if (_bb0.mode == mcchg3 || _bb0.mode == mnseq) {
    _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 + 8);
    _display.print(_metroBpm);
  }

  _display.display();
}

void Ctrlr::displayButtonStatus(const int pinVal, const int btnTog, const int btnX, const int btnY, const float btnSz, const float btnRad) {
  if (pinVal == LOW || btnTog == LOW) {
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
