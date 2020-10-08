#include "ctrlr.h"

//
// Set Tools --> USB Type --> MIDI
//

#define MIDI_CHANNEL 1

#define IN0_BTN     21
#define IN1_BTN     19
#define IN2_BTN     17
#define IN3_BTN     15
#define IN4_BTN     20
#define IN5_BTN     18
#define IN6_BTN     16
#define IN7_BTN     22
#define IN_RENC_SW  7
#define IN_RENC_DT  5
#define IN_RENC_CLK 6

Ctrlr ctrlr(
  MIDI_CHANNEL,
  IN0_BTN,
  IN1_BTN,
  IN2_BTN,
  IN3_BTN,
  IN4_BTN,
  IN5_BTN,
  IN6_BTN,
  IN7_BTN,
  IN_RENC_SW,
  IN_RENC_DT,
  IN_RENC_CLK
);

void setup() {
  Serial.begin(9600);
  ctrlr.setup();
}

void loop() {
  ctrlr.update();
}
