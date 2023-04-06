#ifndef __MPSLCD_H
#define __MPSDLCD_H

void lcd_send_string(char* string);
void lcd_init();
void lcd_send_data (char data);
void lcd_send_cmd (char cmd);
void lcd_set_cursor(int row, int column);
void lcd_clear();

#endif