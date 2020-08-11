// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <cstdio>
#include <windows.h>
#include <detours.h>
#include <WinSock2.h>

#pragma comment(lib,"detours.lib")
#pragma comment(lib,"ws2_32.lib")

int (WINAPI *pSend)(SOCKET s, const char* buf, int len, int flags) = send;
int WINAPI MySend(SOCKET s, const char* buf, int len, int flags);
int (WINAPI *pRecv)(SOCKET s, char* buf, int len, int flags) = recv;
int WINAPI MyRecv(SOCKET s, char* buf, int len, int flags);

FILE* pSendLogFile;
FILE* pRecvLogFile;
BOOL g_bMsgOnce = false;

int WINAPI MySend(SOCKET s, const char* buf, int len, int flags)
{
	MessageBoxA(0, "MySend", 0, 0);
	return pSend(s, buf, len, flags);
}

int WINAPI MyRecv(SOCKET s, char* buf, int len, int flags)
{
	MessageBoxA(0, "MyRecv", 0, 0);
	return pRecv(s, buf, len, flags);
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	if (!g_bMsgOnce)
	{
		MessageBoxA(0, "loaded", 0, 0);
		g_bMsgOnce = true;
	}

	if (DetourIsHelperProcess()) 
	{
		return TRUE;
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pSend, MySend);
		if (DetourTransactionCommit() == NO_ERROR)
		{
			MessageBox(0, "send() detoured successfully", "asd", MB_OK);
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pRecv, MyRecv);
		if (DetourTransactionCommit() == NO_ERROR)
		{
			MessageBox(0, "recv() detoured successfully", "asd", MB_OK);
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH) 
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)pSend, MySend);
		DetourTransactionCommit();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)pRecv, MyRecv);
		DetourTransactionCommit();
	}

	return TRUE;
}
