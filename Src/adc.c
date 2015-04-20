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
DMA_HandleTypeDef DmaHandle;

uint32_t resultDMA;
uint8_t bitBuffer = 0x00;
uint32_t adcResultDMA;
int counter = 0;
unsigned char bit = 0;
uint32_t ADCVoltageValue = 0;
long ADCTimer = 0;
long ADCCount = 0;
long offset = 0;

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
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.Resolution = ADC_RESOLUTION12b;
    AdcHandle.Init.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
    AdcHandle.Init.ScanDirection = ADC_SCAN_DIRECTION_UPWARD;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.ContinuousConvMode = ENABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIG_EDGE_NONE;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection = EOC_SINGLE_CONV;
    AdcHandle.Init.Overrun = OVR_DATA_PRESERVED;
    AdcHandle.Init.LowPowerAutoWait = DISABLE;
    AdcHandle.Init.LowPowerFrequencyMode = DISABLE;
    AdcHandle.Init.LowPowerAutoOff = DISABLE;

    HAL_ADC_Init(&AdcHandle);
    HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

    sConfig.Channel = ADC_CHANNEL_0;
    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
    __ADC1_CLK_DISABLE();
    __ADC1_CLK_ENABLE();
    HAL_ADC_Start_DMA(&AdcHandle, &resultDMA, 1);

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

        /* Peripheral DMA init*/

        DmaHandle.Instance = DMA1_Channel1;
        DmaHandle.Init.Request = DMA_REQUEST_0;
        DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
        DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
        DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        DmaHandle.Init.Mode = DMA_CIRCULAR;
        DmaHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
        HAL_DMA_Init(&DmaHandle);

        __HAL_LINKDMA(AdcHandle, DMA_Handle, DmaHandle);
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

        /* Peripheral DMA DeInit*/
        HAL_DMA_DeInit(AdcHandle->DMA_Handle);

    }
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */


int ARA_ADC_GetBit(uint32_t voltage)
{
    return voltage > THRESHOLD ? 1 : 0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    ADCTimer = HAL_GetTick() - offset;

    if(ADCCount == 500000)
    {
        ADCCount = 0;
        offset = HAL_GetTick();
    }
    ADCCount++,
    ADCVoltageValue = 3330 * resultDMA / 4095;
    bit = ARA_ADC_GetBit(ADCVoltageValue);
    if (bit == 1)
    {
        bitBuffer |= (1u << (7 - counter));
    }
    counter++;
    if (counter > 7)
    {
        FIFO_write_trample(&AdcFIFO, &bitBuffer, 1);
        counter = 0;
        bitBuffer = 0x00;
    }
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
