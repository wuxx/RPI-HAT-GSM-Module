/*
 * MIT License
 * Copyright (c) 2018, Michal Kozakiewicz, github.com/michal037
 *
 * Version: 2.0.0
 * Standard: GCC-C11
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "st7735s.h"

/********************************** EASY PORT *********************************/
/*
 * If you porting this code, you can change below headers and function pointers
 * in gpio structure.
 */
#include <wiringPi.h>
#include <wiringPiSPI.h>
struct
{
	void (* const delay)(unsigned int milliseconds);
	void (* const pinMode)(int pin, int mode);
	void (* const digitalWrite)(int pin, int value);
	int  (* const spiSetup)(int channel, int speed);
	int  (* const spiDataRW)(int channel, uint8 *data, int length);
} static const gpio =
{
	delay,
	pinMode,
	digitalWrite,
	wiringPiSPISetup,
	wiringPiSPIDataRW
};
/****************************** END EASY PORT END *****************************/

/* The global variable that stores the pointer to the structure,
 * with the current active display.
 */
static lcdst_t *activeDisplay;

/*
 * Safe allocation of the memory block.
 *
 * Parameters:
 *   size - Size of memory block to allocate.
 *
 * Return:
 *   Pointer to the memory block. If an error occurs, stop the program.
 */
static inline void *safeMalloc(size_t size)
{
	void *memoryBlock = (void*) malloc(size);

	/* Check the pointer */
	if(memoryBlock == NULL)
	{
		fprintf(stderr, "Out of RAM memory!\n");
		exit(EXIT_FAILURE);
	}
	
	return memoryBlock;
} /* safeMalloc */

/*
 * Write the command to the display driver.
 *
 * Parameters:
 *   cmd - The command to write.
 */
static inline void writeCommand(uint8 cmd)
{
	gpio.digitalWrite(activeDisplay->a0, LOW);
	gpio.spiDataRW(activeDisplay->cs, &cmd, 1);
} /* writeCommand */

/*
 * Write the data to the display driver.
 *
 * Parameters:
 *   data - The data to write.
 */
static inline void writeData(uint8 data)
{
	gpio.digitalWrite(activeDisplay->a0, HIGH);
	gpio.spiDataRW(activeDisplay->cs, &data, 1);
} /* writeData */

lcdst_t *lcdst_init(int spiSpeed, int cs, int a0, int rs)
{
	/* Create the one instance of the lcdst_t structure and activate it */
	lcdst_t *instance = (lcdst_t *) safeMalloc(sizeof(lcdst_t));
	lcdst_setActiveDisplay(instance);
	
	/* Assign specific pins */
	instance->cs = cs;
	instance->a0 = a0;
	instance->rs = rs;
	/*
	 * instance->width; instance->height
	 * The setting of this variables will take place
	 * in the function lcdst_setOrientation() below.
	 */
	
	/* Configure the a0 pin. The logic level is not significant now. */
	gpio.pinMode(instance->a0, OUTPUT);
	
	/* If the rs pin is connected then configure it */
	if(instance->rs != -1)
	{
		gpio.pinMode(instance->rs, OUTPUT);
		gpio.digitalWrite(instance->rs, HIGH); /* Reset OFF */
	}
	
	/* Configure the SPI interface */
	if(gpio.spiSetup(instance->cs, spiSpeed) == -1)
	{
		fprintf(stderr, "Failed to setup the SPI interface!\n");
		exit(EXIT_FAILURE);
	}
	
	/* Software reset; Wait minimum 120ms */
	writeCommand(0x01);
	gpio.delay(150);
	
	/* Sleep out; Wait minimum 120ms */
	writeCommand(0x11);
	gpio.delay(150);
	
#if 0

	/* Set the orientation and the gamma */
	lcdst_setOrientation(0);
	lcdst_setGamma(2); /* Optional */
	
	/* Set the pixel format */
	writeCommand(0x3A);
	#if ST7735S_CFG_PIXEL == ST7735S_PIXEL_FULL
		writeData(0x06);
	#elif ST7735S_CFG_PIXEL == ST7735S_PIXEL_REDUCED
		writeData(0x03);
	#endif
	
	/* Display ON; Wait 100ms before start */
	writeCommand(0x29);
	gpio.delay(100);

#else

	//ST7735R Frame Rate
	writeCommand(0xB1); 
	writeData(0x01); 
	writeData(0x2C); 
	writeData(0x2D); 

	writeCommand(0xB2); 
	writeData(0x01); 
	writeData(0x2C); 
	writeData(0x2D); 

	writeCommand(0xB3); 
	writeData(0x01); 
	writeData(0x2C); 
	writeData(0x2D); 
	writeData(0x01); 
	writeData(0x2C); 
	writeData(0x2D); 
	
	writeCommand(0xB4); //Column inversion 
	writeData(0x07); 
	
	//ST7735R Power Sequence
	writeCommand(0xC0); 
	writeData(0xA2); 
	writeData(0x02); 
	writeData(0x84); 
	writeCommand(0xC1); 
	writeData(0xC5); 

	writeCommand(0xC2); 
	writeData(0x0A); 
	writeData(0x00); 

	writeCommand(0xC3); 
	writeData(0x8A); 
	writeData(0x2A); 
	writeCommand(0xC4); 
	writeData(0x8A); 
	writeData(0xEE); 
	
	writeCommand(0xC5); //VCOM 
	writeData(0x0E); 
	
	writeCommand(0x36); //MX, MY, RGB mode 
	writeData(0xC8); 
	
	//ST7735R Gamma Sequence
	writeCommand(0xe0); 
	writeData(0x0f); 
	writeData(0x1a); 
	writeData(0x0f); 
	writeData(0x18); 
	writeData(0x2f); 
	writeData(0x28); 
	writeData(0x20); 
	writeData(0x22); 
	writeData(0x1f); 
	writeData(0x1b); 
	writeData(0x23); 
	writeData(0x37); 
	writeData(0x00); 	
	writeData(0x07); 
	writeData(0x02); 
	writeData(0x10); 

	writeCommand(0xe1); 
	writeData(0x0f); 
	writeData(0x1b); 
	writeData(0x0f); 
	writeData(0x17); 
	writeData(0x33); 
	writeData(0x2c); 
	writeData(0x29); 
	writeData(0x2e); 
	writeData(0x30); 
	writeData(0x30); 
	writeData(0x39); 
	writeData(0x3f); 
	writeData(0x00); 
	writeData(0x07); 
	writeData(0x03); 
	writeData(0x10);  
	
	writeCommand(0x2a);
	writeData(0x00);
	writeData(0x02);
	writeData(0x00);
	writeData(0x82);

	writeCommand(0x2b);
	writeData(0x00);
	writeData(0x03);
	writeData(0x00);
	writeData(0x83);
	
	writeCommand(0xF0); //Enable test command  
	writeData(0x01); 
	writeCommand(0xF6); //Disable ram power save mode 
	writeData(0x00); 
	
	writeCommand(0x3A); //65k mode 
	writeData(0x05); 

	/* Display ON; Wait 100ms before start */
	writeCommand(0x29);
	gpio.delay(100);

#endif	

	return instance;
} /* lcdst_init */

void lcdst_uninit(lcdst_t *display)
{
	if(display == NULL) return;
	
	/* Optional reset; Free memory block */
	lcdst_hardwareReset(display);
	free(display);
} /* lcdst_uninit */

void lcdst_hardwareReset(lcdst_t *display)
{
	/* If the reset pin is not connected, exit function */
	if(display->rs == -1) return;
	
	/* Apply reset */
	gpio.digitalWrite(display->rs, HIGH); /* Reset OFF */
	gpio.digitalWrite(display->rs, LOW);  /* Reset ON */
	gpio.delay(10);  /* Wait to activate */
	gpio.digitalWrite(display->rs, HIGH); /* Reset OFF*/
	gpio.delay(150); /* Wait before use */
} /* lcdst_hardwareReset */

void lcdst_setActiveDisplay(lcdst_t *display)
{
	activeDisplay = display;
} /* lcdst_setActiveDisplay */

lcdst_t *lcdst_getActiveDisplay(void)
{
	return activeDisplay;
} /* lcdst_getActiveDisplay */

uint8 lcdst_getWidth(void)
{
	return activeDisplay->width;
} /* lcdst_getWidth */

uint8 lcdst_getHeight(void)
{
	return activeDisplay->height;
} /* lcdst_getHeight */

void lcdst_setOrientation(uint8 orientation)
{
	writeCommand(0x36); /* Memory Data Access Control */

	switch(orientation)
	{
		case 1:
			writeData(0x60); /* MX + MV */
			activeDisplay->width  = 160;
			activeDisplay->height = 128;
			lcdst_setWindow(0, 0, 159, 127);
			break;

		case 2:
			writeData(0xC0); /* MY + MX */
			activeDisplay->width  = 128;
			activeDisplay->height = 160;
			lcdst_setWindow(0, 0, 127, 159);
			break;

		case 3:
			writeData(0xA0); /* MY + MV */
			activeDisplay->width  = 160;
			activeDisplay->height = 128;
			lcdst_setWindow(0, 0, 159, 127);
			break;

		default:
			writeData(0x00); /* None */
			activeDisplay->width  = 128;
            activeDisplay->height = 160;
            lcdst_setWindow(0, 0, 127, 159);
			break;
	}
} /* lcdst_setOrientation */

void lcdst_setGamma(uint8 state)
{
	/* The status (0 or 1) of the GS pin can only be empirically tested */
	switch(state)
	{
		case 1:  state = 2; break; /* GS_pin=1: 1.8; GS_pin=0: 2.5 */
		case 2:  state = 4; break; /* GS_pin=1: 2.5; GS_pin=0: 2.2 */
		case 3:  state = 8; break; /* GS_pin=1: 1.0; GS_pin=0: 1.8 */
		default: state = 1; break; /* GS_pin=1: 2.2; GS_pin=0: 1.0 */
	}
	
	/* Set built-in gamma */
	writeCommand(0x26);
	writeData(state);
} /* lcdst_setGamma */

void lcdst_setInversion(uint8 state)
{
	/* Display inversion ON/OFF */
	writeCommand(state ? 0x21 : 0x20);
} /* lcdst_setInversion */

uint8 lcdst_setWindow(uint8 x1, uint8 y1, uint8 x2, uint8 y2)
{
	/* Accept: 0 <= x1 <= x2 < activeDisplay->width */
	if(x2 < x1) return 1;
	if(x2 >= activeDisplay->width) return 1;
	
	/* Accept: 0 <= y1 <= y2 < activeDisplay->height */
	if(y2 < y1) return 1;
	if(y2 >= activeDisplay->height) return 1;
	
	/* Set column address */
	writeCommand(0x2A);
	writeData(0); writeData(x1);
	writeData(0); writeData(x2);
	
	/* Set row address */
	writeCommand(0x2B);
	writeData(0); writeData(y1);
	writeData(0); writeData(y2);
	
	/* Activate RAW write */
	writeCommand(0x2C);
	
	return 0;
} /* lcdst_setWindow */

void lcdst_activateRamWrite(void)
{
	writeCommand(0x2C);
} /* lcdst_activateRamWrite */

/***************************** ST7735S_PIXEL_FULL *****************************/
#if ST7735S_CFG_PIXEL == ST7735S_PIXEL_FULL

void lcdst_pushPx(uint8 r, uint8 g, uint8 b)
{
	writeData(r); writeData(g); writeData(b);
} /* lcdst_pushPx */

void lcdst_drawPx(uint8 x, uint8 y, uint8 r, uint8 g, uint8 b)
{
	if(lcdst_setWindow(x, y, x, y)) return;
	writeData(r); writeData(g); writeData(b);
} /* lcdst_drawPx */

void lcdst_drawHLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b)
{
	/* Draw only in the display space */
	if(l == 0) return;
	if((x+l-1) >= activeDisplay->width) l = activeDisplay->width - x;
	
	/* Draw the line */
	if(lcdst_setWindow(x, y, x+l-1, y)) return;
	while(l--) {writeData(r); writeData(g); writeData(b);}
} /* lcdst_drawHLine */

void lcdst_drawVLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b)
{
	/* Draw only in the display space */
	if(l == 0) return;
	if((y+l-1) >= activeDisplay->height) l = activeDisplay->height - y;
	
	/* Draw the line */
	if(lcdst_setWindow(x, y, x, y+l-1)) return;
	while(l--) {writeData(r); writeData(g); writeData(b);}
} /* lcdst_drawVLine */

void lcdst_drawFRect(uint8 x, uint8 y, uint8 w, uint8 h,
					uint8 r, uint8 g, uint8 b)
{
	/* Draw only in the display space */
	if((w == 0) || (h == 0)) return;
	if((x+w-1) >= activeDisplay->width)  w = activeDisplay->width  - x;
	if((y+h-1) >= activeDisplay->height) h = activeDisplay->height - y;
	
	/* Draw the filed rectangle */
	if(lcdst_setWindow(x, y, x+w-1, y+h-1)) return;
	while(w--) while(h--) {writeData(r); writeData(g); writeData(b);}
} /* lcdst_drawFRect */

/**************************** ST7735S_PIXEL_REDUCED ***************************/
#elif ST7735S_CFG_PIXEL == ST7735S_PIXEL_REDUCED

/* Conversion union; The order in the 'raw' structure is important. */
typedef union
{
	struct {uint8 g:4, r:4, rr:4, b:4, bb:4, gg:4;} raw;
	uint8 toSend[3];
} lcdst_frpx;

void lcdst_pushRPx(uint8 r, uint8 g, uint8 b, uint8 rr, uint8 gg, uint8 bb)
{
	lcdst_frpx data;
	
	/* Prepare the first pixel */
	data.raw.r = r & 0x0F;
	data.raw.g = g & 0x0F;
	data.raw.b = b & 0x0F;
	
	/* Prepare the second pixel */
	data.raw.rr = rr & 0x0F;
	data.raw.gg = gg & 0x0F;
	data.raw.bb = bb & 0x0F;
	
	/* Send pixels */
	writeData(data.toSend[0]);
	writeData(data.toSend[1]);
	writeData(data.toSend[2]);
} /* lcdst_pushRPx */

void lcdst_drawPx(uint8 x, uint8 y, uint8 r, uint8 g, uint8 b)
{
	lcdst_frpx data;
	
	/* Prepare the pixel */
	data.raw.r = r & 0x0F;
	data.raw.g = g & 0x0F;
	data.raw.b = b & 0x0F;
	
	/* Send pixel */
	if(lcdst_setWindow(x, y, x, y)) return;
	writeData(data.toSend[0]);
	writeData(data.toSend[1]);
} /* lcdst_drawPx */

void lcdst_drawHLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b)
{
	lcdst_frpx data;
	
	/* Draw only in the display space */
	if(l == 0) return;
	if((x+l-1) >= activeDisplay->width) l = activeDisplay->width - x;
	
	/* Prepare the pixels */
	data.raw.rr = data.raw.r = r & 0x0F;
	data.raw.gg = data.raw.g = g & 0x0F;
	data.raw.bb = data.raw.b = b & 0x0F;
	
	/* Draw the line */
	if(lcdst_setWindow(x, y, x+l-1, y)) return;
	while(l--)
	{
		writeData(data.toSend[0]);
		writeData(data.toSend[1]);
		writeData(data.toSend[2]);
	}
} /* lcdst_drawHLine */

void lcdst_drawVLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b)
{
	lcdst_frpx data;
	
	/* Draw only in the display space */
	if(l == 0) return;
	if((y+l-1) >= activeDisplay->height) l = activeDisplay->height - y;
	
	/* Prepare the pixels */
	data.raw.rr = data.raw.r = r & 0x0F;
	data.raw.gg = data.raw.g = g & 0x0F;
	data.raw.bb = data.raw.b = b & 0x0F;
	
	/* Draw the line */
	if(lcdst_setWindow(x, y, x, y+l-1)) return;
	while(l--)
	{
		writeData(data.toSend[0]);
		writeData(data.toSend[1]);
		writeData(data.toSend[2]);
	}
} /* lcdst_drawVLine */

void lcdst_drawFRect(uint8 x, uint8 y, uint8 w, uint8 h,
					uint8 r, uint8 g, uint8 b)
{
	lcdst_frpx data;
	
	/* Draw only in the display space */
	if((w == 0) || (h == 0)) return;
	if((x+w-1) >= activeDisplay->width)  w = activeDisplay->width  - x;
	if((y+h-1) >= activeDisplay->height) h = activeDisplay->height - y;
	
	/* Prepare the pixels */
	data.raw.rr = data.raw.r = r & 0x0F;
	data.raw.gg = data.raw.g = g & 0x0F;
	data.raw.bb = data.raw.b = b & 0x0F;
	
	/* Draw the filed rectangle */
	if(lcdst_setWindow(x, y, x+w-1, y+h-1)) return;
	while(w--) while(h--)
	{
		writeData(data.toSend[0]);
		writeData(data.toSend[1]);
		writeData(data.toSend[2]);
	}
} /* lcdst_drawFRect */

#endif /* ST7735S_CFG_PIXEL */

void lcdst_drawRect(uint8 x, uint8 y, uint8 w, uint8 h,
					uint8 r, uint8 g, uint8 b)
{
	/* Draw the rectangle */
	if((w >= 3) && (h >= 3))
	{
		lcdst_drawHLine(x,     y,     w,   r, g, b);
		lcdst_drawHLine(x,     y+h-1, w,   r, g, b);
		lcdst_drawVLine(x,     y+1,   h-2, r, g, b);
		lcdst_drawVLine(x+w-1, y+1,   h-2, r, g, b);
		return;
	}
	
	/* Draw the other shapes */
	lcdst_drawFRect(x, y, w, h, r, g, b);
} /* lcdst_drawRect */

void lcdst_drawScreen(uint8 r, uint8 g, uint8 b)
{
	/* Fill the whole screen with one color */
	//lcdst_drawFRect(0, 0, activeDisplay->width, activeDisplay->height, r, g, b);

	lcdst_drawFRect(0, 0, 127, 127, r, g, b);
} /* lcdst_drawScreen */


void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{

        writeCommand(0x2a);
        writeData(0x00);
        writeData(x_start+2);
        writeData(0x00);
        writeData(x_end+2);

        writeCommand(0x2b);
        writeData(0x00);
        writeData(y_start+3);
        writeData(0x00);
        writeData(y_end+3);

        writeCommand(0x2c);

}

//全屏填充函数
void LCD_Clear(unsigned int Color)
{
        int i, j;

        Lcd_SetRegion(0,0,128-1,128-1);

        for (i=0;i<128;i++) {
            for (j=0;j<128;j++) {
                //writeData_16(Color);
                writeData(Color >> 8);
                writeData(Color);
            }
        }
}

