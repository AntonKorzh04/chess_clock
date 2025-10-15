/*
 * button.h
 *
 *  Created on: Oct 10, 2025
 *      Author: anton
 */

#include "stm32f1xx_hal.h"

#ifndef BUTTON_H_
#define BUTTON_H_

// Цвета подсветки кнопки
typedef enum {
	RED,
	GREEN,
	BLUE,
	OFF
} ButtonLedColor;

// Режимы работы подсветки кнопки
typedef enum {
	NORMAL,
	BLINKING
} ButtonLedState;

class Button {
public:
	Button(GPIO_TypeDef *GPIOx,
			uint16_t GPIO_Pin_Red,
			uint16_t GPIO_Pin_Green,
			uint16_t GPIO_Pin_Blue,
			uint16_t GPIO_Pin):
				port(GPIOx),
				pin_red(GPIO_Pin_Red),
				pin_green(GPIO_Pin_Green),
				pin_blue(GPIO_Pin_Blue),
				pin(GPIO_Pin) {};

	// Порт, считаем, что одинаковый для всех пинов
	GPIO_TypeDef *port;

	// Пины, к которым подключены светодиоды
	uint16_t pin_red;
	uint16_t pin_green;
	uint16_t pin_blue;

	// Пин, с которого считываются нажатия кнопки
	uint16_t pin;

	// управление подсветкой кнопки
	void SetLed(ButtonLedColor color, ButtonLedState blinking_state = NORMAL);

private:
	// state - состояние включена/выключена
	// isShortPressed - устанавливается после отпускания кнопки при кототком нажатии
	//     снимается вручную в управляющем автомате
	// isPressed - устанавливается после сброса таймера долгого нажатия, снимается
	//     автоматически при отпускании кнопки
	bool state, isShortPressed, isPressed;

	// Указатель на таймер, по прерываниям которого работает мигание
};

#endif /* BUTTON_H_ */
