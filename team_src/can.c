/*
 * can.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "all.h"

struct ECAN_REGS ECanaShadow;

/*
 * Function responsible for initializing the CAN module.
 *
 * For simple CAN setup the function CreateCANMailbox can be used to specify user
 * mailboxes.
 */
void CANSetup()
{
	//System specific CAN setup
	SystemCANInit(&ECanaShadow);

	EALLOW;
	//MBOX 0 - 1
	CommandBoxInit();   // Mbox 0
	HeartbeatBoxInit(); // Mbox 1
	/*
	* todo USER: Node specifc CAN setup
	* create mailbox for all Receive and transmit IDs
	* MBOX2 - MBOX31
    *
	* CreateCANMailbox(int mailboxNum, int IDE, int AME, int AAM, int DLC, int STDMSGID, int Mode)
	*/

	//adc TRANSMIT
	CreateCANMailbox(2,0,0,1,8,ADC_ID,0);
	//gp_button TRANSMIT
	CreateCANMailbox(3,0,0,1,8,GP_BUTTON_ID,0);

    EDIS;
    FinishCANInit();
}

char FillCAN(unsigned int Mbox)
{
	CopyMCToShadow(&ECanaShadow);

	//Try to fill heartbeat. If not heartbeat mailbox, see if it's a user mailbox
	if(FillHeartbeat(Mbox, user_ops.UserFlags.all) != 1)
	{
		//todo USER: setup for all transmit MBOXs
		//InsertCANMessage(int Mbox, unsigned int MDH, unsigned int MDL)
		switch (Mbox)
		{
		case ADC_BOX:
			InsertCANMessage(ADC_BOX, 0, user_data.adc);
			return 1;
		case GP_BUTTON_BOX:
			InsertCANMessage(GP_BUTTON_BOX, 0, user_data.gp_button);
			return 1;
		default:
			return 0;
		}
	}
	else
	{
		return 1;
	}
}

//todo SEAN: Determine if setting the ops flag can be in a system function. What if user removes that op?
//Another issue is if you put everything up until the stopWatchComplete into a system function, can_watch
//is no longer available
void SendCAN(unsigned int Mbox)
{
	CopyMCToShadow();
	CheckBusOff();
	CreateMask(Mbox);

	BeginTransmission();
	CheckForFlags();
}


void FillCANData()
{
	//todo USER: use FillCAN to put data into correct mailboxes
	FillCAN(ADC_BOX);
	FillCAN(GP_BUTTON_BOX);
}

// INT9.6
__interrupt void ECAN1INTA_ISR(void)  // eCAN-A
{
	Uint32 ops_id;
	Uint32 dummy;
  	unsigned int mailbox_nr;
  	mailbox_nr = getMailboxNR();
  	//todo USER: Setup ops command
  	if(mailbox_nr == COMMAND_BOX)
  	{
  		ReadCommand();
  	}
  	//todo USER: Setup other reads

  	//To receive more interrupts from this PIE group, acknowledge this interrupt
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// INT9.5
__interrupt void ECAN0INTA_ISR(void)   // eCAN-A
{

	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
