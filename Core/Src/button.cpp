/*
 * button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: anton
 */

#include <button.hpp>

void Button::SetLed(ButtonLedColor _color, ButtonLedMode _mode) {
	if (color == _color && mode == _mode) return;
	color = _color;
	mode = _mode;
	isBlinking = mode == BLINKING;
	switch (_mode) {
		case NORMAL:
			SetLedOnGPIO();
			break;
		case BLINKING:
			SetLedOnGPIO();
			isOn = true;
			break;
		case OFF:
			SetLedOffGPIO();
			break;
		default: break;
	}
}

void Button::ToggleLed() {
	switch (mode) {
		case NORMAL:
			SetLed(color, OFF);
			break;
		case BLINKING:
			if (isOn) SetLedOffGPIO();
				else SetLedOnGPIO();
			isOn = !isOn;
			break;
		case OFF:
			SetLed(color, NORMAL);
			break;
		default: break;
	}
}

void Button::SetLedOnGPIO() {
	switch (color) {
		case RED:
			HAL_GPIO_WritePin(port, pin_green|pin_blue, GPIO_PIN_SET);
			HAL_GPIO_WritePin(port, pin_red, GPIO_PIN_RESET);
			break;
		case GREEN:
			HAL_GPIO_WritePin(port, pin_red|pin_blue, GPIO_PIN_SET);
			HAL_GPIO_WritePin(port, pin_green, GPIO_PIN_RESET);
			break;
		case BLUE:
			HAL_GPIO_WritePin(port, pin_red|pin_green, GPIO_PIN_SET);
			HAL_GPIO_WritePin(port, pin_blue, GPIO_PIN_RESET);
			break;
		default: break;
	}
}

void Button::SetLedOffGPIO() {
	HAL_GPIO_WritePin(port, pin_red|pin_green|pin_blue, GPIO_PIN_SET);
}
