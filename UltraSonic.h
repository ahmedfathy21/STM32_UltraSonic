/*
 **************** @file : UltraSonic.h ********************************************
 **************** @Author: Ahmed Fathy ********************************************
 **************** @brief : Interface UltraSonic Driver *****************************
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "stm32f4xx_hal.h"

#define ULTRASONUC_TRIGGER_PORT			GPIOA
#define ULTRASONIC_TRIGGER_PIN  		GPIO_PIN_1
#define ULTRASONIC_ECHO_PIN_IC  		&htim2          // GPIO_PIN_0


void Ultrasonic_Get_Distance(void);

#endif /* INC_ULTRASONIC_H_ */
