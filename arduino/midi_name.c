#include "usb_names.h"

//
// Set the MIDI device name
//

#define MIDI_NAME   {'C','T','R','L','R','M','I','D','I'}
#define MIDI_NAME_LEN  9

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};
