#ifndef NIBBLE_NIBBLE_H
#define NIBBLE_NIBBLE_H

#include "Pins.hpp"

class Display;
class I2cExpander;

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
