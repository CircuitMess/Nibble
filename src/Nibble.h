#ifndef NIBBLE_NIBBLE_H
#define NIBBLE_NIBBLE_H

#include "Pins.hpp"

#include <CircuitOS.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include <Input/I2cExpander.h>
#include <Input/InputShift.h>
#include <Input/InputI2C.h>
#include <Audio/Piezo.h>
#include <Util/Settings.h>

struct NibbleSettings {
	int sleepTime = 300;
	int shutdownTime = 30;
	bool audio = true;
	bool calibrated = false;
	uint8_t displayTab = 0;
};

NibbleSettings* settings();

class NibbleImpl {
public:

	/**
	 * Initializes display, backlight, Piezo, I2C expander, I2C input, and pre-registers all buttons.
	 */
	void begin();

	Display* getDisplay();
	I2cExpander* getExpander();
	Input* getInput();

	void setBacklight(bool state);
	void shutdown();

private:
	Display* display;
	I2cExpander* expander;
	Input* input;

};

extern NibbleImpl Nibble;

#endif //NIBBLE_NIBBLE_H
