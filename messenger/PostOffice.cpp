#include "stdafx.h"
#include "PostOffice.h"
#include <random>
#include <chrono>

// Maximum Message ID's that can be registered
const size_t MAX_ID_COUNT = 100;

// constructor
PostOffice::PostOffice()
{
	// dummy message for testing
	MessageID AMessageID;
	AMessageID.Status = POStatus::NoMessage;
	_MessageIDList.push_back(AMessageID);
}


// destructor
PostOffice::~PostOffice()
{
}

// if the Message ID exists POStatus::AlreadyRegistered is returned
// otherwise POStatus::NoID is returned if it doesn't exist
POStatus PostOffice::CheckID(const int &TheMessageID) const
{
	for (MessageID Msg : _MessageIDList)
	{
		if (Msg.AMessageID == TheMessageID)
		{
			return POStatus::AlreadyRegistered;
		}
	}
	return POStatus::NoID;
}

// The size of _RegisteredCallbackArray
int PostOffice::SizeCallBackArray()
{
	return _CallbackIndex+1;
}

// add the callback function type R1 to the array
// no validation is done except array bounds validation - caller beware
void PostOffice::AddCallBack(RegisteredMessageProto1 CallbackFunction)
{
	if ((_CallbackIndex >= -1) && (_CallbackIndex < (int)MAX_FUNCTION_COUNT))
	{
		// _CallbackIndex must be incremented before use
		_CallbackIndex++;
		_RegisteredCallbackList[_CallbackIndex].Func.R1 = CallbackFunction;
		_RegisteredCallbackList[_CallbackIndex].Type = FunctionType::R1;
	}
}

// add the callback function type R2 to the array
// no validation is done except array bounds validation - caller beware
void PostOffice::AddCallBack(RegisteredMessageProto2 CallbackFunction)
{
	if ((_CallbackIndex >= -1) && (_CallbackIndex < (int)MAX_FUNCTION_COUNT))
	{
		// _CallbackIndex must be incremented before use
		_CallbackIndex++;
		_RegisteredCallbackList[_CallbackIndex].Func.R2 = CallbackFunction;
		_RegisteredCallbackList[_CallbackIndex].Type = FunctionType::R2;
	}
}

// TheMessageID can be any positive integer greater than 0.
// This function will verify no previous message ID already exists
// and register this ID if there is no previous ID registered.
// Return Values:
// POStatus::Registered -- If TheMessageID is registered successfully
// POStatus::AlreadyRegistered -- If TheMessageID is already registered
// POStatus::AtMaximumLimit -- If the maximum registered message IDs has been reached
// POStatus::NoID -- If TheMessageID is invalid (i.e. TheMessageID <= 0)
POStatus PostOffice::RegisterMessageID(const int &TheMessageID)
{
	if (TheMessageID <= 0)
	{
		return POStatus::NoID;
	}

	if (_MessageIDList.size() >= MAX_ID_COUNT)
	{
		return POStatus::AtMaximumLimit;
	}

	if (CheckID(TheMessageID) == POStatus::AlreadyRegistered)
	{
		return POStatus::AlreadyRegistered;
	}

	MessageID NewMessage;
	NewMessage.AMessageID = TheMessageID;
	_MessageIDList.push_back(NewMessage);

	return POStatus::Registered;
}

// return a random integer between 10,000 and 11,000
// that has not been registered yet
// result status is:
// POStatus::AtMaximumLimit -- if at the limit, i.e. no more registrations left
// POStatus::Success -- a ValidID is returned -- may be invalid by time of use --
POStatus PostOffice::GetValidMessageID(int &ValidID) const
{
	int NewID = 0;

	if (_MessageIDList.size() >= MAX_ID_COUNT)
	{
		return POStatus::AtMaximumLimit;
	}

	do {
		std::default_random_engine dre;
		std::chrono::high_resolution_clock::time_point TheTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds ATime = TheTime.time_since_epoch();
		dre.seed((unsigned int)ATime.count());
		std::uniform_int_distribution<int> distribution(10000, 11000);
		NewID = distribution(dre);

	} while (CheckID(NewID) == POStatus::AlreadyRegistered);

	ValidID = NewID;

	return POStatus::Success;
}

// Register a callback function (style R1) to handle when a message of MessageID is sent
// @MessageID is the ID of the message to service
// @Function is the callback function style R1 to handle the message
// Returned values:
// POStatus::AtMaximumLimit - No more callback functions can be registered, the limit has been reached
// POStatus::Registered - The callback function can be registered
POStatus PostOffice::RegisterToReceiveMessage(const int & MessageID, RegisteredMessageProto1 Function)
{
	int ArrayCounter = 0;

	// make sure the function is not already in the list
	for (RegisteredFunctions a : _RegisteredCallbackList)
	{
		if (a.Func.R1 == Function)
		{
			// being in the array already is not a problem, just don't put the function in the array later
			// return POStatus::AlreadyRegistered;
			// ArrayCounter has the index of this function in the array
			break;
		}

		ArrayCounter++;
	}

	// make sure _RegisteredCallbackList is not full
	if (SizeCallBackArray() >= MAX_FUNCTION_COUNT)
	{
		return POStatus::AtMaximumLimit;
	}

	// add function to callback list (if not found earlier) and adjust ArrayCounter
	if (SizeCallBackArray() <= ArrayCounter)
	{
		AddCallBack(Function);
		ArrayCounter = SizeCallBackArray() - 1;
	}

	// does message currently exist in the Message-Callback function list?
	bool Found = false;
	std::list<MessageFunctions>::iterator FoundMessage;
	for (FoundMessage = _MessageFunctionList.begin(); FoundMessage != _MessageFunctionList.end(); ++FoundMessage)
	{
		if (FoundMessage->TheMessageID.AMessageID == MessageID)
		{
			Found = true;
			break;
		}
	}

	// add index of function to MessageID-Function association list
	if (Found)
	{
		// add index of function to existing MessageID-Function list
		FoundMessage->FunctionIndexes.push_back(ArrayCounter);
	}
	else
	{
		// add to new MessageID-Function association to list
		MessageFunctions Package;
		Package.TheMessageID.AMessageID = MessageID;
		Package.FunctionIndexes.push_back(ArrayCounter);
		_MessageFunctionList.push_back(Package);
	}

	return POStatus::Registered;
}

// Register a callback function (style R2) to handle when a message of MessageID is sent
// @MessageID is the ID of the message to service
// @Function is the callback function style R2 to handle the message
// Returned values:
// POStatus::AtMaximumLimit - No more callback functions can be registered, the limit has been reached
// POStatus::Registered - The callback function can be registered
POStatus PostOffice::RegisterToReceiveMessage(const int & MessageID, RegisteredMessageProto2 Function)
{
	int ArrayCounter = 0;

	// make sure the function is not already in the list
	for (RegisteredFunctions a : _RegisteredCallbackList)
	{
		if (a.Func.R2 == Function)
		{
			// being in the array already is not a problem, just don't put the function in the array later
			// return POStatus::AlreadyRegistered;
			// ArrayCounter has the index of this function in the array
			break;
		}

		ArrayCounter++;
	}

	// make sure _RegisteredCallbackList is not full
	if (SizeCallBackArray() >= MAX_FUNCTION_COUNT)
	{
		return POStatus::AtMaximumLimit;
	}

	// add function to callback list (if not found earlier) and adjust ArrayCounter
	if (SizeCallBackArray() <= ArrayCounter)
	{
		AddCallBack(Function);
		ArrayCounter = SizeCallBackArray() - 1;
	}

	// does message currently exist in the Message-Callback function list?
	bool Found = false;
	std::list<MessageFunctions>::iterator FoundMessage;
	for (FoundMessage = _MessageFunctionList.begin(); FoundMessage != _MessageFunctionList.end(); ++FoundMessage)
	{
		if (FoundMessage->TheMessageID.AMessageID == MessageID)
		{
			Found = true;
			break;
		}
	}

	// add index of function to MessageID-Function association list
	if (Found)
	{
		// add index of function to existing MessageID-Function list
		FoundMessage->FunctionIndexes.push_back(ArrayCounter);
	}
	else
	{
		// add to new MessageID-Function association list
		MessageFunctions Package;
		Package.TheMessageID.AMessageID = MessageID;
		Package.FunctionIndexes.push_back(ArrayCounter);
		_MessageFunctionList.push_back(Package);
	}

	return POStatus::Registered;
}

// run all the callback functions associated with MessageID
// for R1 style functions
POStatus PostOffice::SendMessage(const int & MessageID)
{
	// The MessageID doesn't have to be in the Registered Message list
	// so it's validity is not checked here

	// Find MessageID in the Message-Function association list
	bool bFound = false; // for status reporting later
	for (MessageFunctions a : _MessageFunctionList)
	{
		if (a.TheMessageID.AMessageID == MessageID)
		{
			bFound = true;

			// Execute all callback functions registered with this message
			for (int Index : a.FunctionIndexes)
			{
				(_RegisteredCallbackList[Index].Func.R1)(MessageID);
			}
		}
	}

	return bFound ? POStatus::Success : POStatus::NoMessage;
}

// run all the callback functions associated with MessageID
// for R2 style functions
POStatus PostOffice::SendMessage(const int & MessageID, void* PointerToUnknownDataType)
{
	// The MessageID doesn't have to be in the Registered Message list
	// so it's validity is not checked here

	// Find MessageID in the Message-Function association list
	bool bFound = false; // for status reporting later
	for (MessageFunctions a : _MessageFunctionList)
	{
		if (a.TheMessageID.AMessageID == MessageID)
		{
			bFound = true;

			// Execute all callback functions registered with this message
			for (int Index : a.FunctionIndexes)
			{
				(_RegisteredCallbackList[Index].Func.R2)(MessageID, PointerToUnknownDataType);
			}
		}
	}

	return bFound ? POStatus::Success : POStatus::NoMessage;
}

