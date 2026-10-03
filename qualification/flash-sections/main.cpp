#include <Arduino.h>
#include <ILI9341_T4.h>

// Link-only qualification. Pin values are placeholders, not a wiring profile.
// Do not upload this image: it deliberately exercises blocking calibration.
ILI9341_T4::ILI9341Driver display(10, 9, 13, 11, 12, 8, 7, 6);
DMAMEM uint16_t framebuffer[320 * 240];

void setup() {
    display.begin();
    display.overlayText(framebuffer, "Flash section qualification", 0, 0, 16,
                        0xffff, 1.0f, 0, 1.0f, false);
    display.overlayFPS(framebuffer);
    int calibration[4] = {};
    display.calibrateTouch(calibration);
}

void loop() {}
