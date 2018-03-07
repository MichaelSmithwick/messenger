#pragma once

struct DataCache
{
	char Name[80];
	char Color[80];
	int SerialNumber;
};

void *(MessageToSend)(int MessageID, int MessageNumber, void* Payload);

class Sender
{
public:
	Sender();
	~Sender();
};

