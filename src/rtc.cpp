#include "headers/rtc.h"

int8 *month_map[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

uint32 update_flagRTC()
{
	outport(CMOS_ADDRESS, 0x0A);
	return (inport(CMOS_DATA) & 0x80);
}

uint8 get_registerRTC(uint32 reg)
{
	outport(CMOS_ADDRESS, reg);
	return inport(CMOS_DATA);
}

uint32 time()
{
	datetime dt;
	readRTC(&dt);
	return dt.sec + dt.min * 60 + dt.hour * 3600;
}

void readRTC(datetime *dt)
{

	while (update_flagRTC())
		;

	uint8 sec = get_registerRTC(0x00);
	uint8 min = get_registerRTC(0x02);
	uint8 hour = get_registerRTC(0x04);
	uint8 day = get_registerRTC(0x07);
	uint8 month = get_registerRTC(0x08);
	uint8 year = get_registerRTC(0x09);

	uint8 registerB = get_registerRTC(0x0B);

	if (!(registerB & 0x04))
	{
		sec = (sec & 0x0F) + ((sec / 16) * 10);
		min = (min & 0x0F) + ((min / 16) * 10);
		hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
		day = (day & 0x0F) + (day / 16) * 10;
		month = (month & 0x0F) + (month / 16) * 10;
		year = (year & 0x0F) + (year / 16) * 10;
	}
	if (!(registerB & 0x02) && (hour & 0x80))
	{
		hour = ((hour & 0x7F) + 12) % 24;
	}
	if (hour >= 3)
		hour = hour - 3;
	else
	{
		hour = hour + 21;
	}

	(*dt).sec = sec;
	(*dt).min = min;
	(*dt).hour = hour;
	(*dt).day = day;
	(*dt).month = month;
	(*dt).year = year;
}

void printRTC()
{
	while (update_flagRTC())
		;

	uint8 second = get_registerRTC(0x00);
	uint8 minute = get_registerRTC(0x02);
	uint8 hour = get_registerRTC(0x04);
	uint8 day = get_registerRTC(0x07);
	uint8 month = get_registerRTC(0x08);
	uint8 year = get_registerRTC(0x09);

	uint8 registerB = get_registerRTC(0x0B);

	if (!(registerB & 0x04))
	{
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
		day = (day & 0x0F) + (day / 16) * 10;
		month = (month & 0x0F) + (month / 16) * 10;
		year = (year & 0x0F) + (year / 16) * 10;
	}
	if (!(registerB & 0x02) && (hour & 0x80))
	{
		hour = ((hour & 0x7F) + 12) % 24;
	}
	if (hour >= 3)
		hour = hour - 3;
	else
	{
		hour = hour + 21;
	}

	printf("%d de ", day);
	printf(month_map[month - 1]);
	printf(" de 20%d  - ", year);
	printf("%d:%d:%d", hour, minute, second);
}
