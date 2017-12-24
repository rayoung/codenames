/*
 * cap1114.h
 *
 *  Created on: Dec 21, 2017
 *      Author: Ryan
 */

#ifndef CAP1114_H_
#define CAP1114_H_

#include "main.h"

#define CAP1114_I2C_ADDR				0x50
#define CAP1114_NUM_SENSORS				14

#define CAP1114_MAIN_STATUS_CTRL		0x00

#define CAP1114_BUTTON_STATUS1			0x03
#define CAP1114_BUTTON_STATUS2			0x04

#define CAP1114_CONFIG					0x20
#define CAP1114_CONFIG2					0x40

#define CAP1114_SENSOR_ENABLE			0x21

#define CAP1114_GROUP_CONFIG1			0x23

#define CAP1114_INTERRUPT_ENABLE1		0x27
#define CAP1114_INTERRUPT_ENABLE2		0x28

#define CAP1114_PRODUCT_ID				0xFD

#define CAP1114_PID						0x3A


void cap1114_write(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len);

void cap1114_read(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len);

void cap1114_init(I2C_HandleTypeDef *hi2c);

void cap1114_probe(I2C_HandleTypeDef *hi2c);

void cap1114_button_status(I2C_HandleTypeDef *hi2c, uint16_t *buttons);

#endif /* CAP1114_H_ */
