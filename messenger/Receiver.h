#pragma once

class Receiver
{
public:
	Receiver();
	~Receiver();

	// unfortunately class functions must be declared
	// static so there functions have normal (c_decl) type
	// pointers. The C++ name mangling makes class function pointers
	// impossible to use.
	// The problem with static class functions is they have no 'this'
	// pointer and no insantiation data. As long as they don't have
	// to update any class variables they are fine.
	// To be able to work with individual class instantiations some
	// work arounds have to be employed. One is to pass the instantiated
	// object to the callback function as a parameter. This somewhat defeats 
	// the purpose. Another method is to register the class at the same time
	// as the function is registered and have the class secretly passed as
	// a parameter to the function -- still requires some awareness of the class
	// Another possibility is to use non-class functions and make them friends 
	// of the class, but still they need to be associated with the instantiated
	// object somehow.
	// Generally a registration scheme is employed where the instantiated object
	// does get passed around and normal (non-static) functions are called by
	// using a function name to actual function mapping -- that is not done
	// here because this is supposed to be a simple example :)
	static void ShowTime(const int& MessageID);
	static void SayHello(const int& MessageID);
	static void SayGoodbye(const int& MessageID);
	static void RingABell(const int& MessageID);
	static void Exit(const int& MessageID);

	int GetCounter();

	friend void CountOnMe(const int& MessageID, void* CountItem);

private:
	int _Counter;
};

// friend function used to cause the instantiated object to count
// CountItem is a pointer to the instantiated object
// Since we don't want to include the Receiver header file everywhere we
// call this we make CountItem a void* pointer and let the internals
// of the function cast it appropriately
void CountOnMe(const int& MessageID, void* CountItem);

