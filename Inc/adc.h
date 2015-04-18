/**
  ******************************************************************************
  * File Name          : Inc/adc.h
  * Date               : 05/04/2015 10:49:20
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"

extern ADC_HandleTypeDef AdcHandle;

void MX_ADC_Init(void);
uint32_t ARA_ADC_GetValue(void);
int ARA_ADC_GetBit(uint32_t voltage);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
