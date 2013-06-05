/*
 * Timer.h
 *
 *  Created on: Jan 30, 2012
 *      Author: a0866685
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "CTS/structure.h"

/**
 * timerdelay - 5ms timer delay
 * @param unsigned int delay
 */
void timerdelay(unsigned int tdelay);

/**
 * sleep - sleep for a fixed time
 * @param uint16_t time
 */
void sleep(uint16_t time);

#endif /* TIMER_H_ */

