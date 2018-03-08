#include "stdafx.h"
#include "Sender.h"
#include <iostream>


Sender::Sender()
{
}


Sender::~Sender()
{
}

// show the menu to the user
void Sender::MessageChoices()
{
	std::cout << "\nWhat should I do?" << std::endl;
	std::cout << "\nA. ShowTime Message\nB. Say Hello Message\nC. Say Goodbye Message\nD. Ring A Bell Message\nQ. Quit" << std::endl;
}

// get the choice from the user
int Sender::GetChoice()
{
	char InBuffer[100];

	std::cout << "\n\'X\' exits the program, \'Q\' permits continuing past this part" << std::endl;
	std::cout << "Choose A, B, C, D, Q to quit the menu, or e(X)it : ";
	std::cin.getline(InBuffer, 80);

	return (int)(*InBuffer);
}

