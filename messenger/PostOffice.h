#pragma once
#include <list>
#include "MessageUtils.h"

// Maximum callback functions that can be registered
const size_t MAX_FUNCTION_COUNT = 200;

// all registered callback functions must have one of these forms
// one sends a message only without any additional data -- it is assumed the receiver will know what to do
// the other one sends a message with data -- it assumed the receiver will know what to do with the data
typedef void (*RegisteredMessageProto1)(const int &MessageID);
typedef void(*RegisteredMessageProto2)(const int & MessageID, void* PointerToUnknownDataType);

// union to enable same array to handle multiple function types. (all pointers are the same size)
typedef union UnionCallbackFunction
{
	RegisteredMessageProto2 R2;
	RegisteredMessageProto1 R1;
}CallbackFunction;

// struct to store both callback function and function type
// (the function type is needed to determine what function prototype to use when the message is sent)
typedef struct StructRegisteredFunctions
{
	CallbackFunction Func;
	FunctionType Type;
}RegisteredFunctions;

// enumeration to hold various operation statuses
enum class POStatus
{
	NoMessage = -1,		// used to indicate no message ID in the MessageID array
	AlreadyRegistered,	// MessageID already exists
	Success,			// operation was successful
	Registered,			// new MessageID registered successfully
	AtMaximumLimit,	// no more IDs can be accepted because limit has been reached
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

// struct used to associate all function indexes with the messageID that will invoke them
// this is a one messageID to many service functions scenario
typedef struct StructMessageFunctions
{
	MessageID TheMessageID;			// the message ID to be serviced by functions in the list
	std::list<int>FunctionIndexes;	// list with indexes to functions in registered functions list
}MessageFunctions;

// The message server class
class PostOffice
{
public:
	PostOffice();
	~PostOffice();


	// register messageID
	// any value greater than 0 may be registered.
	// if value already exists POStatus::AlreadyRegistered will be returned
	// otherwise POStatus::Registered will be returned
	POStatus RegisterMessageID(const int &MessageID);

	// will return a valid Message ID but does not guarantee it will remain
	// valid for use. Always check the return value of RegisterMessageID()
	// to ensure the Message ID is registered successfully.
	POStatus GetValidMessageID(int &ValidID) const;

	// the two types of functions can be registered to respond to the specified messageID
	POStatus RegisterToReceiveMessage(const int &MessageID, RegisteredMessageProto1 Function);
	POStatus RegisterToReceiveMessage(const int &MessageID, RegisteredMessageProto2 Function);

	// two types of messages can be sent, one with additional data and one without any data
	POStatus SendMessage(const int &MessageID);
	POStatus SendMessage(const int &MessageID, void* PointerToUnknownDataType);

	// this function removes the need to use an instantiated server class (probably doesn't need to be a friend, but will leave it for now...)
	friend void RegisterFunction(const int &MessageID, void* TheFunction, FunctionType FuncType);

private:
	// list to hold registered Message IDs - start with 10 and expand later if necessary
	std::list<MessageID>_MessageIDList;

	// index and array to hold registered callback functions
	int _CallbackIndex = -1;
	RegisteredFunctions _RegisteredCallbackList[MAX_FUNCTION_COUNT];

	// list to hold message - function associations
	std::list<MessageFunctions>_MessageFunctionList;

	// check existence of MessageID and return POStatus::AlreadyRegistered if it exists
	// or POStatus::NoID if it does not exist
	POStatus CheckID(const int &TheMessageID) const;

	// get number of elements in callback function array
	int SizeCallBackArray();

	// add a callback function to the callback function array
	void AddCallBack(RegisteredMessageProto1 CallbackFunction);
	void AddCallBack(RegisteredMessageProto2 CallbackFunction);
};


