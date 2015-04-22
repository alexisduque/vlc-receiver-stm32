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
#define THRESHOLD 800
#define DMA_BUFFER_SIZE 512
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
DMA_HandleTypeDef DmaHandle;

uint32_t resultDMA[DMA_BUFFER_SIZE];
uint8_t bitBuffer = 0x00;
uint32_t adcResultDMA;
int counter = 0;
unsigned char bit = 0;
uint32_t ADCVoltageValue = 0;
long ADCTimer = 0;
long ADCCount = 0;
long offset = 0;

/* Private function prototypes -----------------------------------------------*/

/* ADC init function */
void MX_ADC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();
    /* DMA controller clock enable */
    __DMA1_CLK_ENABLE();

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
    HAL_DMA_DeInit(&DmaHandle);
    HAL_DMA_Init(&DmaHandle);

    __HAL_LINKDMA(&AdcHandle, DMA_Handle, DmaHandle);

    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    ADC_ChannelConfTypeDef sConfig;
    ADC_AnalogWDGConfTypeDef AnalogWDGConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    AdcHandle.Instance = ADC1;
    AdcHandle.Init.OversamplingMode = DISABLE;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV1;
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

    /**Configure the analog watchdog
    */
    AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
    AnalogWDGConfig.Channel = ADC_CHANNEL_0;
    AnalogWDGConfig.ITMode = ENABLE;
    AnalogWDGConfig.HighThreshold = 0;
    AnalogWDGConfig.LowThreshold = 0;
    HAL_ADC_AnalogWDGConfig(&AdcHandle, &AnalogWDGConfig);

    HAL_ADC_Start_DMA(&AdcHandle, &resultDMA, DMA_BUFFER_SIZE);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* AdcHandle)
{
    // Nothing to do there;
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

int ARA_ADC_Threashold(uint32_t voltage)
{
    return voltage > THRESHOLD ? 1 : 0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    int i = 0;

    /* DEBUG SAMPLING RATE PURPOSE */
    ADCTimer = HAL_GetTick() - offset;
    if(ADCTimer == 1000)
    {
        ADCCount = 0;
        offset = HAL_GetTick();
    }
    ADCCount++;
    /* END DEBUG */

    for (i = 0; i < DMA_BUFFER_SIZE; i++)
    {
        ADCVoltageValue = 3330 * resultDMA[0] / 4065;
        bit = ARA_ADC_Threashold(ADCVoltageValue);
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
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
