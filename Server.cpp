#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h> 
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include<string>

using namespace std;

const int portNum{ 8888 };
//127.0.0.2

int WSAInit();
SOCKET UDPSocketInit();

int main()
{
	WSAInit();
	SOCKET UDPSocket = UDPSocketInit();
	// �\�P�b�g�A�h���X�\���̂�p��
	SOCKADDR_IN	bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));	// 0�N���A�ŏ�����

	bindAddr.sin_family = AF_INET;					// IPv4�A�h���X�g�p
	bindAddr.sin_port = htons(portNum);			// �|�[�g�ԍ��w��
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// ���ׂĂ̎�����NIC���Ώ�



	// �\�P�b�g�A�h���X��񊄂蓖��
	if (bind(UDPSocket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
	{
		cout << WSAGetLastError();
		return -1;
	}

		while (true)
		{
			char Buffer[1024];		// ��M������̊i�[�̈�
			

			SOCKADDR_IN fromAddr;	// ���M��\�P�b�g�A�h���X�����i�[����̈�
			memset(&fromAddr, 0, sizeof(fromAddr));
			fromAddr.sin_family = AF_INET;                // IPv4�A�h���X
			fromAddr.sin_port = htons(portNum);            // �T�[�o�̃|�[�g�ԍ�
			inet_pton(AF_INET, "192.168.43.253", &fromAddr.sin_addr.s_addr);      // �T�[�o��IP�A�h���X
			int fromlen = sizeof(fromAddr);
			int ret = recvfrom(UDPSocket, Buffer, sizeof(Buffer)-1, 0, (SOCKADDR*)&fromAddr, &fromlen);
			Buffer[ret] = '\0';
			if (ret == SOCKET_ERROR)
			{
				cout << WSAGetLastError();
			}
			cout << Buffer<< endl;

			cout << "���M���郁�b�Z�[�W:";
			
			cin >>Buffer;
				/*
				�o��
				���M���b�Z�[�W����
				*/
			//�킩���
			ret = sendto(UDPSocket, Buffer, strlen(Buffer), 0, (SOCKADDR*)&fromAddr, sizeof(fromAddr));	// �I�[��\0������
			if (ret != strlen(Buffer))
			{
				cout << WSAGetLastError();
			}
		}


		if (closesocket(UDPSocket) == SOCKET_ERROR)
		{
			cout << WSAGetLastError();
		}
		WSACleanup();	// WinSock�I������
}

int WSAInit()
{
	WSADATA	wsaData;
	// WinSock2.2 ����������		
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "WSAStartup is completed" << endl;
	return 0;
}

SOCKET UDPSocketInit()
{
	// UDP���X���\�P�b�g�̍쐬
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;
	return sock;
}
