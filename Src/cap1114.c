/*
 * cap1114.c
 *
 *  Created on: Dec 21, 2017
 *      Author: Ryan
 */

#include "cap1114.h"

uint8_t cap1114_write(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len)
{
	uint8_t ret = 0;
	ret |= HAL_I2C_Master_Transmit(hi2c, CAP1114_I2C_ADDR, data, len, 1000);

	return ret;
}

uint8_t cap1114_read(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len)
{
	uint8_t ret = 0;
	ret |= cap1114_write(hi2c, data, 1);
	ret |= HAL_I2C_Master_Receive(hi2c, CAP1114_I2C_ADDR, data, len, 1000);

	return ret;
}

uint8_t cap1114_probe(I2C_HandleTypeDef *hi2c)
{
	uint8_t ret = 0;
	uint8_t pid = CAP1114_PID_REG;
	ret = cap1114_read(hi2c, &pid, 1);

	printf("pid %u 0x%x\n", ret, pid);

	return (ret == CAP1114_PID);
}

uint8_t cap1114_button_status(I2C_HandleTypeDef *hi2c, uint16_t *buttons)
{
	uint8_t ret = 0;
	uint8_t *but = (uint8_t*)buttons;
	but[0] = CAP1114_BUTTON_STATUS1;
	ret |= cap1114_read(hi2c, but, 2);


	uint16_t temp = but[0] | but[1] << 8;
	uint8_t i;
	for (i = 0; i < 16; i++)
	{
		printf("%u ", temp & 0x1);
		temp >>= 1;
	}

	printf("\n");

	return ret;
}
