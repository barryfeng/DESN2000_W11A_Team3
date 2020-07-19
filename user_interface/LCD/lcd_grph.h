/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 *****************************************************************************/

#ifndef _LCD_GRPH_
#define _LCD_GRPH_


/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320
#define COLOR_DEPTH_BITS 16

typedef unsigned short lcd_color_t;

#define   BLACK			0x0000		/*   0,   0,   0 */
#define   NAVY			0x000F      /*   0,   0, 128 */
#define   DARK_GREEN	0x03E0      /*   0, 128,   0 */
#define   DARK_CYAN		0x03EF      /*   0, 128, 128 */
#define   MAROON		0x7800      /* 128,   0,   0 */
#define   PURPLE		0x780F      /* 128,   0, 128 */
#define   OLIVE			0x7BE0      /* 128, 128,   0 */
#define   LIGHT_GRAY	0xC618      /* 192, 192, 192 */
#define   DARK_GRAY		0x7BEF      /* 128, 128, 128 */
#define   BLUE			  0x001F 
#define   BLUE_BG			0xBFFF 
#define   GREEN			0x07E0      /*   0, 255,   0 */
#define   CYAN          0x07FF      /*   0, 255, 255 */
#define   RED           0xF800      /* 255,   0,   0 */
#define   MAGENTA		0xF81F      /* 255,   0, 255 */
#define   YELLOW		0xFFE0      /* 255, 255, 0   */
#define   WHITE			0xFFFF      /* 255, 255, 255 */

#define		RGBENC(r,g,b)	((r&0x1F)<<11) | ((g&0x3F)<<5) | (b&0x1F)
#define 	DECODE_R(e) (e>>11)&0x1F
#define 	DECODE_G(e) (e>>5)&0x3F
#define 	DECODE_B(e) (e)&0x1F

int HSVto16BIT(int h,int s,int v);
void HSVtoRGB(int *r, int *g,int *b,int h,int s,int v );


/******************************************************************************
 * Public functions
 *****************************************************************************/


void lcd_fillScreen(lcd_color_t color);

void lcd_point(unsigned short x, unsigned short y, lcd_color_t color);

void lcd_drawRect(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_fillRect(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_circle(unsigned short x0, unsigned short y0, unsigned short r, lcd_color_t color);

void lcd_fillcircle(unsigned short x0, unsigned short y0, unsigned short r, lcd_color_t color);

unsigned char lcd_putChar(unsigned short x, unsigned short y, unsigned char ch);

void lcd_putString(unsigned short x, unsigned short y, unsigned char *pStr);

void lcd_fontColor(lcd_color_t foreground, lcd_color_t background);

void lcd_picture(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned short *pPicture);





#endif /* _LCD_GRPH_ */
