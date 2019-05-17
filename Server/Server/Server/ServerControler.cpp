#include "ServerControler.h"

int ServerControler::Init()
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
	p_hints->ai_flags = AI_PASSIVE;

	// Get server addr info
	if (int err = getaddrinfo(NULL, serverPort, p_hints.get(), &result); err != 0)
	{
		std::cerr << "getaddrinfo failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// Create Listener Socket
	soc_listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (soc_listener == INVALID_SOCKET)
	{
		std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Bind Socket
	if (int err = bind(soc_listener, result->ai_addr, (int)result->ai_addrlen); err == SOCKET_ERROR)
	{
		std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(soc_listener);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	// Listen in listener Socket
	if (int err = listen(soc_listener, SOMAXCONN); err == SOCKET_ERROR)
	{
		std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(soc_listener);
		WSACleanup();
		return 1;
	}

	FD_ZERO(&fd_master);

	FD_SET(soc_listener, &fd_master);

	return 0;
}

void ServerControler::Start()
{
	while (isServerActive)
	{
		fd_set fd_copy = fd_master;

		int soc_count = select(0, &fd_copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < soc_count; ++i)
		{
			SOCKET soc = fd_copy.fd_array[i];
			if (soc == INVALID_SOCKET)
			{
				std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
				closesocket(soc);
				FD_CLR(soc, &fd_master);
				continue;
			}

			// New connection
			if (soc == soc_listener)
			{
				SOCKET newSoc = accept(soc_listener, NULL, NULL);
				if (newSoc == INVALID_SOCKET)
				{
					std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
					closesocket(newSoc);
					FD_CLR(newSoc, &fd_master);
					continue;
				}

				FD_SET(newSoc, &fd_master);

				OnClientConnect(newSoc);
			}

			// Existing connection
			else
			{
				ZeroMemory(recv_buff, recv_bufflen);

				int receive = recv(soc, recv_buff, recv_bufflen, 0);
				if (receive == 0)
				{
					std::cout << "Connection closed by cliet #" << soc << std::endl;
					closesocket(soc);
					FD_CLR(soc, &fd_master);
				}
				else if (receive < 0)
				{
					std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
					closesocket(soc);
					FD_CLR(soc, &fd_master);
				}
				else
				{
					if (recv_buff[0] == CMD_ESCAPE)
					{
						if (int cmd = OnServerCommand(soc, std::string(recv_buff, receive)); cmd == 0)
						{
							break;
						}
						continue;
					}

					if (soc != soc_listener)
					{
						OnMessageReceived(soc);
					}
				}
			}
		}
	}
	Shutdown();
}

int ServerControler::OnClientConnect(SOCKET client)
{
	std::string msg_welcome = "Successfuly connected to the MD5 encription server\r\n";

	if (int err = send(client, msg_welcome.c_str(), msg_welcome.size() + 1, 0); err == SOCKET_ERROR)
	{
		std::cerr << "Greet new client with socket nr #" << client << " failed with error: " << WSAGetLastError();
		return 1;
	}
	return 0;
}

int ServerControler::OnMessageReceived(SOCKET client)
{
	std::cout << "Sending message to: " << client << std::endl;

	std::string msg = tHasher.RunHash(recv_buff, HASH_ROUNDS);
	msg.append("\r\n");
	if (int err = send(client, msg.c_str(), msg.size() + 1, 0); err == SOCKET_ERROR)
	{
		std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}
	return 0;
}

int ServerControler::OnServerCommand(SOCKET client, std::string command)
{
	if (command.compare(CMD_EXIT))
	{
		isServerActive = false;
		std::cout << "Client #" << client << " requested server shutdown" << std::endl;
		return 1;
	}
	return 0;
}

void ServerControler::Shutdown()
{
	FD_CLR(soc_listener, &fd_master);
	closesocket(soc_listener);

	std::string msg_quit = "Server is shutting down...";

	SOCKET soc;

	while (fd_master.fd_count > 0)
	{
		soc = fd_master.fd_array[0];
		send(soc, msg_quit.c_str(), msg_quit.size() + 1, 0);

		FD_CLR(soc, &fd_master);
		closesocket(soc);
	}

	WSACleanup();
}

ServerControler::~ServerControler()
{
	if (isServerActive)
		Shutdown();
}