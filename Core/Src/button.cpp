/*
 * button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: anton
 */

#include <button.hpp>

void Button::SetLed(ButtonLedColor _color, ButtonLedMode _mode) {
	if (this->color == _color && this->mode == _mode) return;
	this->color = _color;
	this->mode = _mode;
	switch (_mode) {
		case NORMAL:
			this->SetLedOnGPIO();
			break;
		case BLINKING:
			this->SetLedOnGPIO();
			this->isOn = true;
			break;
		case OFF:
			this->SetLedOffGPIO();
		default: break;
	}
}

void Button::ToggleLed() {
	switch (this->mode) {
		case NORMAL:
			this->SetLed(this->color, OFF);
			break;
		case BLINKING:
			if (this->isOn) {
				this->SetLedOffGPIO();
			} else {
				this->SetLedOnGPIO();
			}
			this->isOn = !this->isOn;
			break;
		case OFF:
			this->SetLed(this->color, NORMAL);
			break;
	}
}

void Button::SetLedOnGPIO() {
	switch (this->color) {
		case RED:
			HAL_GPIO_WritePin(this->port, this->pin_green|this->pin_blue, GPIO_PIN_SET);
			HAL_GPIO_WritePin(this->port, this->pin_red, GPIO_PIN_RESET);
			break;
		case GREEN:
			HAL_GPIO_WritePin(this->port, this->pin_red|this->pin_blue, GPIO_PIN_SET);
			HAL_GPIO_WritePin(this->port, this->pin_green, GPIO_PIN_RESET);
			break;
		case BLUE:
			HAL_GPIO_WritePin(this->port, this->pin_red|this->pin_green, GPIO_PIN_SET);
			HAL_GPIO_WritePin(this->port, this->pin_blue, GPIO_PIN_RESET);
			break;
		default: break;
	}
}

void Button::SetLedOffGPIO() {
	HAL_GPIO_WritePin(this->port, this->pin_red|this->pin_green|this->pin_blue, GPIO_PIN_SET);
}
