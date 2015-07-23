#include "UART.h"

//#define TRANSMITTER_BOARD

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = " **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling **** ";
uint8_t aTxBuffer[] = {0xAA,0xBB};//"Hello World";
/* Buffer used for reception */
//uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t aRxBuffer[11];

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void test_Uart(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  #if !defined(TEST)
    HAL_Init();

    /* Configure LED3 and LED4 */
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);

    InitUartPorts();
    /* Configure the system clock to 180 MHz */
    //SystemClock_Config();
  #endif

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD

  /* Configure USER Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  /* Wait for USER Button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
  }
  
  /* The board sends the message and expects to receive it back */
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
  {
	  Error_Handler();
  }
  
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  #if !defined(TEST)
    HAL_Delay(200);
  #endif
  BSP_LED_Off(LED3);
  
  /*##-3- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
  {
    Error_Handler();  
  }
    
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  #if !defined(TEST)
    HAL_Delay(200);
  #endif
  BSP_LED_Off(LED3);
  
#else
  
  /* The board receives the message and sends it back */

  /*##-2- Put UART peripheral in reception process ###########################*/
  while(UartReady == RESET)	{
	  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
	  {
	  }
	  else
	  {
		  UartReady = SET;
	  }
  }
  /* Reset transmission flag */
  UartReady = RESET;

  /* Turn LED3 Off */
  BSP_LED_Off(LED3);
  
#endif /* TRANSMITTER_BOARD */
  
  /*##-4- Compare the sent and received buffers ##############################*/
  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
  {
    Error_Handler();  
  }
  
  /* Infinite loop */
  while (1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    
    /* Wait for 40ms */
  #if !defined(TEST)
    HAL_Delay(40);
  #endif
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}
