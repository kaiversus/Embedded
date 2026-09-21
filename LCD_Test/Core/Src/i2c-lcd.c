#include "i2c-lcd.h"

extern I2C_HandleTypeDef hi2c1;  // Su dung bien hi2c1 da duoc khai bao ben main.c

// Dia chi cua module I2C (PCF8574T la 0x4E, PCF8574AT la 0x7E)
// Theo anh ban chup thi cua ban la con PCF8574T nen de nguyen 0x4E
#define SLAVE_ADDRESS_LCD 0x4E 

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd&0xf0);
  data_l = ((cmd<<4)&0xf0);
  data_t[0] = data_u|0x0C;  //en=1, rs=0
  data_t[1] = data_u|0x08;  //en=0, rs=0
  data_t[2] = data_l|0x0C;  //en=1, rs=0
  data_t[3] = data_l|0x08;  //en=0, rs=0
  HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (data&0xf0);
  data_l = ((data<<4)&0xf0);
  data_t[0] = data_u|0x0D;  //en=1, rs=1
  data_t[1] = data_u|0x09;  //en=0, rs=1
  data_t[2] = data_l|0x0D;  //en=1, rs=1
  data_t[3] = data_l|0x09;  //en=0, rs=1
  HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
  lcd_send_cmd (0x01);
  HAL_Delay(2);
}

void lcd_put_cur(int row, int col)
{
  switch (row)
  {
    case 0:
      col |= 0x80;
      break;
    case 1:
      col |= 0xC0;
      break;
  }
  lcd_send_cmd (col);
}

void lcd_init (void)
{
  // 4 bit initialisation
  HAL_Delay(50);  
  lcd_send_cmd (0x30);
  HAL_Delay(5);  
  lcd_send_cmd (0x30);
  HAL_Delay(1);  
  lcd_send_cmd (0x30);
  HAL_Delay(10);
  lcd_send_cmd (0x20);  // 4bit mode
  HAL_Delay(10);

  // display initialisation
  lcd_send_cmd (0x28); // Function set 
  HAL_Delay(1);
  lcd_send_cmd (0x08); //Display on/off control
  HAL_Delay(1);
  lcd_send_cmd (0x01);  // clear display
  HAL_Delay(2);
  lcd_send_cmd (0x06); //Entry mode set 
  HAL_Delay(1);
  lcd_send_cmd (0x0C); //Display on/off control (tat con tro)
}

void lcd_send_string (char *str)
{
  while (*str) lcd_send_data (*str++);
}
