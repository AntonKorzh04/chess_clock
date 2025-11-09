/*
 * tm1637.c
 *
 *  Created on: 29 сент. 2019 г.
 *      Author: dima
 */

#include <tm1637.hpp>

int8_t TM1637::TubeTab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x40};

/* Public --------------------------------------------------------------------*/

void TM1637::Display4Digits(int8_t *DispData, bool save) {
	int8_t SegData[4];
	if (save) {
		for(uint8_t i = 0; i < 4; i++) {
			SegData[i] = DispData[i];
			CurrentDispData[i] = DispData[i];
		}
	} else {
		for(uint8_t i = 0; i < 4; i++) SegData[i] = DispData[i];
	}

	IntTo7Seg4Digits(SegData);
	Start();
	WriteByte(ADDR_AUTO);
	Stop();

	Start();
	WriteByte(0xc0);
	for(uint8_t i = 0; i < 4; i++) WriteByte(SegData[i]);
	Stop();

	Start();
	WriteByte(Cmd_DispCtrl);
	Stop();
}

void TM1637::Display(uint8_t BitAddr, int8_t DispData, bool save) {
	int8_t SegData;
	SegData = IntTo7Seg(DispData);
	if (save) CurrentDispData[BitAddr] = DispData;

	Start();
	WriteByte(ADDR_FIXED);
	Stop();

	Start();
	WriteByte(BitAddr | 0xc0);
	WriteByte(SegData);
	Stop();

	Start();
	WriteByte(Cmd_DispCtrl);
	Stop();
}

void TM1637::Clear() {
	Display(0x00, 0x7f, false);
	Display(0x01, 0x7f, false);
	Display(0x02, 0x7f, false);
	Display(0x03, 0x7f, false);
}

void TM1637::Reset() {
	Display(0x00, 0x00);
	Display(0x01, 0x00);
	Display(0x02, 0x00);
	Display(0x03, 0x00);
}

void TM1637::SetBrightness(uint8_t brightness) {
	Cmd_DispCtrl = 0x88 + brightness;
}

void TM1637::Point(uint8_t cmd) {
	if(cmd == 0) point_flag = (~point_flag) & 0x01;
	else point_flag = 1;
}

void TM1637::SetBlinkMode(DispBlinkMode _mode) {
	if (mode == _mode) return;
	mode = _mode;
	if (mode != NO_BLINK) {
		Display4Digits(CurrentDispData, false);
		isBlinking = true;
		isOn = true;
	}
	else {
		isBlinking = false;
		Display4Digits(CurrentDispData, false);
	}
}

void TM1637::Toggle() {
	switch (mode) {
		case BLINK_LOW:
			if (isOn) {
				Display(0x02, 0x7f, false);
				Display(0x03, 0x7f, false);
			} else {
				Display(0x02, CurrentDispData[2], false);
				Display(0x03, CurrentDispData[3], false);
			}
			isOn = !isOn;
			break;
		case BLINK_HIGH:
			if (isOn) {
				Display(0x00, 0x7f, false);
				Display(0x01, 0x7f, false);
			} else {
				Display(0x00, CurrentDispData[0], false);
				Display(0x01, CurrentDispData[1], false);
			}
			isOn = !isOn;
			break;
		case BLINK:
			if (isOn) {
				Clear();
				point_flag = 1;
			} else {
				Display4Digits(CurrentDispData, false);
				point_flag = 0;
			}
			isOn = !isOn;
			break;
		case NO_BLINK: break;
		default: break;
	}
}

/* Private -------------------------------------------------------------------*/

void TM1637::Start() {
	CLK_HIGH();
	DelayUs(6);
	DIO_HIGH();
	DelayUs(6);
	DIO_LOW();
	DelayUs(6);
	CLK_LOW();
}

void TM1637::Stop() {
	CLK_LOW();
	DelayUs(6);
	DIO_LOW();
	DelayUs(6);
	CLK_HIGH();
	DelayUs(6);
	DIO_HIGH();
}

void TM1637::IntTo7Seg4Digits(int8_t *DispData) {
	uint8_t PointData;
	if(point_flag == 1) PointData = 0x80;
		else PointData = 0;

	for(uint8_t i = 0; i < 4; i++) {
		if(DispData[i] == 0x7f) DispData[i] = 0x00;
			else DispData[i] = TubeTab[DispData[i]] + PointData;
	}
}

int8_t TM1637::IntTo7Seg(int8_t DispData) {
	uint8_t PointData;
	if(point_flag == 1) PointData = 0x80;
		else PointData = 0;

	if(DispData == 0x7f) DispData = 0x00 + PointData;
		else DispData = TubeTab[DispData] + PointData;

	return DispData;
}

void TM1637::WriteByte(int8_t wr_data) {
  uint8_t count = 0;

  for(uint8_t i = 0; i < 8; i++)
  {
	CLK_LOW();
    if(wr_data & 0x01) DIO_HIGH(); else DIO_LOW();
    DelayUs(6);
    wr_data >>= 1;
    DelayUs(6);
    CLK_HIGH();
    DelayUs(8);
  }

  CLK_LOW();
  DelayUs(6);
  DIO_HIGH();
  DelayUs(6);
  CLK_HIGH();
  DelayUs(8);

  while(DIO_READ()) {
    count += 1;
    if(count == 200) {
    	DIO_LOW();
    	count = 0;
    }
  }
}

