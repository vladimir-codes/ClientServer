#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <stdio.h>	
#include <thread>
#pragma comment(lib, "Ws2_32.lib")


int main()
{
	setlocale(LC_ALL, "rus");
	const short BUFF_SIZE = 1024;
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Error WinSock version initializaion\n";
		std::cout << WSAGetLastError() << std::endl;
		return 1;
	}
	else
	{
		std::cout << "WinSock initializaion is OK\n";
	}
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Server socket initialization is OK" << std::endl;
	}


	in_addr ip_to_num;
	if (inet_pton(AF_INET, "127.0.0.1", &ip_to_num) <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}
	sockaddr_in ConnectInfo;
	ConnectInfo.sin_family = AF_INET;
	ConnectInfo.sin_addr = ip_to_num;
	ConnectInfo.sin_port = htons(1234);

	if (bind(serverSocket, (sockaddr*)&ConnectInfo, sizeof(ConnectInfo)) != 0) 
	{
		std::cout << "Error Socket binding to server info.\n " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else 
	{
		std::cout << "Binding socket to Server info is OK" << std::endl;
	}


	if (listen(serverSocket, SOMAXCONN) != 0) {
		std::cout << "Can't start to listen to.\n " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Listening..." << std::endl;


		sockaddr_in clientInfo;
		int clientInfo_size = sizeof(clientInfo);
		SOCKET ClientConn = accept(serverSocket, (sockaddr*)&clientInfo, &clientInfo_size);

		if (ClientConn == INVALID_SOCKET) {
			std::cout << "Клиент не может подключиться к серверу(\n" << WSAGetLastError() << std::endl;
			closesocket(serverSocket);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}
		else {
			std::cout << "Успешное подключение клиента" << std::endl;


			std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
			short packet_size = 0;
			while (true) {


				recv(ClientConn, servBuff.data(), servBuff.size(), 0);
				std::cout << "Client's message: " << servBuff.data() << std::endl;

				std::cout << "Your (host) message: ";
				fgets(clientBuff.data(), clientBuff.size(), stdin);

				packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

				if (packet_size == SOCKET_ERROR) {
					std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
					closesocket(serverSocket);
					closesocket(ClientConn);
					WSACleanup();
					return 1;
				}

			}



		}
		closesocket(ClientConn);
	}
	closesocket(serverSocket);
	WSACleanup();



	return 0;
}


