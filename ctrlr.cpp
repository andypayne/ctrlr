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

  delay(1000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  _display.clearDisplay();
  _display.display();
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

  // Send Note On on falling edge
  if (_btn0.fallingEdge()) {
    _pin0_val = LOW;
    usbMIDI.sendNoteOn(60, 99, _midiChannel);  // 60 = C4
  }
  if (_btn1.fallingEdge()) {
    _pin1_val = LOW;
    usbMIDI.sendNoteOn(61, 99, _midiChannel);  // 61 = C#4
  }
  if (_btn2.fallingEdge()) {
    _pin2_val = LOW;
    usbMIDI.sendNoteOn(62, 99, _midiChannel);  // 62 = D4
  }
  if (_btn3.fallingEdge()) {
    _pin3_val = LOW;
    usbMIDI.sendNoteOn(63, 99, _midiChannel);  // 63 = D#4
  }
  if (_btn4.fallingEdge()) {
    _pin4_val = LOW;
    usbMIDI.sendNoteOn(64, 99, _midiChannel);  // 64 = E4
  }
  if (_btn5.fallingEdge()) {
    _pin5_val = LOW;
    usbMIDI.sendNoteOn(65, 99, _midiChannel);  // 65 = F4
  }
  if (_btn6.fallingEdge()) {
    _pin6_val = LOW;
    usbMIDI.sendNoteOn(66, 99, _midiChannel);  // 66 = F#4
  }
  if (_btn7.fallingEdge()) {
    _pin7_val = LOW;
    usbMIDI.sendNoteOn(67, 99, _midiChannel);  // 67 = G4
  }

  // Send Note Off on rising edge
  if (_btn0.risingEdge()) {
    _pin0_val = HIGH;
    usbMIDI.sendNoteOff(60, 0, _midiChannel);  // 60 = C4
  }
  if (_btn1.risingEdge()) {
    _pin1_val = HIGH;
    usbMIDI.sendNoteOff(61, 0, _midiChannel);  // 61 = C#4
  }
  if (_btn2.risingEdge()) {
    _pin2_val = HIGH;
    usbMIDI.sendNoteOff(62, 0, _midiChannel);  // 62 = D4
  }
  if (_btn3.risingEdge()) {
    _pin3_val = HIGH;
    usbMIDI.sendNoteOff(63, 0, _midiChannel);  // 63 = D#4
  }
  if (_btn4.risingEdge()) {
    _pin4_val = HIGH;
    usbMIDI.sendNoteOff(64, 0, _midiChannel);  // 64 = E4
  }
  if (_btn5.risingEdge()) {
    _pin5_val = HIGH;
    usbMIDI.sendNoteOff(65, 0, _midiChannel);  // 65 = F4
  }
  if (_btn6.risingEdge()) {
    _pin6_val = HIGH;
    usbMIDI.sendNoteOff(66, 0, _midiChannel);  // 66 = F#4
  }
  if (_btn7.risingEdge()) {
    _pin7_val = HIGH;
    usbMIDI.sendNoteOff(67, 0, _midiChannel);  // 67 = G4
  }

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
