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
	// ソケットアドレス構造体を用意
	SOCKADDR_IN	bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));	// 0クリアで初期化

	bindAddr.sin_family = AF_INET;					// IPv4アドレス使用
	bindAddr.sin_port = htons(portNum);			// ポート番号指定
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// すべての自分のNICが対象



	// ソケットアドレス情報割り当て
	if (bind(UDPSocket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
	{
		cout << WSAGetLastError();
		return -1;
	}

		while (true)
		{
			char Buffer[1024];		// 受信文字列の格納領域
			

			SOCKADDR_IN fromAddr;	// 送信先ソケットアドレス情報を格納する領域
			memset(&fromAddr, 0, sizeof(fromAddr));
			fromAddr.sin_family = AF_INET;                // IPv4アドレス
			fromAddr.sin_port = htons(portNum);            // サーバのポート番号
			inet_pton(AF_INET, "192.168.43.253", &fromAddr.sin_addr.s_addr);      // サーバのIPアドレス
			int fromlen = sizeof(fromAddr);
			int ret = recvfrom(UDPSocket, Buffer, sizeof(Buffer)-1, 0, (SOCKADDR*)&fromAddr, &fromlen);
			Buffer[ret] = '\0';
			if (ret == SOCKET_ERROR)
			{
				cout << WSAGetLastError();
			}
			cout << Buffer<< endl;

			cout << "送信するメッセージ:";
			
			cin >>Buffer;
				/*
				出力
				送信メッセージ入力
				*/
			//わかんね
			ret = sendto(UDPSocket, Buffer, strlen(Buffer), 0, (SOCKADDR*)&fromAddr, sizeof(fromAddr));	// 終端の\0も送る
			if (ret != strlen(Buffer))
			{
				cout << WSAGetLastError();
			}
		}


		if (closesocket(UDPSocket) == SOCKET_ERROR)
		{
			cout << WSAGetLastError();
		}
		WSACleanup();	// WinSock終了処理
}

int WSAInit()
{
	WSADATA	wsaData;
	// WinSock2.2 初期化処理		
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
	// UDPリスンソケットの作成
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;
	return sock;
}
