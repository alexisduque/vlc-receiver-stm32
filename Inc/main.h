/**
  ******************************************************************************
  * @file    Inc/main.h
  * @author  Alexis Duque
  * @version V0.0.1
  * @date    30-March-2015
  * @brief   Main ADC conversion with buffering
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"
#include "fifo.h"
#include "bitbuffer.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define BUFFER_SIZE             2000
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

extern FIFO_t AdcFIFO;
extern BufTypeDef AdcBitBuffer;
extern uint8_t AdcBuffer[BUFFER_SIZE];
#endif /* __MAIN_H */

/*****************************END OF FILE**************************************/
