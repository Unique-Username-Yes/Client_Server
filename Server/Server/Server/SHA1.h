// Algorithm from https://github.com/vog/sha1

#pragma once

#include <iostream>
#include <string>
#include "BaseHash.h"

class SHA1 :public BaseHash
{
public:
	SHA1();
	std::string Encrypt(std::string &message)override;

private:
	void Update(std::istream &is);
	std::string Results();

	uint32_t digest[5];
	std::string buffer;
	uint64_t transforms;
};
