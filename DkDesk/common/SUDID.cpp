#include"SUDID.h"
#include<string>
SUDID::SUDID()
{
	
}

SUDID SUDID::createUDID()
{
	uint32_t id = (rand() << 15) + rand();
	while (id < 100'000'000)
	{
		id *= 10;
	}
	while (id >= 1'000'000'000)
	{
		id /= 10;
	}
	SUDID uid;uid._udid = id;
	return uid;
}