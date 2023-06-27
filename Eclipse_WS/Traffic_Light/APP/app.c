/******************************************************************************
 *
 * File Name: app.c
 *
 * Description: source file for the Application layer for traffic light project
 *
 * Author: Mohamed Ezzat
 *
 * Created on: April 30, 2022
 *
 * Version : v1.0.1
 *
 *******************************************************************************/

/*******************************************************************************
 *                              	Libraries	 	                           *
 *******************************************************************************/

#include"../MCAL/timer1.h"
#include"../MCAL/interrupt.h"
#include"../MCAL/gpio.h"
#include"app.h"
#include<avr/io.h>

/*******************************************************************************
 *                              global Variables 	                           *
 *******************************************************************************/

uint8 g_seconds=0;

/*******************************************************************************
 *                           Functions definitions  	                       *
 *******************************************************************************/

/*
 * Description : -
 * A function to initialize timer , interrupt , LEDs
 */
void APP_init(void)
{

	SREG|=(1<<7);
	/* Initialize the timer with compare mode and prescaler 1024 so frequency of timer is
	 * 16000000/1024 = 15625 Hz , and set the compare value with 15625 so the timer
	 * will give us an interrupt each second , and set the ISR to increment the number of seconds
	 */
	Timer1_ConfigType config_ptr={TIMER1_INITAI_VALUE,TIMER1_COUNTS_IN_ONE_SECOND,N_1024,COMPARE};
	TIMER1_init(&config_ptr);
	TIMER1_setCallBack(APP_inc_seconds);

	/* Initialize the external interrupts INT1 & INT0 which is used in crossing the street*/
	INT_CONF config_interrupt_0={INT0_ID,FALLING_EDGE};
	INT_init(&config_interrupt_0);
	INT_CONF config_interrupt_1={INT1_ID,FALLING_EDGE};
	INT_init(&config_interrupt_1);
	INT_setISR(APP_pass_vertical,1);
	INT_setISR(APP_pass_horizontal,0);
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID,PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID,PIN_INPUT);

	/*LEDs Direction*/
	GPIO_setupPortDirection(LEDS_PORT_4LEDS, PORT_OUTPUT);
	GPIO_setupPortDirection(LEDS_PORT_8LEDS, PORT_OUTPUT);

}

/*
 * Description : -
 * A function to be set as ISR of timer to increment the number of seconds
 */
void APP_inc_seconds(void)
{
	g_seconds++;
}

/*
 * Description : -
 * A function to be set as ISR of INT0 to pass the horizontal street
 */
void APP_pass_horizontal(void)
{
	/* Make the horizontal light yellow then red when it is not red*/
	if(g_seconds>=(END_LOOP_TIME_SEC/2))
		g_seconds=YELLOW_H_RED_V_TIME_SEC;
}


/*
 * Description : -
 * A function to be set as ISR of INT1 to pass the vertical street
 */
void APP_pass_vertical(void)
{
	/* Make the vertical light yellow then red when it is not red*/
	if(g_seconds<(END_LOOP_TIME_SEC/2))
		g_seconds=RED_H_YELLOW_V_TIME_SEC;
}

/*
 * Description : -
 * A function to be set as ISR of INT1 to pass the vertical street
 */
void APP_traffic_light_control(void)
{
	/* Horizontal : red , Vertical : green*/
	if(g_seconds == RED_H_GREEN_V_TIME_SEC)
	{
		/* Horizontal 1 */
		GPIO_writePin(PORTB_ID,PIN0_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN1_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN2_ID, LOGIC_LOW);

		/* Horizontal 2 */
		GPIO_writePin(PORTB_ID,PIN3_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN4_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_LOW);

		/* Vertical 1 */
		GPIO_writePin(PORTB_ID,PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN7_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN0_ID, LOGIC_HIGH);

		/* Vertical 1 */
		GPIO_writePin(PORTA_ID,PIN1_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN2_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN3_ID, LOGIC_HIGH);
	}
	/* Horizontal : red , Vertical : yellow*/
	else if(g_seconds == RED_H_YELLOW_V_TIME_SEC)
	{
		/* Horizontal 1 */
		GPIO_writePin(PORTB_ID,PIN0_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN1_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN2_ID, LOGIC_LOW);

		/* Horizontal 2 */
		GPIO_writePin(PORTB_ID,PIN3_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN4_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_LOW);

		/* Vertical 1 */
		GPIO_writePin(PORTB_ID,PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN7_ID, LOGIC_HIGH);
		GPIO_writePin(PORTA_ID,PIN0_ID, LOGIC_LOW);

		/* Vertical 1 */
		GPIO_writePin(PORTA_ID,PIN1_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN2_ID, LOGIC_HIGH);
		GPIO_writePin(PORTA_ID,PIN3_ID, LOGIC_LOW);
	}

	/* Horizontal : green , Vertical : red*/
	else if(g_seconds == GREEN_H_RED_V_TIME_SEC)
	{
		/* Horizontal 1 */
		GPIO_writePin(PORTB_ID,PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN1_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN2_ID, LOGIC_HIGH);

		/* Horizontal 2 */
		GPIO_writePin(PORTB_ID,PIN3_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN4_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_HIGH);

		/* Vertical 1 */
		GPIO_writePin(PORTB_ID,PIN6_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN7_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN0_ID, LOGIC_LOW);

		/* Vertical 1 */
		GPIO_writePin(PORTA_ID,PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(PORTA_ID,PIN2_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN3_ID, LOGIC_LOW);
	}

	/* Horizontal : yellow , Vertical : red*/
	else if(g_seconds == YELLOW_H_RED_V_TIME_SEC)
	{
		/* Horizontal 1 */
		GPIO_writePin(PORTB_ID,PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN2_ID, LOGIC_LOW);

		/* Horizontal 2 */
		GPIO_writePin(PORTB_ID,PIN3_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN4_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_LOW);

		/* Vertical 1 */
		GPIO_writePin(PORTB_ID,PIN6_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN7_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN0_ID, LOGIC_LOW);

		/* Vertical 2 */
		GPIO_writePin(PORTA_ID,PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(PORTA_ID,PIN2_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN3_ID, LOGIC_LOW);

	}

	/* Horizontal : red , Vertical : red*/
	else if(g_seconds >= END_LOOP_TIME_SEC)
	{
		/*count back*/
		g_seconds=0;
	}

}
