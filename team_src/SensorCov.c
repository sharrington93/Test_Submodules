/*
 * SensorCov().c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"


user_ops_struct ops_temp;
user_data_struct data_temp;

void SensorCov()
{
	SensorCovInit();
	while (sys_ops.State == STATE_SENSOR_COV)
	{
		LatchStruct();
		SensorCovMeasure();
		UpdateStruct();
		FillCANData();
	}
	SensorCovDeInit();
}

void SensorCovInit()
{
	//todo USER: SensorCovInit()
	SystemSensorInit(4);
}


void SensorCovMeasure()
{
	SensorCovSystemInit();

	readADC();
	//todo USER: Sensor Conversion
	//update data_temp and ops_temp
	//use stopwatch to catch timeouts
	//waiting should poll isStopWatchComplete() to catch timeout and throw StopWatchError
	data_temp.adc = A0RESULT;
	data_temp.gp_button = READGPBUTTON();

	if (data_temp.gp_button == 0) 			//if pushed cause stopwatch
	{
		SETLED0();
		int i = 0;
		while (i < 100)
		{
			i++;
		}
	}
	else
	{
		CLEARLED0();
	}
	if (data_temp.adc > 2000)
	{
		SETLED1();
	}
	else
	{
		CLEARLED1();
	}
	PerformSystemChecks();
}

void LatchStruct()
{
	LatchSystemStruct();
	ops_temp = user_ops;
	data_temp = user_data;
}

void UpdateStruct()
{
	SaveOpStates();
	user_ops = ops_temp;
	//todo USER: UpdateStruct
	//update with node specific op changes

	//if ops is not changed outside of sensor conversion copy temp over, otherwise don't change

	//Change bit is only set by ops changes outside of SensorCov.


	DetermineOpStates();
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
	SystemSensorDeInit();
}
