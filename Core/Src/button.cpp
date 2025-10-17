/*
 * button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: anton
 */

#include <button.hpp>

void Button::SetLedMode(ButtonLedMode mode) {
	if (this->mode == mode) { return; }
	switch (mode) {
		case NORMAL:
			this->SetLedOnGPIO();
			if (this->mode == BLINKING) { HAL_TIM_Base_Stop_IT(this->htim); }
			break;
		case BLINKING:
			this->SetLedOnGPIO();
			this->isOn = true;
			HAL_TIM_Base_Start_IT(this->htim);
			break;
		case OFF:
			this->SetLedOffGPIO();
			if (this->mode == BLINKING) { HAL_TIM_Base_Stop_IT(this->htim); }
		default: break;
	}
	this->mode = mode;
}

void Button::ToggleLed() {
	switch (this->mode) {
		case NORMAL:
			this->SetLedMode(OFF);
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
			this->SetLedMode(NORMAL);
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
