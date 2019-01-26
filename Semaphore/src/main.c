/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
TaskHandle_t xTaskHandle_1=NULL;
TaskHandle_t xTaskHandle_2=NULL;
void vTask1(void *params);
void vTask2(void *params);

SemaphoreHandle_t xSemaphore=NULL;

#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

static void prvSetupHardware();
void printmsg(char *msg);

void printmsg(char *msg)
{
	for(uint32_t i=0;i<strlen(msg);i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
		USART_SendData(USART2,msg[i]);
	}
}
char msg[250];
int main(void)
{
#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("Hello Dai Nguyen");
#endif
	DWT->CTRL|=(1<<0);
	RCC_DeInit();
	SystemCoreClockUpdate();
	prvSetupHardware();

	sprintf(msg,"hello Dai Nguyen aaa\n");
	printmsg(msg);
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();
	xSemaphore = xSemaphoreCreateBinary();

	if(xSemaphore!=NULL)
	{
		xSemaphoreGive(xSemaphore);

		xTaskCreate(vTask1,"Task 1",configMINIMAL_STACK_SIZE,NULL,1,&xTaskHandle_1);//
		xTaskCreate(vTask2,"Task 2",configMINIMAL_STACK_SIZE,NULL,1,&xTaskHandle_2);//
		vTaskStartScheduler();
	}
}


void vTask1(void *params)
{
	while(1)
	{
		xSemaphoreTake(xSemaphore, portMAX_DELAY );
		{
			sprintf(msg,"hello from Task 1\n");
			printmsg(msg);
			xSemaphoreGive(xSemaphore);
		}

		vTaskDelay(500/portTICK_RATE_MS);
		//taskYIELD();///

	};
}

void vTask2(void *params)
{
	while(1)
	{
		xSemaphoreTake(xSemaphore, portMAX_DELAY );
		{
			sprintf(msg,"hello from Task 2\n");
			printmsg(msg);
			xSemaphoreGive(xSemaphore);
		}
		vTaskDelay(500/portTICK_RATE_MS);
		//taskYIELD();///
	};
}
static void prvSetupUSART(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	// zeroing every elements of the structure
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins));
	//PA2: RX; Pa3: RX

	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
	gpio_uart_pins.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode=GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd=GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_Speed = GPIO_High_Speed;

	GPIO_Init(GPIOA,&gpio_uart_pins );
	// AF mode setting
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);// TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);// TX

	// Uart parameter init
	// USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
	memset(&uart2_init,0,sizeof(uart2_init));
	uart2_init.USART_BaudRate=115200;
	uart2_init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	uart2_init.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	uart2_init.USART_Parity=USART_Parity_No;
	uart2_init.USART_StopBits=USART_StopBits_1;
	uart2_init.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&uart2_init);
	USART_Cmd(USART2,ENABLE);
	}

static void prvSetupHardware(void)
{
	prvSetupUSART();

	}
