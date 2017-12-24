/*
 * cap1114.c
 *
 *  Created on: Dec 21, 2017
 *      Author: Ryan
 */

#include "cap1114.h"

static uint8_t i2c_buf[4];
static uint16_t button_mask[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x100, 0x200,
						  0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000
						  };

static uint16_t buttons_pressed = 0;

static void cap1114_clear_int(I2C_HandleTypeDef *hi2c)
{
	/* read status register */
	i2c_buf[0] = CAP1114_MAIN_STATUS_CTRL;
	cap1114_read(hi2c, i2c_buf, 1);

	/* clear interrupt bit */
	i2c_buf[1] = i2c_buf[0] & 0xFE;
	i2c_buf[0] = CAP1114_MAIN_STATUS_CTRL;
	cap1114_write(hi2c, i2c_buf, 2);
}

void cap1114_write(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len)
{
	if (HAL_I2C_Master_Transmit(hi2c, CAP1114_I2C_ADDR, data, len, 1000) != HAL_OK)
	{
		printf("error %s: %u\n", __FILE__, __LINE__);
	}
}

void cap1114_read(I2C_HandleTypeDef *hi2c, uint8_t *data, uint8_t len)
{
	cap1114_write(hi2c, data, 1);
	if (HAL_I2C_Master_Receive(hi2c, CAP1114_I2C_ADDR, data, len, 1000) != HAL_OK)
	{
		printf("error %s: %u\n", __FILE__, __LINE__);
	}
}

void cap1114_init(I2C_HandleTypeDef *hi2c)
{
	printf("cap1114\n");

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure RESET pin */
	GPIO_InitStruct.Pin = CAP1114_RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(CAP1114_RESET_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(CAP1114_RESET_Port, CAP1114_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(CAP1114_RESET_Port, CAP1114_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	/* Configure ALERT pin */
	GPIO_InitStruct.Pin = CAP1114_ALERT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(CAP1114_ALERT_Port, &GPIO_InitStruct);

	/* Enable and set EXTI line 4 Interrupt */
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

	cap1114_probe(hi2c);

	/* change press time for group sensors*/
	i2c_buf[0] = CAP1114_GROUP_CONFIG1;
	i2c_buf[1] = 0;
	cap1114_write(hi2c, i2c_buf, 2);

	/* disable repeat rate */
	i2c_buf[0] = CAP1114_CONFIG;
	i2c_buf[1] = 0x20;
	cap1114_write(hi2c, i2c_buf, 2);

	/* disable slider and int on release */
	i2c_buf[0] = CAP1114_CONFIG2;
	i2c_buf[1] = 0x03;
	cap1114_write(hi2c, i2c_buf, 2);

	/* clear interrupt */
	cap1114_clear_int(hi2c);
}

void cap1114_probe(I2C_HandleTypeDef *hi2c)
{
	uint8_t pid = CAP1114_PRODUCT_ID;
	cap1114_read(hi2c, &pid, 1);

	if (pid != CAP1114_PID)
	{
		printf("0x%02x != 0x%02x\n", pid, CAP1114_PID);
	}
}

void cap1114_button_status(I2C_HandleTypeDef *hi2c, uint16_t *buttons)
{
	uint8_t *but = (uint8_t*)buttons;
	but[0] = CAP1114_BUTTON_STATUS1;
	cap1114_read(hi2c, but, 2);

	cap1114_clear_int(hi2c);

	uint16_t pressed = buttons_pressed ^ buttons[0];
	buttons_pressed |= buttons[0];
	uint8_t i = 0;

	printf("pressed ");
	for (i = 0; i < CAP1114_NUM_SENSORS; i++)
	{
		if (buttons_pressed & button_mask[i])
		{
			printf("%u ", i);
		}
	}
	printf("\n");
}
