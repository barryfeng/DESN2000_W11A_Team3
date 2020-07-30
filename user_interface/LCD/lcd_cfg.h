//These parameters configure the LPC2478's LCD controller to our specific LCD
static tLcdParams lcd_config =
{
    28,        /* Horizontal back porch */
    10,       /* Horizontal front porch */
    2,       /* HSYNC pulse width */
    240,      /* Pixels per line */
    3,        /* Vertical back porch */
    2,       /* Vertical front porch */
    2,        /* VSYNC pulse width */
    320,      /* Lines per panel */
    0,        /* Do not invert output enable */
    1,        /* Invert panel clock */
    1,        /* Invert HSYNC */
    1,        /* Invert VSYNC */
    1,        /* AC bias frequency (not used) */
    16,       /* Bits per pixel */
    LCD_ADTFT,    /* LCD panel type */
    0,        /* Single panel display */
};
