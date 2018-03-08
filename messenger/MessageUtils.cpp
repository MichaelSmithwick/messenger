#include "stdafx.h"
#include "MessageUtils.h"
#include "PostOffice.h"

// TODO Add error handling and a method for the user to know the last error

// returns a valid messageID
// however the messageID is not garaunteed to be valid at time of use
int GetValidMessageID()
{
	int ID = 0;
	RegisterFunction(0, (void*)&ID, FunctionType::R253);
	return ID;
}

// send the message to the server so it can callback the correct functions
void SendMessage(const int & MessageID, void * TheData)
{
	RegisterFunction(MessageID, TheData, FunctionType::R254);
}

// register a message with the server
void RegisterMessage(const int & MessageID)
{
	RegisterFunction(MessageID, nullptr, FunctionType::R255);
}

// This function does a lot more than just Register Functions
// This function contains a pointer to the PostOffice object that will handle all the
// messaging and function calling.
//
// ===> The RegisterServer() function must be called before this function is used <===
//
// If a server is not registered this function will do nothing
void RegisterFunction(const int &MessageID, void * TheFunction, FunctionType FuncType)
{
	static PostOffice* PO = nullptr;

	if (FuncType == FunctionType::R0 && TheFunction != nullptr)
	{
		PO = (PostOffice*)TheFunction;
	}

	if (FuncType == FunctionType::R1 && PO != nullptr)
	{
		PO->RegisterToReceiveMessage(MessageID, (RegisteredMessageProto1)TheFunction);
	}

	if (FuncType == FunctionType::R2 && PO != nullptr)
	{
		PO->RegisterToReceiveMessage(MessageID, (RegisteredMessageProto2)TheFunction);
	}

	if (FuncType == FunctionType::R255 && PO != nullptr)
	{
		PO->RegisterMessageID(MessageID);
	}

	if (FuncType == FunctionType::R254 && PO != nullptr)
	{
		// 'TheFunction' is actually a pointer to the data package
		// for the R2 type function and in this 
		// case it can be a pointer to anything.
		// If the pointer is null FunctionType::R1 will be called
		// otherwise FuctionType::R2 will be called
		if (TheFunction == nullptr)
		{
			PO->SendMessage(MessageID);
		}
		else
		{
			PO->SendMessage(MessageID, TheFunction);
		}
	}

	if (FuncType == FunctionType::R253 && PO != nullptr)
	{
		if (TheFunction != nullptr)
		{
			PO->GetValidMessageID(*((int*)TheFunction));
		}
	}
}

// register the server to handle the messages and callbacks
// 
// ===> This function MUST be called before any other functions in this file <===
//
void RegisterServer(PostOffice * TheServer)
{
	RegisterFunction(0, (void*)TheServer, FunctionType::R0);
}
