#include <iostream>
#include "ClientControl.h"


#define DEFAULT_PORT "26488"
#define SERVER_NAME "127.0.0.1"

int main(void)
{
	ClientControl cc(SERVER_NAME, DEFAULT_PORT);
	cc.Init();
	std::string input_buff;
	do
	{
		std::cout << "> ";
		std::getline(std::cin, input_buff);
		
		if (input_buff.size() > 0)
		{
			cc.SendAndListen(input_buff);
		}
		else
		{
			std::cout << "Terminating connection..." << std::endl;
		}
	} while (input_buff.size() > 0);

	return 0;
}