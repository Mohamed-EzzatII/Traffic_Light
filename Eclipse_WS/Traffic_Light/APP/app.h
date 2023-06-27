/******************************************************************************
 *
 * File Name: app.h
 *
 * Description: header file for the Application layer for traffic light project
 *
 * Author: Mohamed Ezzat
 *
 * Created on: April 30, 2022
 *
 * Version : v1.0.1
 *
 *******************************************************************************/

#ifndef APP_APP_H_
#define APP_APP_H_

/*******************************************************************************
 *                              	Libraries	 	                           *
 *******************************************************************************/
#include "../MCAL/std_types.h"
/*******************************************************************************
 *                              Definitions 	                               *
 *******************************************************************************/

#define TIMER1_COUNTS_IN_ONE_SECOND 3906
#define TIMER1_INITAI_VALUE 		0
#define LEDS_PORT_8LEDS				PORTB_ID
#define LEDS_PORT_4LEDS				PORTA_ID
#define RED_H_GREEN_V_TIME_SEC		0
#define RED_H_YELLOW_V_TIME_SEC		9
#define GREEN_H_RED_V_TIME_SEC		11
#define YELLOW_H_RED_V_TIME_SEC		19
#define END_LOOP_TIME_SEC			21

/*******************************************************************************
 *                              global Variables 	                           *
 *******************************************************************************/


/*******************************************************************************
 *                           Functions Prototype  	                       *
 *******************************************************************************/
/*
 * Description : -
 * A function to initialize timer , interrupt , LEDs
 */
void APP_init(void);

/*
 * Description : -
 * A function to be set as ISR of timer to increment the number of seconds
 */
void APP_inc_seconds(void);

/*
 * Description : -
 * A function to be set as ISR of INT0 to pass the horizontal street
 */
void APP_pass_horizontal(void);

/*
 * Description : -
 * A function to be set as ISR of INT1 to pass the vertical street
 */
void APP_pass_vertical(void);

/*
 * Description : -
 * A function to be set as ISR of INT1 to pass the vertical street
 */
void APP_traffic_light_control(void);


#endif /* APP_APP_H_ */
