#ifndef NIBBLE_NIBBLE_H
#define NIBBLE_NIBBLE_H

#include "Pins.hpp"

#include <CircuitOS.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>

class NibbleImpl {
public:
	void begin();

	Display* getDisplay();
	I2cExpander* getExpander();


private:
	Display* display;
	I2cExpander* expander;

};

extern NibbleImpl Nibble;

#endif //NIBBLE_NIBBLE_H
