#include "stdafx.h"
#include "PostOffice.h"
#include <random>
#include <chrono>

// Maximum Message ID's that can be registered
const size_t MAX_ID_COUNT = 100;

PostOffice::PostOffice()
{
	MessageID AMessageID;
	AMessageID.Status = POStatus::NoMessage;
	_MessageIDList.push_back(AMessageID);
}


PostOffice::~PostOffice()
{
}

// if the Message ID exists POStatus::AlreadyRegistered is returned
// otherwise POStatus::NoID is returned if it doesn't exist
POStatus PostOffice::CheckID(const int &TheMessageID)
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

// TheMessageID can be any positive integer greater than 0.
// This function will verify no previous message ID already exists
// and register this ID if there is no previous ID registered.
// Return Values:
// POStatus::Registered -- If TheMessageID is registered successfully
// POStatus::AlreadyRegistered -- If TheMessageID is already registered
// POStatus::AtMaximumIDLimit -- If the maximum registered message IDs has been reached
// POStatus::NoID -- If TheMessageID is invalid (i.e. TheMessageID <= 0)
POStatus PostOffice::RegisterMessageID(const int &TheMessageID)
{
	if (TheMessageID <= 0)
	{
		return POStatus::NoID;
	}

	if (_MessageIDList.size() >= MAX_ID_COUNT)
	{
		return POStatus::AtMaximumIDLimit;
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
// POStatus::AtMaximumIDLimit -- if at the limit, i.e. no more registrations left
// POStatus::Success -- a ValidID is returned -- may be invalid by time of use
POStatus PostOffice::GetValidMessageID(int &ValidID)
{
	int NewID = 0;

	if (_MessageIDList.size() >= MAX_ID_COUNT)
	{
		return POStatus::AtMaximumIDLimit;
	}

	do {
		std::default_random_engine dre;
		std::chrono::high_resolution_clock::time_point TheTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds ATime = TheTime.time_since_epoch();
		dre.seed(ATime.count());
		std::uniform_int_distribution<int> distribution(10000, 11000);
		NewID = distribution(dre);

	} while (CheckID(NewID) == POStatus::AlreadyRegistered);

	ValidID = NewID;

	return POStatus::Success;
}
