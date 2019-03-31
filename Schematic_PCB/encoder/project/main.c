#include "stm32f10x.h"
#include "stdio.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
  	/* e.g. write a character to the LCD */
	//lcd_Data_Write((u8)ch);
	USART_SendData(USART2,(u8)ch);
	
	/*Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
	{}

  	return ch;
}


GPIO_InitTypeDef 					GPIO_InitStructure;
TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
NVIC_InitTypeDef					NVIC_InitStructure;
USART_InitTypeDef					UART_InitStructure;
TIM_OCInitTypeDef  				TIM_OCInitStructure;
TIM_BDTRInitTypeDef 			TIM_BDTRInitStructure;



uint32_t encoder_pulse1=0, encoder_pulse2=0; 
uint32_t count_temp1=0 , count_temp2 =0, count_test=0;
uint32_t count_recent1 =0, count_recent2 =0, count_update1=0, count_update2=0;
float motor_speed1=0, motor_speed2 =0;

void GPIO_Configuration(void);
void Delay_ms(uint16_t time);
void TIM4_TIME(void);
void TIM1_PWM(void);
void TIM23_ENCODER_Configuration(void);
void UART_Configuration (void);

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	GPIO_Configuration();
	UART_Configuration();
	TIM4_TIME();
	TIM1_PWM();
	TIM23_ENCODER_Configuration();
  while (1)
  {
		printf("so vong/phut = %4.3f,%4.3f \n",motor_speed1,motor_speed2 );
		GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)(1^GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)));
		Delay_ms(100);
  }
}

void GPIO_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Delay_ms(uint16_t time)
{
uint32_t time_n=time*12000;
	while(time_n!=0){time_n--;}

}
void UART_Configuration (void)
	{
		/*Cap clock cho USART và port su dung*/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
			/* Cau Tx mode AF_PP, Rx mode FLOATING  */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		/*Cau hinh USART*/
			UART_InitStructure.USART_BaudRate = 115200;
			UART_InitStructure.USART_WordLength = USART_WordLength_8b;
			UART_InitStructure.USART_StopBits = USART_StopBits_1;
			UART_InitStructure.USART_Parity = USART_Parity_No;
			UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART2, &UART_InitStructure);
				
			/* Cho phep UART hoat dong */
			USART_Cmd(USART2, ENABLE);
	
	}

void TIM4_TIME(void)
{
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4 , ENABLE);
  
	// TIM3 configuration 
	TIM_TimeBaseInitStructure.TIM_Period = 99; // delay 10ms          
	TIM_TimeBaseInitStructure.TIM_Prescaler = (7200 - 1);	 //1khz
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	// Clear TIM3 update pending flags 
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	//TIM enable counter 
	TIM_Cmd(TIM4, ENABLE);	
	//Enable TIM3 Update interrupts 
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* 2 bit for pre-emption priority, 2 bits for subpriority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// Enable the TIM4 global Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM1_PWM(void)
{
	/*Enable clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	/*Configure PWM Pin out TIM1 CH2- CH2N CH3 - CH3N*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
/*Configure Frequency for PWM by: 72Mhz/(Period+1)/(Prescaler+1) */
        
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;	 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInitStructure.TIM_Period = 1000; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	/*Configure PWM*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 500;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	/*PWM Channel 1*/
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
	
//	/*PWM Channel 2*/
	TIM_OCInitStructure.TIM_Pulse = 700;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
	/*Enable PWM_TIM*/
  TIM_Cmd(TIM1, ENABLE);
	 /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void TIM23_ENCODER_Configuration(void)
{
	TIM_ICInitTypeDef			TIM_ICInitStructure;
	/*Enable clock for timers*/
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM2, ENABLE);
	
	/*Enable clock for GPIO ports*/
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE);
													
	/*Configure for Encoder interface 1*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*Configure for Encoder interface 2*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure Timer for Encoder Interface 1*/		
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=0xFFFF;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

/* Configure Timer for Encoder Interface 2*/		
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=0xFFFF;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	/*Configure Encoder Interface 1 capture channel*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	/*Configure Encoder Interface 2 capture channel*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	/*Configure Encoder Interface 1*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	/*Configure Encoder Interface 2*/
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	/*Reset Counter 1*/
	TIM_SetCounter(TIM3, 0);
	/*Reset Counter 2*/
	TIM_SetCounter(TIM2, 0);
	/*Enable 1nd Timer */
	TIM_Cmd(TIM3,ENABLE);	
	/*Enable 2nd Timer */
	TIM_Cmd(TIM2,ENABLE);
	
	/*Clear Interrupt Update flag 1*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	/*Clear Interrupt Update flag 2*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	
	/*Enable Update Interrupt of 2ng TIMER*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/*Enable Update Interrupt of 2ng TIMER*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

/* 2 bit for pre-emption priority, 2 bits for subpriority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
				
	/*Configure NVIC for Encoder Interface Update Interrupt 1*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	/*Configure NVIC for Encoder Interface Update Interrupt 2*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

