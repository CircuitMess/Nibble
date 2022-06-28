#include "Nibble.h"
#include <Loop/LoopManager.h>

NibbleImpl Nibble;

void NibbleImpl::begin(){

	if(!Settings::init(new NibbleSettings, sizeof(NibbleSettings))){
		settings()->shutdownTime = 300; //5 minutes
		settings()->sleepTime = 30; //30 seconds
		settings()->audio = true; //audio on
		settings()->calibrated = false;
		settings()->displayTab = 0;
		Settings::store();
	}

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
	LoopManager::addListener(input);

	display = new Display(128, 128, -1, 0);
	display->begin();
	if(settings()->displayTab == 1){
		display->getTft()->init(INITR_GREENTAB3);
	}
	display->getBaseSprite()->clear(TFT_BLACK);
	display->commit();

	input->loop(0);

	uint8_t portRead = 0;
	for(int i = 0; i < 7; i++){
		portRead |= (~Nibble.getInput()->getButtonState(i) & 1) << i;
	}
	portRead = portRead & 0b01111111;

	if(input->getButtonState(BTN_C) && portRead){
		Sprite* canvas = display->getBaseSprite();
		canvas->setTextFont(0);
		canvas->setTextColor(TFT_WHITE);
		canvas->setTextSize(1);
		canvas->setCursor(0, 60);
		canvas->printCenter("Hold MENU\n");
		canvas->printCenter("to switch\n");
		canvas->printCenter("display driver.");
		display->commit();

		uint32_t time = millis();
		while(millis() - time <= 2000){
			delay(100);
			input->loop(0);
			if(!input->getButtonState(BTN_C)) break;
		}

		if(input->getButtonState(BTN_C)){
			settings()->displayTab = !settings()->displayTab;
			Settings::store();

			canvas->clear(TFT_BLACK);
			display->commit();

			display->getTft()->init(settings()->displayTab ? INITR_GREENTAB3 : INITR_GREENTAB128);
		}

		canvas->clear(TFT_BLACK);
		display->commit();
	}

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

NibbleSettings* settings(){
	return static_cast<NibbleSettings*>(Settings::data());
}
