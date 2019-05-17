#include "HashText.h"
#include "SHA1.h"

HashText::HashText()
{
	hashFunc = new SHA1();
}
HashText::HashText(const HashText &c)
{
	hashFunc = new SHA1();
}
HashText::~HashText()
{
	delete hashFunc;
}

std::string HashText::RunHash(std::string message, int roundNum)
{
	hash = hashFunc->Encrypt(message);
	for (int i = 0; i < roundNum - 1; ++i)
	{
		hash = hashFunc->Encrypt(hash);
	}
	return hash;
}