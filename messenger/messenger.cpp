// messenger.cpp : Defines the entry point for the console application.
//

//
// The purpose of this project is to illustrate how a message can be registered and then sent at
// anytime to specified destinations or broadcast to all destinations. The destinations are registered
// by the entities wishing to receive the informaion to be sent to them individually or to them as
// part of a broadcast.
//
// The project consists of three logical entities: (1) The sender; (2) The reciever; and (3) The postoffice.
// The postoffice is the intermediary that allows the sender and the receiver to be uncoupled from each other.
//
// The sender will register a message it wishes to send with the postoffice.
// The receiver will register as desirous of receiving the message and provide a callback function address
// to be used when a message is available to the postoffice.
//
// The three entities will be represented by the Sender, Reciever, and PostOffice classes.
//
// In addition to the 3 classes messages and callback functions can be registered as normal non-class entities also
// and examples are given
//

#include "stdafx.h"
#include <iostream>
#include "PostOffice.h"
#include "Sender.h"
#include "Receiver.h"

// some enumerated messages
enum class Messages
{
	ShowTime,
	SayHello,
	SayGoodbye,
	RingABell,
	Exit
};

// callback for Doorbell message
// (note a class function also handles this with a different result)
// both this function and the other function are called when the message is sent
void DingDong(const int &MessageID);

// a callback function associated with a randomly generated messageID
void MyCallBack(const int &MessageID);

// process the users menu item choice by sending the appropriate message
void ProcessChoice(int TheChoice);

// a function that registers some messages
void RegisterMessages();

// a function that registers Receiver class static functions and the DingDong() function above
void RegisterCallbackFunctions(Receiver &Destination);

int main()
{
	PostOffice MailMan;		// server
	Sender Source;			// sender (could also be a receiver if desired)
	Receiver Destination;	// receiver (could also be a sender if desired)

	// server must be registered!!
	RegisterServer(&MailMan);

	RegisterMessages();

	RegisterCallbackFunctions(Destination);

	std::cout << "\nBefore the registration of CountOnMe callback function the Destination Counter is : " << Destination.GetCounter() << std::endl;

	// Make the 'Destination object' count...
	// get a valid messageID
	int CountingID = GetValidMessageID();
	// register the messageID
	RegisterMessage(CountingID);
	// A non-class callback function is registered for CountingID message servicing
	RegisterFunction(CountingID, (void*)CountOnMe, FunctionType::R2);

	// message sent to Destination object where the previously registered CountOnMe function will be called
	SendMessage(CountingID, (void*)&Destination); // Count 1...
	SendMessage(CountingID, (void*)&Destination); // Count 2...
	SendMessage(CountingID, (void*)&Destination); // Count 3...
	SendMessage(CountingID, (void*)&Destination); // Count 4...
	SendMessage(CountingID, (void*)&Destination); // Count 5...
	SendMessage(CountingID, (void*)&Destination); // Count 6...

	std::cout << "\nAfter the CountingID message is sent 6 times The Destination Counter is : " << Destination.GetCounter() << "\n" << std::endl;

	// show a menu of messages that can be sent and let the user choose what to do
	int TheChoice;
	do {
		Source.MessageChoices();
		TheChoice = Source.GetChoice();
		ProcessChoice(TheChoice);
	} while (TheChoice != 'Q' && TheChoice != 'q');

	// -------------------------------------------------------------
	// while classes interacted above, below this line
	// only regular functions are used. The PostOffice class
	// does not care what entity registers to send messages
	// nor what entities register to service the messages.

	std::cout << "\nGenerating random valid messageIDs" << std::endl;

	// generate a bunch of random but valid message IDs
	int AnID;
	for (int i = 0; i < 10; i++)
	{
		AnID = GetValidMessageID();
		std::cout << AnID << std::endl;
	}

	std::cout << "\nUsing one of the random messageIDs to callback the MyCallBack() non-class function" << std::endl;
	// register a message and a function to service the message
	// then send the message -- the result : the function is called
	RegisterMessage(AnID);
	RegisterFunction(AnID, MyCallBack, FunctionType::R1);
	SendMessage(AnID);

	std::cout << "\nSending ShowTime message" << std::endl;
	// send a message to an unsuspecting object (it will be serviced nonetheless)
	SendMessage((const int)Messages::ShowTime);

	std::cout << "\nSending RingABell message\nNote that two callback functions actually service this message" << std::endl;
	// ring the bell again...
	SendMessage((const int)Messages::RingABell);

	return 0;
}

// register callback functions with the server and associate them with the appropriated messages
// when the messages are sent the callback functions will be called.
void RegisterCallbackFunctions(Receiver &Destination)
{
	RegisterFunction((const int)Messages::ShowTime, Destination.ShowTime, FunctionType::R1);
	RegisterFunction((const int)Messages::SayHello, Destination.SayHello, FunctionType::R1);
	RegisterFunction((const int)Messages::SayGoodbye, Destination.SayGoodbye, FunctionType::R1);
	RegisterFunction((const int)Messages::RingABell, Destination.RingABell, FunctionType::R1);
	RegisterFunction((const int)Messages::RingABell, DingDong, FunctionType::R1); // additional non-sequiter function
	RegisterFunction((const int)Messages::Exit, Destination.Exit, FunctionType::R1);
}

// register some messages with server.
void RegisterMessages()
{
	RegisterMessage((const int)Messages::ShowTime);
	RegisterMessage((const int)Messages::SayHello);
	RegisterMessage((const int)Messages::SayGoodbye);
	RegisterMessage((const int)Messages::RingABell);
	RegisterMessage((const int)Messages::Exit);
}

// send the message that matches the users choice.
void ProcessChoice(int TheChoice)
{
	switch (TheChoice)
	{
	case 'A':
	case 'a':
		SendMessage((const int)Messages::ShowTime);
		break;
	case 'B':
	case 'b':
		SendMessage((const int)Messages::SayHello);
		break;
	case 'C':
	case 'c':
		SendMessage((const int)Messages::SayGoodbye);
		break;
	case 'D':
	case 'd':
		SendMessage((const int)Messages::RingABell);
		break;
	case 'X':
	case 'x':
		SendMessage((const int)Messages::Exit);
		break;
	default:
		break;
	};

}

// a standalone callback function that was register to respond to
// a random message ID generated by the PostOffice server
void MyCallBack(const int &MessageID)
{
	std::cout << MessageID << std::endl;
	std::cout << "\nMessageID : " << MessageID << "\nCongratulations, you found me!\n" << std::endl;
}

// a callback to be added to the Messages::RingABell message to show how 
// multiple callbacks can be made by one messages
void DingDong(const int &MessageID)
{
	std::cout << "\n\n Ding dong, Ding dong, Ding dong, ...\n" << std::endl;
}