// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
	SOCKET sListen = 0;
	sockaddr_in sin = { 0 };
	sockaddr_in remoteAddr = { 0 };
	char szText[] = "TCP Server Demo";
	int nAddrLen = 0;

	nAddrLen = sizeof(sockaddr_in);
	//fill sin  
	sin.sin_port = htons(4567);
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	//init wsa  
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "initlization failed!" << endl;

		exit(0);
	}

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind failed!" << endl;

		return 0;
	}

	if (listen(sListen, 2) == SOCKET_ERROR)
	{
		cout << "listen failed!" << endl;

		return 0;
	}

	SOCKET sClient = INADDR_ANY;

	while (true)
	{
		sClient = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);

		if (sClient == INVALID_SOCKET)
		{
			cout << "accept failed!" << endl;

			continue;
		}

		while (true)
		{
			send(sClient, szText, strlen(szText), 0);
			Sleep(1000);
		}	

		closesocket(sClient);

	}

	closesocket(sListen);

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
