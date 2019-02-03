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
#include "queue.h"
#include "timers.h"
#include "stm32f4xx_tim.h"


void printmsg(char *msg);
void vTask1_menu_display(void *params);
void vTask2_command_handling(void *params);
void vTask3_command_processing(void *params);
void vTask4_uart_write(void *params);
void vTask5_led_toggle(void *params);

static void prvSetupHardware();

TaskHandle_t xTaskHandle_1=NULL;
TaskHandle_t xTaskHandle_2=NULL;
TaskHandle_t xTaskHandle_3=NULL;
TaskHandle_t xTaskHandle_4=NULL;
TaskHandle_t xTaskHandle_5=NULL;
TimerHandle_t led_timer_handle = NULL;

QueueHandle_t command_queue=NULL;
QueueHandle_t uart_write_queue=NULL;



SemaphoreHandle_t xSemaphore=NULL;

#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

typedef struct APP_CMD

{
	uint8_t COMMAND;
	uint8_t ARG[10];
}APP_CMD_t;
uint8_t command_buffer[20];
uint8_t command_len;
APP_CMD_t strct_app_command;

char menu[]={
		"\
		\\n 	LED On....1\
		\r\n 	LED OFF... 2\
		\r\n 	LED TOGGLE...3 \
		\r\n 	LED TOGGLE OFF...4  \
		\r\n 	LED READ STATUS...5\
		\r\n 	RTC PRINT Date TIME...6\
		\r\n 	EXIT APP...0\
		\r\n 	Type your option here: "
};
char count=0;
int count_timer2;
uint8_t  command_buffer[20];
char LED_TOGGLE=0;
char msg[250];
void printmsg(char *msg)
{
	for(uint32_t i=0;i<strlen(msg);i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
		USART_SendData(USART2,msg[i]);
	}
}
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
	GPIO_LD4_Setup();
	//SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start();
	//=======================================
	// create queue
	command_queue=xQueueCreate(10,sizeof(APP_CMD_t*));
	uart_write_queue=xQueueCreate(10,sizeof(char*));
	//=======================================

	//xSemaphore = xSemaphoreCreateBinary();

	if(command_queue!=NULL)
	{
		//xSemaphoreGive(xSemaphore);

		xTaskCreate(vTask1_menu_display,"Task 1 Menu",500,NULL,1,&xTaskHandle_1);//
		xTaskCreate(vTask2_command_handling,"Task 2 CMD Handling",500,NULL,1,&xTaskHandle_2);//
		xTaskCreate(vTask3_command_processing,"Task 3 CMD Process",500,NULL,1,&xTaskHandle_3);//
		xTaskCreate(vTask4_uart_write,"Task 4 UART Write",500,NULL,1,&xTaskHandle_4);//
		xTaskCreate(vTask5_led_toggle,"Task 5 LEG TOGGLE TASK",500,NULL,1,&xTaskHandle_5);//

		vTaskStartScheduler();
	}
	//for(;;);
}


void vTask1_menu_display(void *params)
{
	char *pData = menu;
	while(1)
	{
		//for(int i=0;i<=strlen(msg);i++)

		 /* xQueueSend: Only for use with queues that have a length of one - so the queue is either
		 * empty or full.*/
			//taskENTER_CRITICAL();
			taskENTER_CRITICAL();

			xQueueSend(uart_write_queue,&pData,portMAX_DELAY);
			xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
			//taskEXIT_CRITICAL();
			taskEXIT_CRITICAL();

		//sprintf(msg,menu);
		//printmsg(msg);
	}
}

void vTask2_command_handling(void *params)
{
	char cmd_code;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//sprintf(msg,"Hello from Task 2");
		//printmsg(msg);
		// get command code and save into APP_CMD_t
		taskENTER_CRITICAL();
		cmd_code=command_buffer[0];
		strct_app_command.COMMAND=cmd_code;

		taskEXIT_CRITICAL();
		/*if(command_buffer[0]=='1')
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_SET);
			LED_TOGGLE=0;
		}
		else if(command_buffer[0]=='2')
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_RESET);
			LED_TOGGLE=0;
		}
		else if(command_buffer[0]=='3')
		{
			LED_TOGGLE=1;
		}*/
		xQueueSend(command_queue,&strct_app_command,portMAX_DELAY);



	}
}

void vTask3_command_processing(void *params)
{
	APP_CMD_t app;
	while(1)
	{

		xQueueReceive(command_queue,&app,portMAX_DELAY);
		//taskENTER_CRITICAL();

		if(app.COMMAND=='1')
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_SET);
			LED_TOGGLE=0;
		}
		else if (app.COMMAND=='2')
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_RESET);
			LED_TOGGLE=0;
			//xTimerStart
		}
		else if (app.COMMAND=='3' )
		{
			//LED_TOGGLE=1;
			LED_Toggle();

		}

		//taskEXIT_CRITICAL();


	}
}
void vTask4_uart_write(void *params)
{
	char  *pData=NULL;
	while(1)
	{
			xQueueReceive(uart_write_queue,&pData,portMAX_DELAY);
			printmsg(pData);
	}

}

void vTask5_led_toggle(void *params)
{
	while(1)
	{
		//if(LED_TOGGLE==1)
			//LED_Toggle();
	}
	}

void USART2_IRQHandler()
{
	//
	uint16_t databyte;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
	{
		databyte=USART_ReceiveData(USART2);
		command_buffer[count]=databyte & 0xFF;
		count++;
		if(databyte=='\r')
		{
			count=0;
			xTaskNotifyFromISR(xTaskHandle_1,0,eNoAction,&xHigherPriorityTaskWoken);
			xTaskNotifyFromISR(xTaskHandle_2,0,eNoAction,&xHigherPriorityTaskWoken);
		}
	}


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
	gpio_uart_pins.GPIO_Mode=GPIO_Mode_AF;// Alternate function
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
	// Set USART prioprity
	NVIC_SetPriority(USART2_IRQn,5);
	//enable UART reception INT
	NVIC_EnableIRQ(USART2_IRQn);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	}

static void prvSetupHardware(void)
{
	prvSetupUSART();

	}
void GPIO_LD4_Setup(void)
{
	GPIO_InitTypeDef GPIO12_init;


	GPIO12_init.GPIO_Pin=GPIO_Pin_12;
	GPIO12_init.GPIO_Mode=GPIO_Mode_OUT;
	GPIO12_init.GPIO_Speed=GPIO_High_Speed;
	GPIO12_init.GPIO_OType=GPIO_OType_PP;
	//GPIO12_init.GPIO_PuPd=GPIO_PuPd_UP;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_Init(GPIOD,&GPIO12_init);



	}

void Delay(void)
{
	int count;
	for(count=0;count<=20000;count++);
	}


void led_togglebit(void)
{
	GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	}
void LED_Toggle(void)
{
	//if(LED_TOGGLE==1)
	{
		/*Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();*/
		if(led_timer_handle==NULL)
		{
			led_timer_handle = xTimerCreate("LED-TIMER",pdMS_TO_TICKS(500),pdTRUE,NULL,led_togglebit);
			xTimerStart(led_timer_handle,portMAX_DELAY);
		}
		else
		{
			xTimerStart(led_timer_handle,portMAX_DELAY);

		}
		/*GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_RESET);
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		Delay();
		GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_SET);*/
	}
}

void TIM2_IRQHandler()
{
	count_timer2++;

	if (count_timer2==10000 && LED_TOGGLE==1)
	{
		count_timer2=0;
		//GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		//GPIO_WriteBit(GPIOD,GPIO_Pin_12, Bit_SET);
		//sprintf(msg,"Hello from TIMER 2");
		//printmsg(msg);
	}

}
void TIMER_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct ;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitStruct.TIM_Prescaler=0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Period=0xFFFF;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter=;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn,5);
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);

}


