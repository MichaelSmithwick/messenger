#pragma once
#include <list>

// enumeration to hold various operation statuses
enum class POStatus
{
	NoMessage = -1,		// used to indicate no message ID in the MessageID array
	AlreadyRegistered,	// MessageID already exists
	Success,			// operation was successful
	Registered,			// new MessageID registered successfully
	AtMaximumIDLimit,	// no more IDs can be accepted because limit has been reached
	NoID				// the specified ID does not exist
};

// the union will use storage for only one int but give the ability to 
// have an enumerated status as well as an int defined ID.
// The int defined ID should always be greater than 0 but actually can be
// any number other than -1
union MessageID
{
	POStatus Status;
	int AMessageID;
};

class PostOffice
{
public:
	PostOffice();
	~PostOffice();


	// any value greater than 0 may be registered.
	// if value already exists POStatus::AlreadyRegistered will be returned
	// otherwise POStatus::Registered will be returned
	POStatus RegisterMessageID(const int &MessageID);

	// will return a valid Message ID but does not guarantee it will remain
	// valid for use. Always check the return value of RegisterMessageID()
	// to ensure the Message ID is registered successfully.
	POStatus GetValidMessageID(int &ValidID);

private:
	// array to hold registered Message IDs - start with 10 and expand later if necessary
	std::list<MessageID>_MessageIDList;

	// check existence of MessageID and return POStatus::AlreadyRegistered if it exists
	// or POStatus::NoID if it does not exist
	POStatus CheckID(const int &TheMessageID);
};

