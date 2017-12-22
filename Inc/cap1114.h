/*
 * cap1114.h
 *
 *  Created on: Dec 21, 2017
 *      Author: Ryan
 */

#ifndef CAP1114_H_
#define CAP1114_H_

#include "stm32f0xx_hal.h"

#define CAP1114_I2C_ADDR			0x50
#define CAP1114_NUM_SENSORS			14

#define CAP1114_BUTTON_STATUS1		0x03
#define CAP1114_BUTTON_STATUS2		0x04

#define CAP1114_PID_REG				0xFD

#define CAP1114_PID					0x3A


uint8_t cap1114_write(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len);

uint8_t cap1114_read(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len);

uint8_t cap1114_probe(I2C_HandleTypeDef *hi2c);

uint8_t cap1114_button_status(I2C_HandleTypeDef *hi2c, uint16_t *buttons);

#endif /* CAP1114_H_ */
