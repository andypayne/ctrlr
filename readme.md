# CTRLR MIDI Controller

A programmable MIDI Controller

![CTRLR](docs/ctrlr.png)


## Hardware

- [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
- [Adafruit SSD1306 128x32 display](https://www.adafruit.com/product/661)
- 8 pushbuttons (from [Anchor Electronics](https://anchor-electronics.com/))
- A rotary encoder
- 3D-printed enclosure, buttons caps, and rotary wheel


## Dependencies for the Arduino project

- [buttoner](https://github.com/andypayne/buttoner)
- [midi_defs](https://github.com/andypayne/midi_defs)


## Pure Data integration

Objects for [Pure Data](https://puredata.info/) are located in [/PureData](/PureData).


### ctrlr.pd

![ctrlr.pd](docs/ctrlr_pd_shot.png)


### ctrlr_test.pd

![ctrlr_test.pd](docs/ctrlr_test_pd_shot.png)


## Issues

TODO - elaborate on issues

- enclosure - thickness, piece fit, layout (ports), support columns, etc
- USB port
- screen - fit, size, asymmetry
- button travel
- access to reset, etc
- header pin connectors - poor fit

