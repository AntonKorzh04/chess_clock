/*
 * tm1637.h
 *
 *  Created on: 29 сент. 2019 г.
 *      Author: dima
 */

#ifndef TM1637_H_
#define TM1637_H_

#include "stm32f1xx_hal.h"

#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0

class TM1637 {
public:
	TM1637(GPIO_TypeDef *GPIOx,
			uint16_t GPIO_DIO_Pin,
			uint16_t GPIO_CLK_Pin):
				port(GPIOx),
				DIO_Pin(GPIO_DIO_Pin),
				CLK_Pin(GPIO_CLK_Pin) {
		DWTInit();
		Cmd_DispCtrl = 0;
		point_flag = 0;
		SetBrightness(7);
		Clear();
	};

	/* Управление дисплеем  --------------------------------------------------*/

	// Отображения массива цифр
	void Display4Digits(int8_t *DispData);

	// Отображение символа по адресу
	void Display(uint8_t BitAddr, int8_t DispData);

	// Очистка дисплея
	void Clear();

	// Сброс дисплея (все нули)
	void Reset();

	// Установка яркости (применяется при следующем отображении)
	void SetBrightness(uint8_t brightness);

	/* Установка отображения двоеточия:
	 * 0 - мигает, 1 - горит постоянно
	 удалить функцию - нет двоеточия */
	void Point(uint8_t cmd);

	void BlinkLow();
	void BlinkHigh();

private:
	// Порт, считаем, что одинаковый для всех пинов
	GPIO_TypeDef *port;

	// Пины DIO и CLK
	uint16_t DIO_Pin, CLK_Pin;

	//
	uint8_t Cmd_DispCtrl;

	//
	uint8_t point_flag;

	// То, что отображается в данный момент
	int8_t CurrentDispData[4];

	/* Коды символов для отображения на дисплее
	 * 0~9, A(10), b(11), -(12) */
	static int8_t TubeTab[];

	// Управление GPIO
	void CLK_HIGH() { HAL_GPIO_WritePin(port, CLK_Pin, GPIO_PIN_SET); }
	void CLK_LOW() { HAL_GPIO_WritePin(port, CLK_Pin, GPIO_PIN_RESET); }
	void DIO_HIGH() { HAL_GPIO_WritePin(port, DIO_Pin, GPIO_PIN_SET); }
	void DIO_LOW() { HAL_GPIO_WritePin(port, DIO_Pin, GPIO_PIN_RESET); }
	GPIO_PinState DIO_READ() { return HAL_GPIO_ReadPin(port, DIO_Pin); }

	// Отправка стартовой/стоповой последовательности
	void Start();//send start bits
	void Stop(); //send stop bits

	// Перевод цифр в коды для отображения
	void IntTo7Seg4Digits(int8_t DispData[]);
	int8_t IntTo7Seg(int8_t DispData);

	// Отправить 8 бит данных на дисплей
	void WriteByte(int8_t wr_data);// write 8bit data to tm1637
};

#endif /* TM1637_H_ */
