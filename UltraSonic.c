/*
 **************** @file : UltraSonic.c ********************************************
 **************** @Author: Ahmed Fathy ********************************************
 **************** @brief : Interface UltraSonic Driver *****************************
 */
#include "../Inc/UltraSonic.h"
extern TIM_HandleTypeDef  htim2;
extern uint16_t distance;
static volatile uint8_t isRisingCaptured = 0;
static volatile uint32_t IC_Value1 = 0;
static volatile uint32_t IC_Value2 = 0;
static volatile uint32_t IC_Difference = 0;
static void Delay_10US(void){

	__HAL_TIM_SET_COUNTER(ULTRASONIC_ECHO_PIN_IC,0);

	while(__HAL_TIM_GET_COUNTER(ULTRASONIC_ECHO_PIN_IC) < 5);

}

void Ultrasonic_Get_Distance(void)
{
	/* Send Trigger Signal to the Ultrasonic Trigger Pin */
	HAL_GPIO_WritePin(ULTRASONUC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_PIN_SET);
	Delay_10US();
	HAL_GPIO_WritePin(ULTRASONUC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_PIN_RESET);

	__HAL_TIM_ENABLE_IT(&htim2 , TIM_IT_CC1);
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	/* Capture rising edge */
	if(0 == isRisingCaptured)
	{
		IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		isRisingCaptured = 1;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	/* Capture falling edge */
	else if(1 == isRisingCaptured)
	{
		IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COUNTER(htim, 0);

		if(IC_Value2 > IC_Value1)
		{
			IC_Difference = IC_Value2 - IC_Value1;
		}
		else if(IC_Value1 > IC_Value2)
		{
			IC_Difference = (0xFFFF - IC_Value1) + IC_Value2;
		}

		distance = IC_Difference * 0.0173;

		isRisingCaptured = 0;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
	}
	else{ /* Nothing */ }
}
