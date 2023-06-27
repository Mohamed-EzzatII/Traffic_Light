/******************************************************************************
 *
 * Module: external interrupt
 *
 * File Name: interrupt.c
 *
 * Description: source file for the AVR external interrupt driver dynamic configuration
 *
 * Author: Mohamed Ezzat
 *
 * Created on: March 21, 2022
 *
 * Version : v1.0.1
 *
 *******************************************************************************/

/************************Libraries and inclusions*******************************/

#include<avr/io.h>
#include<avr/interrupt.h>
#include"interrupt.h"
#include"common_macros.h"

/*******************************************************************************/

/*******************************************************************************
 *                              global Variables 	                           *
 *******************************************************************************/

static volatile void (*g_callBackPtr[3])(void)={NULL_PTR};

/*******************************************************************************
 *                           Functions definitions  	                       *
 *******************************************************************************/

/*
 * Description :
 * A function to initialize our external interrupt
 */
void INT_init(const INT_CONF* config)
{
	/* set the global interrupt enable */
	SET_BIT(SREG,7);

	/* enable the interrupts for our chosen module*/
	GICR |= config->intNumber;

	/* selecting the mode for interrupt */

	/*for interrupt zero , we control it with the 2 LSBs in MCUCR */
	if( (config->intNumber) == INT0_ID)
	{
		MCUCR|=(config->intMode);
	}

	/*for interrupt one , we control it with the 3rd and 4th bits in MCUCR */
	else if( (config->intNumber) == INT1_ID)
	{
		MCUCR |= ( (config->intMode) << 2);
	}

	/*for interrupt one , we control it with the 6th bit in MCUCR */
	else if( (config->intNumber) == INT2_ID)
	{
		/* 2 & 1 = 0 - > shift left by 6 and put in MCUCSR then 6th bit is 0 (falling edge)
		 * 3 & 1 = 1 - > shift left by 6 and put in MCUCSR then 6th bit is 1 (rising edge)
		 */
		MCUCSR |= ( (config->intMode) & 1 ) << 6;
	}
}

/*
 * Description :
 * A function to initialize our external interrupt
 */
void INT_deInit(const INT_CONF* config)
{

	/* disable the interrupts for our chosen module*/
	GICR&=(~(config->intNumber));
}


/*
 * Description :
 * A function to change the interrupt cause mode
 */
void INT_changeConfig(const INT_CONF* config)
{
	/* selecting the mode for interrupt */

	/*for interrupt zero , we control it with the 2 LSBs in MCUCR */
	if( (config->intNumber) == INT0_ID)
	{
		MCUCR|=(config->intMode);
	}

	/*for interrupt one , we control it with the 3rd and 4th bits in MCUCR */
	else if( (config->intNumber) == INT1_ID)
	{
		MCUCR |= ( (config->intMode) << 2);
	}

	/*for interrupt one , we control it with the 6th bit in MCUCR */
	else if( (config->intNumber) == INT2_ID)
	{
		/* 2 & 1 = 0 - > shift left by 6 and put in MCUCSR then 6th bit is 0 (falling edge)
		 * 3 & 1 = 1 - > shift left by 6 and put in MCUCSR then 6th bit is 1 (rising edge)
		 */
		MCUCSR |= ( (config->intMode) & 1 ) << 6;
	}
}



/*
 * Description :
 * A function to set ISR function code using call back technique
 */
void INT_setISR(void (*ptr)(void) , uint8 interrupt_number)
{
	if(interrupt_number < 3)
		g_callBackPtr[interrupt_number] = ptr;
}


/*******************************************************************************
 *                      Interrupt Service routine(ISR)  	                    *
 *******************************************************************************/


/*
 * Description :
 * ISR of INT0
 */
ISR(INT0_vect)
{
	if(g_callBackPtr[0] != NULL_PTR)
	{
		g_callBackPtr[0]();
	}

}

/*
 * Description :
 * ISR of INT1
 */
ISR(INT1_vect)
{
	if(g_callBackPtr[1] != NULL_PTR)
	{
		g_callBackPtr[1]();
	}
}

/*
 * Description :
 * ISR of INT2
 */
ISR(INT2_vect)
{
	if(g_callBackPtr[2] != NULL_PTR)
	{
		g_callBackPtr[2]();
	}
}
