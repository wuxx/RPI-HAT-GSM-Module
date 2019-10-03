/*
 * MIT License
 * Copyright (c) 2018, Michal Kozakiewicz, github.com/michal037
 *
 * Version: 2.0.0
 * Standard: GCC-C11
 */

#include <stdio.h>
#include <wiringPi.h>
#include "st7735s.h"

int main(int argc, char *argv[])
{
	lcdst_t *myDisplay = NULL;
	
	/* Initialize the Wiring Pi library */
	wiringPiSetup();
	
	/* Initialize the display and receive pointer */
                                 /*  cs,dc/rs,reset  */
	myDisplay = lcdst_init(30000000, 0, 27, 28);
	
	/* Print received pointer */
	printf("myDisplay: %p\n", myDisplay);
	
	LCD_Clear(0xffff);

	LCD_Clear(0xf800);

	LCD_Clear(0x07e0);

	LCD_Clear(0x001f);

	LCD_Clear(0xffff);

	LCD_Clear(0x0000);

    while(1);

	/* Fill the display */
	lcdst_drawScreen(0, 70, 160);
	lcdst_drawScreen(0, 0, 0);

	
	/* Draw the lines */
	lcdst_drawHLine(0, 49, 128, 0, 255, 255);
	lcdst_drawHLine(0, 39, 128, 255, 255, 0);
	lcdst_drawVLine(117, 0, 160, 0, 255, 255);
	lcdst_drawVLine(107, 0, 160, 255, 255, 0);

    while(1);
	
	/* Draw the rectangles */
	lcdst_drawRect(10, 10, 10, 10, 0, 255, 255);
	lcdst_drawRect(10, 30, 10, 10, 255, 255, 0);
	lcdst_drawFRect(30, 10, 10, 10, 0, 255, 255);
	lcdst_drawFRect(30, 30, 10, 10, 255, 255, 0);

	
	/* Send the raw data */
	lcdst_setWindow(20, 20, 29, 29);
	for(uint8 i=0; i<100; i++) lcdst_pushPx(255, 0, 255);
	lcdst_setWindow(0, 0, 127, 159); /* Optional reset */
    while(1);
	
	/* Uninitialize the display */
	//lcdst_uninit(myDisplay);
	
	return 0;
} /* main */
