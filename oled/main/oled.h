/*
 * 	oled.h
 */

void oled_init( void );
void oled_clear( void );
void oled_contrast( unsigned contrast );
void oled_text( unsigned line, const char *text );
