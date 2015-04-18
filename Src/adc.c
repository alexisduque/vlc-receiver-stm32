/**
  ******************************************************************************
  * File Name          : Src/adc.c
  * Date               : 05/04/2015 10:49:20
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define THRESHOLD 2000
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;

/* Private function prototypes -----------------------------------------------*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);

/* ADC init function */
void MX_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    AdcHandle.Instance = ADC1;
    AdcHandle.Init.OversamplingMode = DISABLE;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV1;
    AdcHandle.Init.Resolution = ADC_RESOLUTION12b;
    AdcHandle.Init.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    AdcHandle.Init.ScanDirection = ADC_SCAN_DIRECTION_UPWARD;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.ContinuousConvMode = ENABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIG_EDGE_NONE;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.EOCSelection = EOC_SINGLE_CONV;
    AdcHandle.Init.Overrun = OVR_DATA_PRESERVED;
    AdcHandle.Init.LowPowerAutoWait = DISABLE;
    AdcHandle.Init.LowPowerFrequencyMode = DISABLE;
    AdcHandle.Init.LowPowerAutoOff = DISABLE;
    HAL_ADC_Init(&AdcHandle);

    /** Start ADC Calibration
    */
    HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);
    /**Configure for the selected ADC regular channel to be converted.
    */
    sConfig.Channel = ADC_CHANNEL_0;
    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* AdcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(AdcHandle->Instance==ADC1)
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

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* AdcHandle)
{

    if(AdcHandle->Instance==ADC1)
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
    if(HAL_ADC_Start(&AdcHandle) != HAL_OK)
    {
        /* Start Conversation Error */
        Error_Handler();
    }

    /* Wait for the end of conversion */
    HAL_ADC_PollForConversion(&AdcHandle, 10);

    /* Check if the continuous conversion of regular channel is finished */
    if(HAL_ADC_GetState(&AdcHandle) == HAL_ADC_STATE_EOC)
    {
        /* Get the converted value of regular channel */
        return 3330 * HAL_ADC_GetValue(&AdcHandle) / 4095;
    }
    return 0;
}

int ARA_ADC_GetBit(uint32_t voltage)
{
    return voltage > THRESHOLD ? 1 : 0;
}
/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
