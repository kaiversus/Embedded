#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stm32f1xx_hal.h"

void lcd_init(void);   // khoi tao lcd
void lcd_send_cmd(char cmd);  // gui lenh
void lcd_send_data(char data);  // gui du lieu (1 ky tu)
void lcd_send_string(char *str);  // gui chuoi ky tu
void lcd_put_cur(int row, int col);  // dua con tro den hang row (0-1), cot col (0-15)
void lcd_clear(void); // xoa man hinh

#endif
