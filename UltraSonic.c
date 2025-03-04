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
#define SOUND_SPEED_CM_PER_US 0.0173  // Speed of sound in cm per µs
#define MIN_VALID_DISTANCE 2.0        // Minimum valid distance in cm
#define MAX_VALID_DISTANCE 400.0      // Maximum valid distance in cm
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
		IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		isRisingCaptured = 1;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	/* Capture falling edge */
	else
	{
		IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COUNTER(htim, 0);

		// Calculate the pulse width (time difference)
					IC_Difference = (IC_Value2 >= IC_Value1) ?
					                (IC_Value2 - IC_Value1) :
					                ((0xFFFF - IC_Value1) + IC_Value2);

		distance = IC_Difference * SOUND_SPEED_CM_PER_US;


		// Validate distance measurement
		if(distance >= MIN_VALID_DISTANCE && distance <= MAX_VALID_DISTANCE)
					{
						isReadingFinished = 1;
					}
					else
					{
						distance = 0;  // Invalid measurement
					}



		isRisingCaptured = 0;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
		__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC3);
	}

}
