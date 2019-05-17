#pragma once

#include <string>

class BaseHash;
class HashText
{
public:
	HashText();
	HashText(const HashText&);
	~HashText();
	std::string RunHash(std::string message, int roundNum);
private:
	BaseHash *hashFunc;
	std::string hash;
};