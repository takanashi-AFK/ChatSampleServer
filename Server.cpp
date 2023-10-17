#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h> 
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include<string>

using namespace std;

const int portNum{ 1888 };

int main()
{
	WSADATA	wsaData;
	// WinSock2.2 ����������		
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "WSAStartup is completed" << endl;

	// UDP���X���\�P�b�g�̍쐬
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;

	// �\�P�b�g�A�h���X�\���̂�p��
	SOCKADDR_IN	bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));	// 0�N���A�ŏ�����

	bindAddr.sin_family = AF_INET;					// IPv4�A�h���X�g�p
	bindAddr.sin_port = htons(portNum);			// �|�[�g�ԍ��w��
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// ���ׂĂ̎�����NIC���Ώ�

	// �\�P�b�g�A�h���X��񊄂蓖��
	if (bind(sock, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
	{
		cout << WSAGetLastError();
		return -1;
	}

		while (true)
		{
			char fromBuffer[1024];		// ��M������̊i�[�̈�
			char toBuffer[1024];	// ���M������̊i�[�̈�

			SOCKADDR_IN toAddr;	// ���M��\�P�b�g�A�h���X�����i�[����̈�
			int fromlen = sizeof(toAddr);
			int ret = recvfrom(sock, fromBuffer, sizeof(fromBuffer), 0, (SOCKADDR*)&toAddr, &fromlen);
			if (ret == SOCKET_ERROR)
			{
				cout << WSAGetLastError();
			}

			cout << fromBuffer << endl;
			cout << "���M���郁�b�Z�[�W:";
			
			cin >>toBuffer;
				/*
				�o��
				���M���b�Z�[�W����
				*/
			//�킩���
			int ret = sendto(sock, toBuffer, strlen(toBuffer), 0, (SOCKADDR*)&toAddr, sizeof(toAddr));	// �I�[��\0������
			if (ret != strlen(toBuffer) + 1)
			{
				cout << WSAGetLastError();
			}

			cout << "complete" << endl;
		}


		if (closesocket(sock) == SOCKET_ERROR)
		{
			cout << WSAGetLastError();
		}
		WSACleanup();	// WinSock�I������
}