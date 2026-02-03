/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ADC01_Pin GPIO_PIN_0
#define ADC01_GPIO_Port GPIOA
#define ADC02_Pin GPIO_PIN_1
#define ADC02_GPIO_Port GPIOA
#define UART_TX_WIFI_Pin GPIO_PIN_2
#define UART_TX_WIFI_GPIO_Port GPIOA
#define UART_RX_WIFI_Pin GPIO_PIN_3
#define UART_RX_WIFI_GPIO_Port GPIOA
#define GPIO01_Pin GPIO_PIN_4
#define GPIO01_GPIO_Port GPIOA
#define INPUT_flag01_Pin GPIO_PIN_5
#define INPUT_flag01_GPIO_Port GPIOA
#define INPUT_flag02_Pin GPIO_PIN_6
#define INPUT_flag02_GPIO_Port GPIOA
#define INPUT_flag03_Pin GPIO_PIN_7
#define INPUT_flag03_GPIO_Port GPIOA
#define INPUT_flag04_Pin GPIO_PIN_0
#define INPUT_flag04_GPIO_Port GPIOB
#define INPUT_flag05_Pin GPIO_PIN_1
#define INPUT_flag05_GPIO_Port GPIOB
#define UART_TX_GSM_Pin GPIO_PIN_10
#define UART_TX_GSM_GPIO_Port GPIOB
#define UART_RX_GSM_Pin GPIO_PIN_11
#define UART_RX_GSM_GPIO_Port GPIOB
#define OUTPUT00_Pin GPIO_PIN_12
#define OUTPUT00_GPIO_Port GPIOB
#define OUTPUT01_Pin GPIO_PIN_13
#define OUTPUT01_GPIO_Port GPIOB
#define OUTPUT02_Pin GPIO_PIN_14
#define OUTPUT02_GPIO_Port GPIOB
#define OUTPUT03_Pin GPIO_PIN_15
#define OUTPUT03_GPIO_Port GPIOB
#define UART_TXEN_RS485_Pin GPIO_PIN_8
#define UART_TXEN_RS485_GPIO_Port GPIOA
#define UART_TX_RS485_Pin GPIO_PIN_9
#define UART_TX_RS485_GPIO_Port GPIOA
#define UART_RX_RS485_Pin GPIO_PIN_10
#define UART_RX_RS485_GPIO_Port GPIOA
#define Power_flag_Pin GPIO_PIN_11
#define Power_flag_GPIO_Port GPIOA
#define INPUT_flag06_Pin GPIO_PIN_12
#define INPUT_flag06_GPIO_Port GPIOA
#define OUTPUT04_Pin GPIO_PIN_15
#define OUTPUT04_GPIO_Port GPIOA
#define RF_R2_Pin GPIO_PIN_3
#define RF_R2_GPIO_Port GPIOB
#define RF_R1_Pin GPIO_PIN_4
#define RF_R1_GPIO_Port GPIOB
#define I2c_WP_EEPROM_Pin GPIO_PIN_5
#define I2c_WP_EEPROM_GPIO_Port GPIOB
#define I2c_SCL_EEPROM_Pin GPIO_PIN_6
#define I2c_SCL_EEPROM_GPIO_Port GPIOB
#define I2c_SDA_EEPROM_Pin GPIO_PIN_7
#define I2c_SDA_EEPROM_GPIO_Port GPIOB
#define OUTPUT05_Pin GPIO_PIN_8
#define OUTPUT05_GPIO_Port GPIOB
#define OUTPUT06_Pin GPIO_PIN_9
#define OUTPUT06_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
