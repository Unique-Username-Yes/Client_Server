#pragma once

#include <string>
#include <WS2tcpip.h>
#include <memory>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFFLEN 512

class ClientControl
{
public:
	ClientControl(PCSTR serverName, PCSTR serverPort) : serverName(serverName),serverPort(serverPort) {};
	~ClientControl();

	int Init();

	int SendAndListen(const std::string message);
private:
	int Send(const std::string message)const;
	int Receive();

	PCSTR serverName;
	PCSTR serverPort;

	std::unique_ptr<WSADATA> p_wsaData = std::make_unique<WSADATA>();
	SOCKET soc_connection = INVALID_SOCKET;

	std::unique_ptr<addrinfo> p_hints = std::make_unique<addrinfo>();
	addrinfo *result = nullptr;

	char recv_buff[DEFAULT_BUFFLEN];
};