/**
  ******************************************************************************
  * @file    Src/main.c
  * @author  Alexis Duque
  * @version V0.0.1
  * @date    30-March-2015
  * @brief   Main ADC conversion with buffering
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"
#include "fifo.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FIFO_t AdcFIFO;
uint8_t AdcBuffer[BUFFER_SIZE];
uint8_t testVal = 0x02;
unsigned char bit = 0;
uint32_t ADCVoltageValue = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Init LED2 */
    BSP_LED_Init(LED2);
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    FIFO_init(&AdcFIFO, AdcBuffer, BUFFER_SIZE);
    MX_GPIO_Init();
    MX_ADC_Init();
    MX_I2C1_Init();

    /* Infinite loop */
    while(1)
    {
        ARA_I2C_Listen();
        ADCVoltageValue = ARA_ADC_GetValue();
        bit = ARA_ADC_GetBit(ADCVoltageValue);
        //bit = !bit;
        FIFO_write_trample(&AdcFIFO, &bit, 1);
        //FIFO_write_trample(&AdcFIFO, &ADCVoltageValue, 1);
    }

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    __SYSCFG_CLK_ENABLE();

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* Turn On LED2 */
    BSP_LED_On(LED2);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/*****************************END OF FILE**************************************/
