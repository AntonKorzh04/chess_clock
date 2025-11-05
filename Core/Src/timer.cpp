/*
 * timer.cpp
 *
 *  Created on: Oct 19, 2025
 *      Author: anton
 */

#include <timer.hpp>

RTC_TimeTypeDef Timer::globalTime = {0, 0, 0};

/* Public --------------------------------------------------------------------*/

void Timer::AddHour(bool withDelay) {
	globalTime.Hours++;
	if (withDelay) HAL_Delay(100);
}

void Timer::AddMinute(bool withDelay) {
	globalTime.Minutes++;
	if (withDelay) HAL_Delay(100);
}

void Timer::AddSecond(bool withDelay) {
	globalTime.Seconds++;
	if (withDelay) HAL_Delay(100);
}

void Timer::ResetGlobal() {
	Timer::globalTime = {0, 0, 0};
}

void Timer::Add5Sec() {
	time.Seconds += 5U;
	if (time.Hours != 0) changeToMMSS = 3;
}

void Timer::Add10Sec() {
	time.Seconds += 10U;
	if (time.Hours != 0) changeToMMSS = 3;
}

void Timer::Reset() {
	isOn = false;
	isUp = false;

	time.Hours = 0U;
	time.Minutes = 0U;
	time.Seconds = 0U;

	changeToMMSS = 0U;
}

void Timer::SetTimeFromGlobal() {
	time = globalTime;
	DispTime();
}

void Timer::Tick() {
	if (!IS_RTC_SECONDS(--time.Seconds)) {
		time.Seconds = 59U;
		if (!IS_RTC_MINUTES(--time.Minutes)) {
			time.Minutes = 59U;
			if (!IS_RTC_HOUR24(--time.Hours)) {
				Stop();
				time.Hours = 0U;
				time.Minutes = 0U;
				time.Seconds = 0U;
				isUp = true;
			}
		}
	}
	DispTime();
}

/* Private -------------------------------------------------------------------*/

void Timer::DispTime() {
	disp->Point(0);

	bool withHours = true;
	if (time.Hours == 0) withHours = false;
	else if (changeToMMSS) {
		withHours = false;
		changeToMMSS--;
	}

	if (withHours) {
		disp->Display(0x0, time.Hours / 10);
		disp->Display(0x1, time.Hours % 10);
		disp->Display(0x2, time.Minutes / 10);
		disp->Display(0x3, time.Minutes % 10);
	} else {
		disp->Display(0x0, time.Minutes / 10);
		disp->Display(0x1, time.Minutes % 10);
		disp->Display(0x2, time.Seconds / 10);
		disp->Display(0x3, time.Seconds % 10);
	}
}
