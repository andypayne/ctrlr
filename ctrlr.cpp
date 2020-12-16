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
    _modeChanging(false),
    _modeSel(mnote),
    _sw_millis(SW_MILLIS),
    _rencBtnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b0Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b1Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b2Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b3Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b4Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b5Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b6Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _b7Btnr(HIGH, LOW, HIGH, SW_MILLIS, SW_DBL_MILLIS),
    _bb0({ defBtnMode, 60, maj_7(60), 20, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Cmaj
    _bb1({ defBtnMode, 61, min_7(62), 21, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Dmin
    _bb2({ defBtnMode, 62, min_7(64), 22, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Emin
    _bb3({ defBtnMode, 63, maj_7(65), 23, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Fmaj
    _bb4({ defBtnMode, 64, dom_7(67), 24, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Gmaj
    _bb5({ defBtnMode, 65, min_7(69), 25, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Amin
    _bb6({ defBtnMode, 66, dim_7(71), 26, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Bmin7b5
    _bb7({ defBtnMode, 67, maj_7(72), 27, MidiDefs::MIDI_HIGH, 0, Metro(BPM_TO_MILLIS(DEF_REP_BPM)) }),  // Cmaj
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
    _metro(BPM_TO_MILLIS(METRO_BPM)),
    _seqNote(0),
    _delayOn(false),
    _chordsOn(false),
    //_seqSteps{ {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    //_seqSteps{ {60, 64}, {64, 80}, {62, 0}, {60, 10}, {66, 80}, {64, 0}, {66, 10}, {62, 80}, {60, 0}, {64, 10}, {68, 80}, {62, 0}, {62, 10}, {64, 0}, {60, 64}, {70, 90} },
    //_seqSteps{ {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0}, {60, 0} },
    _seqSteps{ {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95}, {60, 95} },
    _seqEditStep(0),
    _seqPlaying(true),
    _numSeq2Steps(NUM_SEQ2_STEPS),
    _editMode(senote)
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
  _btn0_seq = 0;
  _btn1_seq = 0;
  _btn2_seq = 0;
  _btn3_seq = 0;
  _btn4_seq = 0;
  _btn5_seq = 0;
  _btn6_seq = 0;
  _btn7_seq = 0;
}

void Ctrlr::setup() {
#if defined SSD1306_128_32
  Serial.print(F("SSD1306_128_32 is defined"));
#endif

  delay(500);

  _rencBtnr.setup();
  _b0Btnr.setup();
  _b1Btnr.setup();
  _b2Btnr.setup();
  _b3Btnr.setup();
  _b4Btnr.setup();
  _b5Btnr.setup();
  _b6Btnr.setup();
  _b7Btnr.setup();

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
  delay(750);
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

void Ctrlr::processBtn(btnBehavior& bb, Buttoner& bBtnr, int& pinVal, int& btnTog, int& btnSeq, long& btnTime) {
  if (bb.mode == mnote) {
    if (bBtnr.isPressedDown()) {
      pinVal = LOW;
      if (_delayOn) {
        bb.repCount = MAX_REP_COUNT;
        bb.repMetro.interval(BPM_TO_MILLIS(DEF_REP_BPM));
        bb.repMetro.reset();
      }
      if (_chordsOn) {
        usbMIDI.sendNoteOn(bb.chordVal[0], VEL_NOTE_ON, _midiChannel);
        usbMIDI.sendNoteOn(bb.chordVal[1], VEL_NOTE_ON, _midiChannel);
        usbMIDI.sendNoteOn(bb.chordVal[2], VEL_NOTE_ON, _midiChannel);
      } else {
        usbMIDI.sendNoteOn(bb.noteVal, VEL_NOTE_ON, _midiChannel);
      }
    } else if (bBtnr.isReleased()) {
      pinVal = HIGH;
      //usbMIDI.sendNoteOn(bb.noteVal, VEL_NOTE_OFF, _midiChannel);
    }
  } else if (bb.mode == mcchg) {
    if (bBtnr.isPressedDown()) {
      pinVal = LOW;
      usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, _midiChannel);
    } else if (bBtnr.isReleased()) {
      pinVal = HIGH;
    }
  } else if (bb.mode == mcchg2) {
    if (bBtnr.isPressedDown()) {
      pinVal = LOW;
      usbMIDI.sendControlChange(bb.ctlNum, bb.ctlVal, _midiChannel);
    } else if (bBtnr.isReleased()) {
      pinVal = HIGH;
      usbMIDI.sendControlChange(bb.ctlNum, MidiDefs::MIDI_LOW, _midiChannel);
    }
  } else if (bb.mode == mcchg3) {
    if (bBtnr.isPressedDown()) {
      pinVal = LOW;
    } else if (bBtnr.isReleased()) {
      pinVal = HIGH;
    }
    if ((long)(millis() - btnTime) >= _sw_millis && bBtnr.isPressedDown()) {
      btnTog = (btnTog == LOW) ? HIGH : LOW;
      btnTime = millis();
    }
  } else if (bb.mode == mnseq) {
    if (bBtnr.isSinglePressed()) {
      pinVal = LOW;
      btnSeq = cycleBtnSeq(btnSeq);
//    } else if (bBtnr.isSingleReleased()) {
    } else if (bBtnr.isReleased()) {
      pinVal = HIGH;
    }
  }
}

int cycleBtnSeq(const int btnSeq) {
  switch (btnSeq) {
    case 127:
      return 0;
      break;
    case 95:
      return 127;
      break;
    case 63:
      return 95;
      break;
    case 31:
      return 63;
      break;
    case 0:
      return 31;
      break;
    default:
      return 0;
      break;
  }
}

const Ctrlr::btnMode Ctrlr::relativeMode(const Ctrlr::btnMode& mode, const int relVal) {
  if (mode + relVal >= 0) {
    return (btnMode)(((int)mode + relVal) % NUM_BTN_MODES);
  } else {
    // modulo of a negative number - this works as expected in C but not in Ruby or Python
    // See: https://torstencurdt.com/tech/posts/modulo-of-negative-numbers/
    return (btnMode)((((int)mode + relVal) % NUM_BTN_MODES) + NUM_BTN_MODES);
  }
}

void Ctrlr::doRepeats(btnBehavior& bb) {
  if (bb.mode == mnote && bb.repCount > 0) {
    if (bb.repMetro.check() == 1) {
      //if (bb.repCount % 2 == 0) {
        float vel = VEL_NOTE_ON * (log(10)/(2 * (float)(2 * (1 + MAX_REP_COUNT - bb.repCount))) - 0.025 * 20.0 / MAX_REP_COUNT);
        //Serial.println(vel);
        usbMIDI.sendNoteOn(bb.noteVal, vel, _midiChannel);
      //} else {
      //  usbMIDI.sendNoteOn(bb.noteVal, VEL_NOTE_OFF, _midiChannel);
      //}
      bb.repCount -= 1;
      if (bb.repCount == 0) {
        usbMIDI.sendNoteOn(bb.noteVal, VEL_NOTE_OFF, _midiChannel);
      }
    }
  }
}

void Ctrlr::update() {
  _renc_sw_val = digitalRead(_inRencSwitch);
  _rencBtnr.setVal(_renc_sw_val);

  if (_delayOn) {
    doRepeats(_bb0);
    doRepeats(_bb1);
    doRepeats(_bb2);
    doRepeats(_bb3);
    doRepeats(_bb4);
    doRepeats(_bb5);
    doRepeats(_bb6);
    doRepeats(_bb7);
  }

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
    switch(_seqStep) {
      case 0:
          _seqNote = _bb0.noteVal;
//        if (_btn0_seq > 0) {
          usbMIDI.sendNoteOn(_bb0.noteVal, _btn0_seq, _midiChannel);
//        }
        break;
      case 1:
          _seqNote = _bb1.noteVal;
//        if (_btn1_seq > 0) {
          usbMIDI.sendNoteOn(_bb1.noteVal, _btn1_seq, _midiChannel);
//        }
        break;
      case 2:
          _seqNote = _bb2.noteVal;
//        if (_btn2_seq > 0) {
          usbMIDI.sendNoteOn(_bb2.noteVal, _btn2_seq, _midiChannel);
//        }
        break;
      case 3:
          _seqNote = _bb3.noteVal;
//        if (_btn3_seq > 0) {
          usbMIDI.sendNoteOn(_bb3.noteVal, _btn3_seq, _midiChannel);
//        }
        break;
      case 4:
          _seqNote = _bb4.noteVal;
//        if (_btn4_seq > 0) {
          usbMIDI.sendNoteOn(_bb4.noteVal, _btn4_seq, _midiChannel);
//        }
        break;
      case 5:
          _seqNote = _bb5.noteVal;
//        if (_btn5_seq > 0) {
          usbMIDI.sendNoteOn(_bb5.noteVal, _btn5_seq, _midiChannel);
//        }
        break;
      case 6:
          _seqNote = _bb6.noteVal;
//        if (_btn6_seq > 0) {
          usbMIDI.sendNoteOn(_bb6.noteVal, _btn6_seq, _midiChannel);
//        }
        break;
      case 7:
          _seqNote = _bb7.noteVal;
//        if (_btn7_seq > 0) {
          usbMIDI.sendNoteOn(_bb7.noteVal, _btn7_seq, _midiChannel);
//        }
        break;
      default:
        break;
    }
    _seqStep += 1;
    if (_seqStep == NUM_SEQ_STEPS) {
      _seqStep = 0;
    }
  } else if (_bb0.mode == mnseq2 && _metro.check() == 1 && _seqPlaying) {
    usbMIDI.sendNoteOn(_seqSteps[_seqStep].note, _seqSteps[_seqStep].vel, _midiChannel);
    _seqStep += 1;
    if (_seqStep == _numSeq2Steps) {
      _seqStep = 0;
    }
  }

  _b0Btnr.setVal(digitalRead(_in0Pin));
  _b0Btnr.update();
  _b1Btnr.setVal(digitalRead(_in1Pin));
  _b1Btnr.update();
  _b2Btnr.setVal(digitalRead(_in2Pin));
  _b2Btnr.update();
  _b3Btnr.setVal(digitalRead(_in3Pin));
  _b3Btnr.update();
  _b4Btnr.setVal(digitalRead(_in4Pin));
  _b4Btnr.update();
  _b5Btnr.setVal(digitalRead(_in5Pin));
  _b5Btnr.update();
  _b6Btnr.setVal(digitalRead(_in6Pin));
  _b6Btnr.update();
  _b7Btnr.setVal(digitalRead(_in7Pin));
  _b7Btnr.update();

  // DEBUG
  //printBtnStatus("b0- ", _b0Btnr);
  //printBtnStatus("b3- ", _b3Btnr);

  processBtn(_bb0, _b0Btnr, _pin0_val, _btn0_tog, _btn0_seq, _btn0_time);
  processBtn(_bb1, _b1Btnr, _pin1_val, _btn1_tog, _btn1_seq, _btn1_time);
  processBtn(_bb2, _b2Btnr, _pin2_val, _btn2_tog, _btn2_seq, _btn2_time);
  processBtn(_bb3, _b3Btnr, _pin3_val, _btn3_tog, _btn3_seq, _btn3_time);
  processBtn(_bb4, _b4Btnr, _pin4_val, _btn4_tog, _btn4_seq, _btn4_time);
  processBtn(_bb5, _b5Btnr, _pin5_val, _btn5_tog, _btn5_seq, _btn5_time);
  processBtn(_bb6, _b6Btnr, _pin6_val, _btn6_tog, _btn6_seq, _btn6_time);
  processBtn(_bb7, _b7Btnr, _pin7_val, _btn7_tog, _btn7_seq, _btn7_time);

  _rencBtnr.update();

  if (_rencBtnr.isSinglePressed()) {
    if (_modeSel == mnote) {
      if (!_delayOn && !_chordsOn) {
        _delayOn = true;
      } else if (_delayOn) {
        _delayOn = false;
        _chordsOn = true;
      } else if (_chordsOn) {
        _chordsOn = false;
      }
    }
  }
  if (_rencBtnr.isDoublePressed()) {
    // Switch mode
    if (_bb0.mode == mnseq) {
      _bb0.mode = mnote;
      _bb1.mode = mnote;
      _bb2.mode = mnote;
      _bb3.mode = mnote;
      _bb4.mode = mnote;
      _bb5.mode = mnote;
      _bb6.mode = mnote;
      _bb7.mode = mnote;
    } else {
      _bb0.mode = (btnMode)(_bb0.mode + 1);
      _bb1.mode = (btnMode)(_bb1.mode + 1);
      _bb2.mode = (btnMode)(_bb2.mode + 1);
      _bb3.mode = (btnMode)(_bb3.mode + 1);
      _bb4.mode = (btnMode)(_bb4.mode + 1);
      _bb5.mode = (btnMode)(_bb5.mode + 1);
      _bb6.mode = (btnMode)(_bb6.mode + 1);
      _bb7.mode = (btnMode)(_bb7.mode + 1);
    }
    usbMIDI.sendControlChange(MODE_CHG_CTL_NUM, _bb0.mode, _midiChannel);
  }
  if (_rencBtnr.isHeldStarted()) {
    //Serial.println("_HOLD_");
  }
  if (_rencBtnr.isReleased()) {
    //Serial.println("_RELEASED_");
  }

  long new_renc_val = _renc.read();

  // 16-step sequencer
  // b0, b4 - edit step fwd/bwd
  // b1 - edit note/velocity toggle
  // b2 - store/recall
  // b5 - edit tempo/length toggle
  // b3 - play/pause toggle
  if (_bb0.mode == mnseq2) {
    if (_b0Btnr.isPressedDown() || _b0Btnr.isHeld()) {
      _seqEditStep = (_seqEditStep == 0) ? _seqEditStep : _seqEditStep - 1;
    } else if (_b4Btnr.isPressedDown() || _b4Btnr.isHeld()) {
      _seqEditStep = (_seqEditStep == _numSeq2Steps - 1) ? _seqEditStep : _seqEditStep + 1;
    }
    if (_b1Btnr.isSinglePressed()) {
      _editMode = _editMode == senote ? sevel : senote;
    }
    if (_b5Btnr.isSinglePressed()) {
      _editMode = _editMode == setempo ? selen : setempo;
    }
    if (_b3Btnr.isSinglePressed()) {
      _seqPlaying = !_seqPlaying;
      if (_seqPlaying) {
        _seqStep = 0;
        _metro.reset();
      }
    }
    if (!_modeChanging) {
      if (new_renc_val != _renc_val) {
        if (new_renc_val > _renc_val) {
          if (_editMode == setempo) {
            _metroBpm = _metroBpm + (new_renc_val - _renc_val);
            _metro.interval(BPM_TO_MILLIS(_metroBpm));
            _seqStep = 0;
            _metro.reset();
          } else if (_editMode == selen) {
            _numSeq2Steps += 1;
            _numSeq2Steps = _numSeq2Steps > 16 ? 16 : _numSeq2Steps;
            _seqStep = 0;
            //_metro.reset();
          } else if (_editMode == senote) {
            _seqSteps[_seqEditStep].note += 1;
            _seqSteps[_seqEditStep].note = _seqSteps[_seqEditStep].note > 127 ? 127 : _seqSteps[_seqEditStep].note;
          } else if (_editMode == sevel) {
            _seqSteps[_seqEditStep].vel += 1;
            _seqSteps[_seqEditStep].vel = _seqSteps[_seqEditStep].vel > 127 ? 127 : _seqSteps[_seqEditStep].vel;
          }
        } else {
          if (_editMode == setempo) {
            _metroBpm = _metroBpm + (new_renc_val - _renc_val);
            _metro.interval(BPM_TO_MILLIS(_metroBpm));
            _seqStep = 0;
            _metro.reset();
          } else if (_editMode == selen) {
            _numSeq2Steps -= 1;
            _numSeq2Steps = _numSeq2Steps < 1 ? 1 : _numSeq2Steps;
            _seqStep = 0;
            //_metro.reset();
          } else if (_editMode == senote) {
            _seqSteps[_seqEditStep].note -= 1;
            _seqSteps[_seqEditStep].note = _seqSteps[_seqEditStep].note < 21 ? 21 : _seqSteps[_seqEditStep].note;
          } else if (_editMode == sevel) {
            _seqSteps[_seqEditStep].vel -= 1;
            _seqSteps[_seqEditStep].vel = _seqSteps[_seqEditStep].vel < 0 ? 0 : _seqSteps[_seqEditStep].vel;
          }
        }

        _renc_val = new_renc_val;
      }
    }
  }

  if (_rencBtnr.isHeld()) {
    _modeChanging = true;
    if (new_renc_val != _renc_val) {
      int dampedVal = 0;
      if (new_renc_val > DAMP_INCR + _renc_val) {
        dampedVal = 1;
      } else if (new_renc_val < _renc_val - DAMP_INCR) {
        dampedVal = -1;
      }
      if (dampedVal != 0) {
        _modeSel = relativeMode(_modeSel, new_renc_val > _renc_val ? 1 : -1);
        _renc_val = new_renc_val;
      }
    }
  } else if (_rencBtnr.isHoldReleased()) {
    _modeChanging = false;
    // Change the mode for all buttons
    _bb0.mode = _modeSel;
    _bb1.mode = _modeSel;
    _bb2.mode = _modeSel;
    _bb3.mode = _modeSel;
    _bb4.mode = _modeSel;
    _bb5.mode = _modeSel;
    _bb6.mode = _modeSel;
    _bb7.mode = _modeSel;
  } else {
    if (_bb0.mode == mnseq && _b0Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb0.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb1.mode == mnseq && _b1Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb1.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb2.mode == mnseq && _b2Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb2.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb3.mode == mnseq && _b3Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb3.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb4.mode == mnseq && _b4Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb4.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb5.mode == mnseq && _b5Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb5.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb6.mode == mnseq && _b6Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb6.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if (_bb7.mode == mnseq && _b7Btnr.isHeld()) {
      // Note val
      if (new_renc_val != _renc_val) {
        _bb7.noteVal += (new_renc_val - _renc_val);
        _renc_val = new_renc_val;
      }
    } else if ((_bb0.mode == mcchg3 && _pin0_val == LOW) || _bb0.mode == mnseq) {
      // Tempo mode
      if (new_renc_val != _renc_val) {
        _metroBpm = _metroBpm + (new_renc_val - _renc_val);
        _metro.interval(BPM_TO_MILLIS(_metroBpm));
        _metro.reset();
        _renc_val = new_renc_val;
      }
    } else if (!_modeChanging) {
      // Pitch bend mode
      if (new_renc_val != _renc_val) {
        _renc_pitch_val = _renc_pitch_val + (new_renc_val - _renc_val);
        usbMIDI.sendPitchBend(_renc_pitch_val, _midiChannel);
        _renc_val = new_renc_val;
      }
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

  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);

  // Buttons
  float btnSz = 11;
  float btnRad = 2;
  // Encoder
  int encRad = 10;

  if (_bb0.mode == mnseq2) {
    _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 15);
    //displayCurrentMode();
    _display.print(F(MidiDefs::noteNames(_seqSteps[_seqStep].note)));
    _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 3);
    _display.print(F(MidiDefs::noteNames(_seqSteps[_seqEditStep].note)));
    if (_seqPlaying) {
      _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 + 9);
      switch(_editMode) {
        case senote:
          _display.print("NT");
          break;
        case sevel:
          _display.print("VL");
          break;
        case setempo:
          _display.print("TP");
          break;
        case selen:
          _display.print("LN");
          break;
        default:
          _display.print(_seqSteps[_seqEditStep].vel);
          break;
      }

    } else {
      // Pause symbol
      _display.fillRect(5.55 * (marX + btnSz) + encRad + 1, _display.height() / 2 + 9, 3, 7, SSD1306_WHITE);
      _display.fillRect(5.55 * (marX + btnSz) + encRad + 1 + 6, _display.height() / 2 + 9, 3, 7, SSD1306_WHITE);
    }
    displaySeq2();
    _display.display();
    return;
  }

  if (_bb0.mode == mnseq) {
    float btn0SeqSz = 0.75 * _btn0_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn1SeqSz = 0.75 * _btn1_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn2SeqSz = 0.75 * _btn2_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn3SeqSz = 0.75 * _btn3_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn4SeqSz = 0.75 * _btn4_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn5SeqSz = 0.75 * _btn5_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn6SeqSz = 0.75 * _btn6_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn7SeqSz = 0.75 * _btn7_seq / 127.0 * btnSz + 0.25 * btnSz;
    float btn0X = 0 * (marX + btnSz) + 0.5 * (btnSz - btn0SeqSz) + marX;
    float btn0Y = 0 * (marY + btnSz) + 0.5 * (btnSz - btn0SeqSz) + marY;
    float btn1X = 1 * (marX + btnSz) + 0.5 * (btnSz - btn1SeqSz) + marX;
    float btn1Y = 0 * (marY + btnSz) + 0.5 * (btnSz - btn1SeqSz) + marY;
    float btn2X = 2 * (marX + btnSz) + 0.5 * (btnSz - btn2SeqSz) + marX;
    float btn2Y = 0 * (marY + btnSz) + 0.5 * (btnSz - btn2SeqSz) + marY;
    float btn3X = 3 * (marX + btnSz) + 0.5 * (btnSz - btn3SeqSz) + marX;
    float btn3Y = 0 * (marY + btnSz) + 0.5 * (btnSz - btn3SeqSz) + marY;
    float btn4X = 0 * (marX + btnSz) + 0.5 * (btnSz - btn4SeqSz) + marX;
    float btn4Y = 1 * (marY + btnSz) + 0.5 * (btnSz - btn4SeqSz) + marY;
    float btn5X = 1 * (marX + btnSz) + 0.5 * (btnSz - btn5SeqSz) + marX;
    float btn5Y = 1 * (marY + btnSz) + 0.5 * (btnSz - btn5SeqSz) + marY;
    float btn6X = 2 * (marX + btnSz) + 0.5 * (btnSz - btn6SeqSz) + marX;
    float btn6Y = 1 * (marY + btnSz) + 0.5 * (btnSz - btn6SeqSz) + marY;
    float btn7X = 3 * (marX + btnSz) + 0.5 * (btnSz - btn7SeqSz) + marX;
    float btn7Y = 1 * (marY + btnSz) + 0.5 * (btnSz - btn7SeqSz) + marY;
    displayButtonStatus(_pin0_val, _seqStep == 0 ? LOW : HIGH, btn0X, btn0Y, btn0SeqSz, btnRad);
    displayButtonStatus(_pin1_val, _seqStep == 1 ? LOW : HIGH, btn1X, btn1Y, btn1SeqSz, btnRad);
    displayButtonStatus(_pin2_val, _seqStep == 2 ? LOW : HIGH, btn2X, btn2Y, btn2SeqSz, btnRad);
    displayButtonStatus(_pin3_val, _seqStep == 3 ? LOW : HIGH, btn3X, btn3Y, btn3SeqSz, btnRad);
    displayButtonStatus(_pin4_val, _seqStep == 4 ? LOW : HIGH, btn4X, btn4Y, btn4SeqSz, btnRad);
    displayButtonStatus(_pin5_val, _seqStep == 5 ? LOW : HIGH, btn5X, btn5Y, btn5SeqSz, btnRad);
    displayButtonStatus(_pin6_val, _seqStep == 6 ? LOW : HIGH, btn6X, btn6Y, btn6SeqSz, btnRad);
    displayButtonStatus(_pin7_val, _seqStep == 7 ? LOW : HIGH, btn7X, btn7Y, btn7SeqSz, btnRad);
  } else {
    float btn0X = marX;
    float btn0Y = marY;
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
  float rencX = 4.5 * (marX + btnSz) + encRad;
  float rencY = _display.height() / 2;
  if (_renc_sw_val == LOW) {
    _display.fillCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  } else {
    _display.drawCircle(rencX, rencY, 0.5 * encRad, SSD1306_WHITE);
  }

  if (!_modeChanging && _bb0.mode != mnseq) {
    // Pitch renc
    float indPitchX = encRad * cos(2 * PI * _renc_pitch_val / 80.0 - PI / 2);
    float indPitchY = encRad * sin(2 * PI * _renc_pitch_val / 80.0 - PI / 2);
    drawRencIndicator(indPitchX, indPitchY, rencX, rencY);
  }

  if (!_modeChanging && ((_bb0.mode == mcchg3 && _pin0_val == LOW) || _bb0.mode == mnseq || _bb0.mode == mnseq2)) {
    // Metro renc
    float indMetroX = encRad * cos(2 * PI * _metroBpm / 80.0 - PI / 2);
    float indMetroY = encRad * sin(2 * PI * _metroBpm / 80.0 - PI / 2);
    drawRencIndicator(indMetroX, indMetroY, rencX, rencY);
  }

  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 3);
  if (_bb0.mode == mnseq && _b0Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb0.noteVal)));
  } else if (_bb1.mode == mnseq && _b1Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb1.noteVal)));
  } else if (_bb2.mode == mnseq && _b2Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb2.noteVal)));
  } else if (_bb3.mode == mnseq && _b3Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb3.noteVal)));
  } else if (_bb4.mode == mnseq && _b4Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb4.noteVal)));
  } else if (_bb5.mode == mnseq && _b5Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb5.noteVal)));
  } else if (_bb6.mode == mnseq && _b6Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb6.noteVal)));
  } else if (_bb7.mode == mnseq && _b7Btnr.isHeld()) {
    _display.print(F(MidiDefs::noteNames(_bb7.noteVal)));
  } else if (_bb0.mode == mnseq) {
    _display.print(F(MidiDefs::noteNames(_seqNote)));
  } else {
    _display.print(_renc_pitch_val);
  }

  _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 - 15);
  displayCurrentMode();

  if (_bb0.mode == mcchg3 || _bb0.mode == mnseq) {
    _display.setCursor(5.55 * (marX + btnSz) + encRad, _display.height() / 2 + 8);
    _display.print(_metroBpm);
  }

  _display.display();
}

void Ctrlr::displaySeq2() {
  float sidebarWidth = 24.0;
  float stepWidth = (_display.width() - sidebarWidth) / _numSeq2Steps;
  float minNote = 127;
  float maxNote = 0;
  float minVel = 127;
  float maxVel = 0;
  for (int i = 0; i < _numSeq2Steps; ++i) {
    if (_seqSteps[i].note > maxNote) {
      maxNote = _seqSteps[i].note;
    } else if (_seqSteps[i].note < minNote) {
      minNote = _seqSteps[i].note;
    }
    if (_seqSteps[i].vel > maxVel) {
      maxVel = _seqSteps[i].vel;
    } else if (_seqSteps[i].vel < minVel) {
      minVel = _seqSteps[i].vel;
    }
  }
  for (int i = 0; i < _numSeq2Steps; ++i) {
    //float yNoteLvl = _display.height() - (_seqSteps[i].note - 60);
    //float yVelLvl = _display.height() - _display.height() * (_seqSteps[i].vel / 127.0);
    // Scale display values by the range
    float yNoteLvl = _display.height() - _display.height() / (maxNote - minNote) * (_seqSteps[i].note - minNote);
    float yVelLvl = _display.height() - _display.height() / (maxVel - minVel) * (_seqSteps[i].vel - minVel);
    float stepHeight = i == _seqStep ? 3 : 1;
    _display.fillRoundRect(i * stepWidth, yNoteLvl, stepWidth - 2, stepHeight, 2, SSD1306_WHITE);
    _display.drawRect(i * stepWidth + 0.5 * stepWidth - 2, yVelLvl, 2, 1, SSD1306_WHITE);
    if (i == _seqEditStep) {
      _display.drawRect(i * stepWidth, _display.height() - 1, stepWidth - 2, 1, SSD1306_WHITE);
    }
  }
}

void Ctrlr::displayCurrentMode() {
  if (_modeChanging) {
    switch(_modeSel) {
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
      case mnseq2:
        _display.print(F("NS2"));
        break;
      default:
        break;
    }
  } else {
    switch(_bb0.mode) {
      case mnote:
        _display.print(F("N"));
        if (_delayOn) {
          _display.print(F(" DL"));
        } else if (_chordsOn) {
          _display.print(F(" CH"));
        }
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
      case mnseq2:
        _display.print(F("NS2"));
        break;
      default:
        break;
    }
  }
}

void Ctrlr::displayButtonStatus(const int pinVal, const int btnTog, const int btnX, const int btnY, const float btnSz, const float btnRad) {
  if (pinVal == LOW || btnTog == LOW) {
    _display.fillRoundRect(btnX, btnY, btnSz, btnSz, btnRad, SSD1306_WHITE);
  } else {
    _display.drawRoundRect(btnX, btnY, btnSz, btnSz, btnRad, SSD1306_WHITE);
  }
}

void Ctrlr::printBtnStatus(const char *pre, Buttoner& bBtnr) {
  const char *bStatus = bBtnr.statusToStr();
  if (strcmp(bStatus, "")) {
    Serial.print(pre);
    Serial.println(bStatus);
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
