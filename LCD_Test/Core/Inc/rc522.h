#ifndef __MFRC522_H
#define __MFRC522_H

#include "stm32f1xx_hal.h"
#include "main.h"

#define MFRC522_CS_PORT RC522_CS_GPIO_Port
#define MFRC522_CS_PIN RC522_CS_Pin
#define MFRC522_RST_PORT RC522_RST_GPIO_Port
#define MFRC522_RST_PIN RC522_RST_Pin

#define MI_OK 0
#define MI_NOTAGERR 1
#define MI_ERR 2

// RC522 Commands
#define PCD_IDLE              0x00
#define PCD_AUTHENT           0x0E
#define PCD_RECEIVE           0x08
#define PCD_TRANSMIT          0x04
#define PCD_TRANSCEIVE        0x0C
#define PCD_RESETPHASE        0x0F
#define PCD_CALCCRC           0x03

#define PICC_REQIDL           0x26
#define PICC_REQALL           0x52
#define PICC_ANTICOLL         0x93
#define PICC_SElECTTAG        0x93
#define PICC_AUTHENT1A        0x60
#define PICC_AUTHENT1B        0x61
#define PICC_READ             0x30
#define PICC_WRITE            0xA0
#define PICC_DECREMENT        0xC0
#define PICC_INCREMENT        0xC1
#define PICC_RESTORE          0xC2
#define PICC_TRANSFER         0xB0
#define PICC_HALT             0x50

void MFRC522_Init(void);
uint8_t MFRC522_ReadRegister(uint8_t addr);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t MFRC522_Anticoll(uint8_t *serNum);
uint8_t MFRC522_Check(uint8_t* id);

#endif
