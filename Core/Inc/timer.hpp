/*
 * timer.hpp
 *
 *  Created on: Oct 19, 2025
 *      Author: anton
 */

#include "stm32f1xx_hal.h"
#include <tm1637.hpp>

#ifndef INC_TIMER_HPP_
#define INC_TIMER_HPP_

class Timer {
public:
	Timer(TM1637 *_disp):
	disp(_disp) {
		Reset();
	};

	/* Добавление по одному часу/минуте/секунде для настройки времени
	 * с помощью кнопок перед началом игры
	 * Задержка для плавного быстрого добавления при долгом нажатии кнопки
	 * (чтобы не слишком быстро)
	 */
	void AddHour(bool withDelay = false);
	void AddMinute(bool withDelay = false);
	void AddSecond(bool withDelay = false);

	/* Для добавления времени другому игроку во время игры
	 * Отдельные функции для лучшей читаемости
	 * Если на дисплее отображается в формате HH:MM (часы != 0), то на 3 тика
	 * режим отображения ставим MM:SS
	 */
	void Add5Sec();
	void Add10Sec();

	// Управление таймером
	void Start() { isOn = true; }
	void Stop() { isOn = false; }
	void Reset();

	/* Декремент на 1с, вызывается в прерывании RTC
	 * здесь же отображаем время на дисплей
	 */
	void Tick();

	// Текущее состояние (вкл/выкл)
	bool isOn;

	// Флаг окончания времени
	bool isUp;

	// Связанный с таймером дисплей
	TM1637 *disp;

	/* Вывод текущего времени таймера на дисплей
	 * withHours = 1 => HH:MM
	 * withHours = 0 => MM:SS
	 */
	void DispTime(bool withHours);

private:
	// Структура для хранения времени
	RTC_TimeTypeDef time;

	/* Счетчик тиков, в течение которых нужно отображать формат MM:SS
	 * Устанавливается в Add5Sec и Add10Sec, если в данный момент режим
	 * отображения HH:MM (часы != 0)
	 */
	uint8_t changeToMMSS;
};

#endif /* INC_TIMER_HPP_ */
