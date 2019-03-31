#include "stm32f10x.h"

GPIO_InitTypeDef 					GPIO_InitStructure;
void GPIO_Configuration(void);
void Delay_ms(uint16_t time);
void TIM4_Configuration(void);
int main(void)
{
	GPIO_Configuration();
	TIM4_Configuration();
  while (1)
  {
		
		
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
void TIM4_Configuration(void)
{	
		/*Configure using library*/
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
		NVIC_InitTypeDef NVIC_InitStructure;
		/*Enable TIM4 clock*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		/*TIM4 Configuration at 2Hz*/
		TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;
		TIM_TimeBaseInitStructure.TIM_Period = 9999;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);  // clear update flag
		TIM_ITConfig( TIM4, TIM_IT_Update, ENABLE); // enable Update Interrupt
		TIM_Cmd(TIM4, ENABLE); //enable timer 4
		/*Configure Interrupt request for TIM4*/
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}


