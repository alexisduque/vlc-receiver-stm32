/**
  ******************************************************************************
  * File Name          : I2C.h
  * Date               : 05/04/2015 10:49:21
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
