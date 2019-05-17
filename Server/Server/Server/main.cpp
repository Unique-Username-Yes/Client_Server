#include "ServerControler.h"

#define SERVER_PORT "26488"
#define SERVER_NAME "127.0.0.1"

int main(void)
{
	ServerControler sc(SERVER_NAME, SERVER_PORT);
	
	if (sc.Init() != 0)
		return 1;
	sc.Start();
	return 0;
}




//auto p_wsaData = std::make_unique<WSADATA>();
////int err;

//SOCKET soc_listener = INVALID_SOCKET;
//SOCKET soc_client = INVALID_SOCKET;

//addrinfo *result = NULL;
//auto p_hints = std::make_unique<addrinfo>();

//int send_result;
//char recv_buff[DEFAULT_BUFFLEN];
//int recv_bufflen = DEFAULT_BUFFLEN;

//if (int err = WSAStartup(MAKEWORD(2, 2), p_wsaData.get()); err != 0)
//{
//	fprintf(stderr, "WSAData initilization failed with error code: %d\n", err);
//	return 1;
//}

//ZeroMemory(p_hints.get(), sizeof(*p_hints));
//p_hints->ai_family = AF_INET;
//p_hints->ai_socktype = SOCK_STREAM;
//p_hints->ai_protocol = IPPROTO_TCP;
//p_hints->ai_flags = AI_PASSIVE;

//// Get server addr info
//if (int err = getaddrinfo(NULL, DEFAULT_PORT, p_hints.get(), &result); err != 0)
//{
//	fprintf(stderr, "getaddrinfo failed with error: %d\n", err);
//	WSACleanup();
//	return 1;
//}

//// Create Socket
//soc_listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//if (soc_listener == INVALID_SOCKET)
//{
//	fprintf(stderr, "socket failed with error: %ld\n", WSAGetLastError());
//	freeaddrinfo(result);
//	WSACleanup();
//	return 1;
//}

//// Bind Socket
//if (int err = bind(soc_listener, result->ai_addr, (int)result->ai_addrlen); err == SOCKET_ERROR)
//{
//	fprintf(stderr,"bind failed with error: %d\n", WSAGetLastError());
//	freeaddrinfo(result);
//	closesocket(soc_listener);
//	WSACleanup();
//	return 1;
//}

//freeaddrinfo(result);

//// Listen in listener Socket
//if (int err = listen(soc_listener, SOMAXCONN); err == SOCKET_ERROR)
//{
//	fprintf(stderr, "listen failed with error: %d\n", WSAGetLastError());
//	closesocket(soc_listener);
//	WSACleanup();
//	return 1;
//}


//fd_set fd_master;
//FD_ZERO(&fd_master);

//FD_SET(soc_listener,&fd_master);

//bool isRunning = true;

//while (isRunning)
//{
//	fd_set fd_copy = fd_master;

//	int soc_count = select(0, &fd_copy, nullptr, nullptr, nullptr);
//	for (int i = 0; i < soc_count; ++i)
//	{
//		SOCKET soc = fd_copy.fd_array[i];
//		if (soc == INVALID_SOCKET)
//		{
//			fprintf(stderr, "socket failed with error: %ld\n", WSAGetLastError());
//			closesocket(soc);
//			FD_CLR(soc, &fd_master);
//			continue;
//		}

//		// New connection
//		if (soc == soc_listener)
//		{
//			soc_client = accept(soc_listener, NULL, NULL);
//			if (soc_client == INVALID_SOCKET)
//			{
//				fprintf(stderr, "accept failed with error: %d\n", WSAGetLastError());
//				closesocket(soc_client);
//				FD_CLR(soc_client, &fd_master);
//				continue;
//			}

//			FD_SET(soc_client, &fd_master);

//			std::string msg_welcome = "Successfuly connected to the MD5 encription server\r\n";
//			send_result = send(soc_client, msg_welcome.c_str(), msg_welcome.size() + 1, 0);
//			if (send_result == SOCKET_ERROR)
//			{
//				fprintf(stderr, "Send to socket #%d failed with error: %d\n", soc_client, WSAGetLastError());
//			}
//		}
//		// Existing connection
//		else
//		{
//			ZeroMemory(recv_buff, recv_bufflen);

//			int receive = recv(soc, recv_buff, recv_bufflen, 0);
//			if (receive <= 0)
//			{
//				printf("Connection closed by client...\n");
//				closesocket(soc);
//				FD_CLR(soc, &fd_master);
//			}
//			//else if (receive < 0)
//			//{
//			//	fprintf(stderr, "recv failed with error: %d\n", WSAGetLastError());
//			//	closesocket(soc);
//			//	FD_CLR(soc, &fd_master);
//			//}
//			else
//			{
//				if (recv_buff[0] == '\\')
//				{
//					printf("received command: %s\n", recv_buff);
//					std::string cmd = std::string(recv_buff, receive);
//					if (cmd == cmd_exit)
//					{
//						printf("Exit command found\n");
//						isRunning = false;
//						break;
//					}

//					continue;
//				}

//				if (soc != soc_listener)
//				{
//					printf("Sending msg to: %d",soc);
//					std::string msg_received = "Server received your message\r\n";
//					send_result = send(soc, msg_received.c_str(), msg_received.size() + 1, 0);
//					if (send_result == SOCKET_ERROR)
//					{
//						fprintf(stderr, "send failed with error: %d\n", WSAGetLastError());
//					}
//				}
//			}
//		}
//	}
//}

//FD_CLR(soc_listener, &fd_master);
//closesocket(soc_listener);

//std::string msg_quit = "Server is shutting down...";

//while (fd_master.fd_count > 0)
//{
//	soc_client = fd_master.fd_array[0];
//	send_result = send(soc_client, msg_quit.c_str(), msg_quit.size() + 1, 0);

//	FD_CLR(soc_client, &fd_master);
//	closesocket(soc_client);
//}


//WSACleanup();