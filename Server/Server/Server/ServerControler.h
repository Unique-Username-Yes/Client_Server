#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <memory>
#include "HashText.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFFLEN 512

const std::string CMD_EXIT = "\\closeserver";
const char CMD_ESCAPE = '\\';
const int HASH_ROUNDS = 10000;

class ServerControler
{
public:
	ServerControler(PCSTR serverName, PCSTR serverPort) : serverName(serverName),serverPort(serverPort) {};
	~ServerControler();

	int Init();

	void Start();

	void Shutdown();
private:
	int OnClientConnect(SOCKET client);

	int OnMessageReceived(SOCKET client);

	int OnServerCommand(SOCKET client, std::string command);

private:
	PCSTR serverName, serverPort;
	std::unique_ptr<WSADATA> p_wsaData = std::make_unique<WSADATA>();

	SOCKET soc_listener = INVALID_SOCKET;
	//SOCKET soc_client = INVALID_SOCKET;

	addrinfo *result = NULL;
	std::unique_ptr<addrinfo> p_hints = std::make_unique<addrinfo>();

	char recv_buff[DEFAULT_BUFFLEN];
	int recv_bufflen = DEFAULT_BUFFLEN;

	fd_set fd_master;

	bool isServerActive = true;

	HashText tHasher;
};