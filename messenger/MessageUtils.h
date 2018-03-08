#pragma once

class PostOffice;

enum class FunctionType
{
	R0,		// register server
	R1,		// function type with only messageID
	R2,		// function type with messageID and Data payload
	R253,	// get valid messageID function type
	R254,	// send message function type
	R255	// register callback function type
};

// get a randomly generated valid messageID
int GetValidMessageID();

// send a message and optional data
void SendMessage(const int& MessageID, void* TheData = nullptr);

// register a message - each registered message must be unique
void RegisterMessage(const int &MessageID);

// register a function - more than one function can service the same message
void RegisterFunction(const int &MessageID, void* TheFunction, FunctionType FuncType);

// register the server to handle messages and callback functions
// !!! Must be called FIRST before any other function !!!
void RegisterServer(PostOffice* TheServer);