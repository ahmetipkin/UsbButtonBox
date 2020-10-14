#include "Arduino.h"
#include <cstdint>
#include <USBComposite.h>
#include <USBHID.h>

//The setup function is called once at startup of the sketch

constexpr int SHIFTER_SWITCH = 10;
constexpr int RANGE_SWITCH = 11;

constexpr int BUTTON_ROW_SIZE = 3;
constexpr int BUTTON_COL_SIZE = 4;

constexpr int BUTTON_ROW[BUTTON_ROW_SIZE] = { 14, 13, 12 };
constexpr int BUTTON_COL[BUTTON_COL_SIZE] = { 15, 16, 17, 18 };

USBHID usbHid;
HIDJoystick joystick(usbHid);
USBCompositeSerial usbSerial;

void setup() {
    using namespace std;

    usbHid.begin(usbSerial);
    joystick.setManualReportMode(true);

    pinMode(SHIFTER_SWITCH, INPUT_PULLUP);
    pinMode(RANGE_SWITCH, INPUT_PULLUP);

    for (uint16_t i = 0; i < BUTTON_ROW_SIZE; ++i) {
        pinMode(BUTTON_ROW[i], OUTPUT);
        digitalWrite(BUTTON_ROW[i], 1); // pull high
    }

    for (uint16_t i = 0; i < BUTTON_COL_SIZE; ++i) {
        pinMode(BUTTON_COL[i], INPUT_PULLUP);
    }
}

// The loop function is called in an endless loop
void loop() {
    // tuslari vs oku

    using namespace std;

    joystick.button(1, digitalRead(SHIFTER_SWITCH) == 0 ? true : false);
    joystick.button(2, digitalRead(RANGE_SWITCH) == 0 ? true : false);

    for (uint16_t i = 0; i < BUTTON_ROW_SIZE; ++i) {
        digitalWrite(BUTTON_ROW[i], 0); // pull low

        for (uint16_t j = 0; j < BUTTON_COL_SIZE; ++j) {
            uint16_t buttonIndex = 3 + i * BUTTON_COL_SIZE + j;

            joystick.button(buttonIndex, digitalRead(BUTTON_COL[j]) == 0 ? true : false);

        }

        digitalWrite(BUTTON_ROW[i], 1); // pull back high
    }

    joystick.sendReport();
}
