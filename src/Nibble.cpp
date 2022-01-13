#include "Nibble.h"

NibbleImpl Nibble;

void NibbleImpl::begin(){
	expander = new I2cExpander();

	if(expander->begin(0x74, 4, 5)){
		expander->pinMode(BL_PIN_I2C, OUTPUT);
		expander->pinWrite(BL_PIN_I2C, true);

		input = new InputI2C(expander);
	}else{
		delete expander;
		expander = nullptr;

		pinMode(BL_PIN_IO, OUTPUT);
		digitalWrite(BL_PIN_IO, HIGH);

		InputShift* inputShift = new InputShift(5, 4, 0, 7);
		inputShift->begin();
		input = inputShift;
	}

	input->preregisterButtons({ BTN_A, BTN_B, BTN_C, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT });

	display = new Display(128, 128, -1, 0);
	display->begin();
	display->getBaseSprite()->clear(TFT_BLACK);
	display->commit();

	Piezo.begin(BUZZ_PIN);
}

void NibbleImpl::setBacklight(bool state){
	if(expander){
		expander->pinWrite(BL_PIN_I2C, state);
	}else{
		digitalWrite(BL_PIN_IO, state ? HIGH : LOW);
	}
}

void NibbleImpl::shutdown(){
	setBacklight(false);
	display->getTft()->writecommand(16);

	if(expander){
		expander->portConfig(0xFFFF);
	}

	ESP.deepSleep(0);
}

Display* NibbleImpl::getDisplay(){
	return display;
}

I2cExpander* NibbleImpl::getExpander(){
	return expander;
}

Input* NibbleImpl::getInput(){
	return input;
}
