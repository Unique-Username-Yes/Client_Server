#pragma once

#include <string>

class BaseHash
{
public:
	virtual std::string Encrypt(std::string &message) = 0;
protected:
};