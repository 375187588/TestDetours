// client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>  
#include <windows.h>  

using namespace std;

#pragma comment(lib, "WS2_32.lib")  


int main()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	SOCKET sock = 0;

	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "initlization failed!" << endl;
		exit(0);
	}

	sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		cout << "failed socket!" << endl;

		return 0;
	}

	sockaddr_in sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr)) == -1)
	{
		cout << "connect failed!" << endl;

		return 0;
	}

	while (true)
	{
		char buffer[256] = "\0";
		int  nRecv = 0;

		nRecv = recv(sock, buffer, 256, 0);

		if (nRecv > 0)
		{
			buffer[nRecv] = '\0';

			cout << "reveive data: " << buffer << endl;
		}
		Sleep(10);
	}
	

	closesocket(sock);

	WSACleanup();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
