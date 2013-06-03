/*
 * Timer.c
 *
 *  Created on: Jan 30, 2012
 *      Author: a0866685
 */

#include "Timer.h"

/**
 * timerdelay - timer used for haptics effects
 */
void timerdelay(unsigned int tdelay)
{
    TA0CCR0 = tdelay;
    TA0CTL = MC_1+TACLR+TASSEL_2;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
    __bis_SR_register(LPM0_bits+GIE);
    __no_operation();
}

/**
 * sleep - sleep for a fixed time
 * @param uint16_t time
 */
void sleep(uint16_t time)
{
    TA0CCR0 = time;
    TA0CTL = MC_1+TACLR+TASSEL_1;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
    __bis_SR_register(LPM0_bits+GIE);
    __no_operation();
}
