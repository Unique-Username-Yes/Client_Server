#include "ClientControl.h"
#include <iostream>

int ClientControl::Init()
{
	if (int err = WSAStartup(MAKEWORD(2, 2), p_wsaData.get()); err != 0)
	{
		std::cerr << "WSAData initilization failed with error code: " << err << std::endl;
		return 1;
	}

	ZeroMemory(p_hints.get(), sizeof(*p_hints));
	p_hints->ai_family = AF_INET;
	p_hints->ai_socktype = SOCK_STREAM;
	p_hints->ai_protocol = IPPROTO_TCP;

	// Get Address and port of server
	if (int err = getaddrinfo(serverName, serverPort, p_hints.get(), &result); err != 0)
	{
		std::cerr << "getaddrinfo failed with error: " << err << std::endl;
		return 1;
	}

	// Look for first viable address
	for (addrinfo *ptr = result; ptr != nullptr; ptr = ptr->ai_next)
	{
		// Create connection socket
		soc_connection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (soc_connection == INVALID_SOCKET)
		{
			std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			return 1;
		}

		// Connect to server
		if (int err = connect(soc_connection, ptr->ai_addr, (int)ptr->ai_addrlen); err == SOCKET_ERROR)
		{
			soc_connection = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (soc_connection == INVALID_SOCKET)
	{
		std::cerr << "Unable to connect to the server" << std::endl;
		return 1;
	}

	// Hello from server
	if (int err = Receive(); err > 0)
	{
		std::cerr << "Failed to read initial greeting" << std::endl;
		return 1;
	}
}

int ClientControl::SendAndListen(const std::string message)
{
	if (int err = Send(message) > 0)
		return 1;
	if (int err = Receive();err>0)
		return 1;
}

int ClientControl::Send(const std::string message)const
{
	if (soc_connection == INVALID_SOCKET)
	{
		std::cerr << "Send failed - Unable to connect to the server" << std::endl;
		return 1;
	}

	if (int err = send(soc_connection, message.c_str(), message.size() + 1, 0); err == SOCKET_ERROR)
	{
		std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}
	return 0;
}

int ClientControl::Receive()
{
	if (soc_connection == INVALID_SOCKET)
	{
		std::cerr << "Receive failed - Unable to connect to the server" << std::endl;
		return 1;
	}


	if (int receive = recv(soc_connection, recv_buff, DEFAULT_BUFFLEN, 0); receive < 0)
	{
		std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}
	else if (receive == 0)
	{
		std::cout << "Server> Closing connection..." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Server> " << recv_buff << std::endl;
		return 0;
	}
}

ClientControl::~ClientControl()
{
	closesocket(soc_connection);
	WSACleanup();
}