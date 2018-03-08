#include "stdafx.h"
#include "Receiver.h"
#include <chrono>
#include <iostream>
#include <string>


Receiver::Receiver():_Counter(0)
{
}


Receiver::~Receiver()
{
}

// system time (kind of...) callback function
void Receiver::ShowTime(const int & MessageID)
{
	std::chrono::system_clock::time_point TheTime = std::chrono::system_clock::now();
	std::chrono::system_clock::duration Duration = TheTime.time_since_epoch();


	std::cout << "\nSystem Time: " << Duration.count() << std::endl;
}

// Hello callback function
void Receiver::SayHello(const int & MessageID)
{
	std::cout << "\nHello everyone!" << std::endl;
}

// Goodbye callback function
void Receiver::SayGoodbye(const int & MessageID)
{
	std::cout << "\nGoodbye everyone!" << std::endl;
}

// Ring a bell callback (note in the example program a different callback function adds "Ding dong..." when the message is sent)
void Receiver::RingABell(const int & MessageID)
{
	std::cout << "\nI'm ringing a bell..." << std::endl;
}

// Exit callback function
void Receiver::Exit(const int & MessageID)
{
	std::cout << "\nExiting" << std::endl;
	exit(0);
}

// getter for _Counter
int Receiver::GetCounter()
{
	return _Counter;
}

// class is passed in as a void pointer and then cast to
// a Receiver class.
// since this function is a friend of the class it can
// increment the private class member _Counter
void CountOnMe(const int & MessageID, void* CountItem)
{
	((Receiver*)CountItem)->_Counter++;
}

