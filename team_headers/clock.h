/*
 * clock.h
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void ClockSetup();

//todo USER: define clock
#define CLOCK_PERIOD 500000 //uS //so half second

#define DATAOUT_TICKS		1	//2hz

//todo USER: CLOCK_TICKS_CLEAR should have the same number of zeros as clock_struct has elements (as seen below)
//#define CLOCK_TICKS_CLEAR {0,0}
#define CLOCK_TICKS_CLEAR	{0}

typedef struct CLOCK_TICKS
{
	int DataOut;
}clock_struct;


#endif /* CLOCK_H_ */
