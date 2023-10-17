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
	// WinSock2.2 初期化処理		
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "WSAStartup is completed" << endl;

	// UDPリスンソケットの作成
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;

	// ソケットアドレス構造体を用意
	SOCKADDR_IN	bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));	// 0クリアで初期化

	bindAddr.sin_family = AF_INET;					// IPv4アドレス使用
	bindAddr.sin_port = htons(portNum);			// ポート番号指定
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// すべての自分のNICが対象

	// ソケットアドレス情報割り当て
	if (bind(sock, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
	{
		cout << WSAGetLastError();
		return -1;
	}

		while (true)
		{
			char fromBuffer[1024];		// 受信文字列の格納領域
			char toBuffer[1024];	// 送信文字列の格納領域

			SOCKADDR_IN toAddr;	// 送信先ソケットアドレス情報を格納する領域
			int fromlen = sizeof(toAddr);
			int ret = recvfrom(sock, fromBuffer, sizeof(fromBuffer), 0, (SOCKADDR*)&toAddr, &fromlen);
			if (ret == SOCKET_ERROR)
			{
				cout << WSAGetLastError();
			}

			cout << fromBuffer << endl;
			cout << "送信するメッセージ:";
			
			cin >>toBuffer;
				/*
				出力
				送信メッセージ入力
				*/
			//わかんね
			int ret = sendto(sock, toBuffer, strlen(toBuffer), 0, (SOCKADDR*)&toAddr, sizeof(toAddr));	// 終端の\0も送る
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
		WSACleanup();	// WinSock終了処理
}