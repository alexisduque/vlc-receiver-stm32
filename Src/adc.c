/**
  ******************************************************************************
  * File Name          : ADC.c
  * Date               : 05/04/2015 10:49:20
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "gpio.h"

ADC_HandleTypeDef hadc;

/* ADC init function */
void MX_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    hadc.Instance = ADC1;
    hadc.Init.OversamplingMode = DISABLE;
    hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc.Init.Resolution = ADC_RESOLUTION10b;
    hadc.Init.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
    hadc.Init.ScanDirection = ADC_SCAN_DIRECTION_UPWARD;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ContinuousConvMode = ENABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIG_EDGE_NONE;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.EOCSelection = EOC_SINGLE_CONV;
    hadc.Init.Overrun = OVR_DATA_PRESERVED;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerFrequencyMode = DISABLE;
    hadc.Init.LowPowerAutoOff = DISABLE;
    HAL_ADC_Init(&hadc);

    /** Start ADC Calibration
    */
    HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
    /**Configure for the selected ADC regular channel to be converted.
    */
    sConfig.Channel = ADC_CHANNEL_0;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(hadc->Instance==ADC1)
    {

        /* Peripheral clock enable */
        __ADC1_CLK_ENABLE();

        /**ADC GPIO Configuration
        PA0-WKUP     ------> ADC_IN0
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

    if(hadc->Instance==ADC1)
    {
        /* Peripheral clock disable */
        __ADC1_CLK_DISABLE();

        /**ADC GPIO Configuration
        PA0-WKUP     ------> ADC_IN0
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    }
}

uint32_t ARA_ADC_GetValue(void)
{
    /* Start the conversion process */
    if(HAL_ADC_Start(&hadc) != HAL_OK)
    {
        /* Start Conversation Error */
        //Error_Handler();
    }

    /* Wait for the end of conversion */
    HAL_ADC_PollForConversion(&hadc, 10);

    /* Check if the continuous conversion of regular channel is finished */
    if(HAL_ADC_GetState(&hadc) == HAL_ADC_STATE_EOC)
    {
        /* Get the converted value of regular channel */
        return HAL_ADC_GetValue(&hadc);
    }
    return 0;
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
