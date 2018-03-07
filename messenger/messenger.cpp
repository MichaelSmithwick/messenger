// messenger.cpp : Defines the entry point for the console application.
//

//
// The purpose of this project is to illustrate how a message can be registered and then sent at
// anytime to specified destinations or broadcast to all destinations. The destinations are registered
// by the entities wishing to receive the informaion to be sent to them individually or to them as
// part of a broadcast.
//
// The project consists of three logical entities: (1) The sender; (2) The reciever; and (3) The postoffice.
// The postoffice is the intermediary that enables the sender and the receiver to be uncoupled from each other.
//
// The sender will register a message it wishes to send with the postoffice.
// The receiver will register as desirous of receiving the message and provide a callback function address
// to be used when a message is available to the postoffice.
//
// The three entities will be represented by the Sender, Reciever, and PostOffice classes.
//

#include "stdafx.h"
#include <iostream>
#include "PostOffice.h"


int main()
{
	PostOffice MailMan;
	if (MailMan.RegisterMessageID(1) == POStatus::Registered)
	{
		std::cout << "Success ID 1 Registered" << std::endl;
	}
	else
	{
		std::cout << "Failed, ID 1 could not be Registered" << std::endl;
	}

	if (MailMan.RegisterMessageID(1) == POStatus::Registered)
	{
		std::cout << "Success ID 1 Registered" << std::endl;
	}
	else
	{
		std::cout << "Failed, ID 1 could not be Registered" << std::endl;
	}

	int AnID;

	for (int i = 0; i < 10; i++)
	{
		MailMan.GetValidMessageID(AnID);
		std::cout << AnID << std::endl;
	}

	return 0;
}
