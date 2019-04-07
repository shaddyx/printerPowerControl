/*
 * buttons.h
 *
 *  Created on: Apr 7, 2019
 *      Author: shaddy
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

int homePosition();
int onButtonPressed();
int offButtonPressed();
void pollButtons();

unsigned long getHomeHold();
unsigned long getonHold();
unsigned long getOffHold();
unsigned long getOutOfHomeHold();


#endif /* BUTTONS_H_ */
