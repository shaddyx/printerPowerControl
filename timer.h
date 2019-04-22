/*
 * timer.h
 *
 *  Created on: Feb 26, 2014
 *      Author: shaddy
 */

#ifndef TIMER_H_
#define TIMER_H_

int64_t setTimer(int64_t ms);
char checkTimer(int64_t* timer);
int64_t getTime();
void pollTimer();
void initTimer();

#endif /* TIMER_H_ */
