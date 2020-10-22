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
    _btn0(in0Pin, 5),
    _btn1(in1Pin, 5),
    _btn2(in2Pin, 5),
    _btn3(in3Pin, 5),
    _btn4(in4Pin, 5),
    _btn5(in5Pin, 5),
    _btn6(in6Pin, 5),
    _btn7(in7Pin, 5),
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

void Ctrlr::btnOnEdge(Bounce &btn, int& pinVal, const int& midiChannel, btnCallback cb) {
  if (btn.fallingEdge()) {
    cb(btn, pinVal, midiChannel, true);
  } else if (btn.risingEdge()) {
    cb(btn, pinVal, midiChannel, false);
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

  btnOnEdge(_btn0, _pin0_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(60, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 60 = C4
  });
  btnOnEdge(_btn1, _pin1_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(61, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 61 = C#4
  });
  btnOnEdge(_btn2, _pin2_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(62, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 62 = D4
  });
  btnOnEdge(_btn3, _pin3_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(63, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 63 = D#4
  });
  btnOnEdge(_btn4, _pin4_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(64, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 64 = E4
  });
  btnOnEdge(_btn5, _pin5_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(65, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 65 = F4
  });
  btnOnEdge(_btn6, _pin6_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(66, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 66 = F#4
  });
  btnOnEdge(_btn7, _pin7_val, _midiChannel, [](Bounce& btn, int& pinVal, const int& midiChannel, const bool fall) {
    pinVal = fall ? LOW : HIGH;
    usbMIDI.sendNoteOn(67, fall ? VEL_NOTE_ON : VEL_NOTE_OFF, midiChannel);  // 67 = G4
  });

  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) { }

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

  long new_renc_val = _renc.read();
  if (new_renc_val != _renc_val) {
    _renc_val = new_renc_val;
    usbMIDI.sendPitchBend(_renc_val, _midiChannel);
  }

  _display.print(F("RENC "));
  _display.println(_renc_val);
  _display.display();
}
