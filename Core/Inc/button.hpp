/*
 * button.hpp
 *
 *  Created on: Oct 10, 2025
 *      Author: anton
 */

#include "stm32f1xx_hal.h"

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

// Цвета подсветки кнопки
typedef enum {
	RED,
	GREEN,
	BLUE
} ButtonLedColor;

// Режимы работы подсветки кнопки
typedef enum {
	NORMAL,
	BLINKING,
	OFF
} ButtonLedMode;

class Button {
public:
	Button(GPIO_TypeDef *GPIOx,
			uint16_t GPIO_Pin_Red,
			uint16_t GPIO_Pin_Green,
			uint16_t GPIO_Pin_Blue,
			uint16_t GPIO_Pin,
			TIM_HandleTypeDef *htim):
				port(GPIOx),
				pin_red(GPIO_Pin_Red),
				pin_green(GPIO_Pin_Green),
				pin_blue(GPIO_Pin_Blue),
				pin(GPIO_Pin),
				htim(htim) {
		this->SetLedColor(RED);
		this->SetLedMode(OFF);
	};

	/* isShortPressed - устанавливается после отпускания кнопки при кототком нажатии
		 снимается вручную в управляющем автомате
	isPressed - устанавливается после сброса таймера долгого нажатия, снимается
		автоматически при отпускании кнопки */
	bool isShortPressed, isPressed;

	// Установка цвета (только поле класса)
	void SetLedColor(ButtonLedColor color) { this->color = color; };

	// Установка режима подсветки (непосредственно включение)
	void SetLedMode(ButtonLedMode mode);

	// Переключение состояний (обычное вкл\выкл или для режима мигания)
	void ToggleLed();

private:
	// Порт, считаем, что одинаковый для всех пинов
	GPIO_TypeDef *port;

	// Пины, к которым подключены светодиоды
	uint16_t pin_red;
	uint16_t pin_green;
	uint16_t pin_blue;

	// Пин, с которого считываются нажатия кнопки
	uint16_t pin;

	// Текущий цвет и режим подсветки кнопки
	ButtonLedColor color;
	ButtonLedMode mode;

	// Флаг для работы режима мигания
	bool isOn;

	// Управление GPIO в соответствии с установленным цветом
	void SetLedOnGPIO();

	// Управление GPIO при выключении подсветки
	void SetLedOffGPIO();

	// Указатель на таймер, по прерываниям которого работает мигание
	TIM_HandleTypeDef *htim;
};

#endif /* BUTTON_HPP_ */
