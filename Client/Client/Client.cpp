#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>	
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	setlocale(LC_ALL,"rus");
	WSAData wsaData;
	const short BUFF_SIZE = 1024;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Ошибка создания такой версии WinSock \n";
		std::cout << WSAGetLastError() << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Успешное создание WinSock \n";
	}
	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "Ошибка создания ClientSocket\n" << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Успешное создание ClientSocket" << std::endl;
	}


	in_addr ip_to_num;
	if (inet_pton(AF_INET, "127.0.0.1", &ip_to_num) <= 0)
	{
		std::cout << "Ошибка преобразования формата ip\n" << WSAGetLastError() << std::endl;
		return 1;
	}
	sockaddr_in ConnectInfo;
	ConnectInfo.sin_family = AF_INET;
	ConnectInfo.sin_addr = ip_to_num;
	ConnectInfo.sin_port = htons(1234);

	if (connect(ClientSocket, (sockaddr*)&ConnectInfo, sizeof(ConnectInfo)) != 0) {
		std::cout << "Ошибка подключения к серверу " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	else 
	{
		std::cout << "Успешное подключение, сервер готов к передаче сообщениями" << std::endl;


		std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Buffers for sending and receiving data
		short packet_size = 0;												// The size of sending / receiving packet in bytes

		while (true) {

			std::cout << "Your (Client) message to Server: ";
			fgets(clientBuff.data(), clientBuff.size(), stdin);

			packet_size = send(ClientSocket, clientBuff.data(), clientBuff.size(), 0);

			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

			packet_size = recv(ClientSocket, servBuff.data(), servBuff.size(), 0);

			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			else
				std::cout << "Server message: " << servBuff.data() << std::endl;

		}

	}
}

