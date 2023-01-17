#pragma once
#pragma warning
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996).
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "addys.h"
//ANYONE THAT HAS THIS DLL
/*
hello this was made on 12/26/2022, eyestep is fixed (hopefully works throughout the years)
fuck the newbie kids on wrd that joined 2022, anyways, hopefully this is a good exploit
speedsterkawaii - 14 years, 5'5 freshman,
*/
DWORD WINAPI DllPipe(PVOID lvpParameter)
{
	std::string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\sexus"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			typedef int(__cdecl* print_func)(int, const char*, ...);
			print_func rbx_print = (print_func)(print_address);
			rbx_print(1, WholeScript.c_str());
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

static void DllLoad()
{
	DWORD PAGE_WRITE;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &PAGE_WRITE);
	*(BYTE*)(&FreeConsole) = 0xC3;
	FreeConsole();
	SetConsoleTitleA("[-] Script Execution By SpeedStarsKiwi [-]");
	freopen("CONOUT$", "w", stdout); 
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
	EyeStep::open(L"RobloxPlayerBeta.exe");
	printf("[*] Scanned target RobloxPlayerBeta.\n");

	int print_call = EyeStep::util::nextCall(EyeStep::scanner::scan_xrefs("Video recording stopped")[0], false, false);
	printf("[*] Got next call and cross reference.\n");
    print_address = base + EyeStep::util::raslr(print_call - 0x400000);
	printf("[*] Replaced first address with call.\n");
	typedef int(__cdecl* print_func)(int, const char*, ...);
	print_func rbx_print = (print_func)(print_address);

	auto spawn_call = EyeStep::scanner::scan_xrefs("Spawn function requires 1 argument")[0x000000000];
	printf("[*] Got next call and cross reference.\n");
	auto spawn_prologue = EyeStep::util::getPrologue(spawn_call);
	spawn_address = EyeStep::util::raslr(spawn_prologue - 0x400000);
	printf("[*] Replaced second address with call.\n");

	scanner().get();
	printf("[*] Grabbed task scheduler successfully.\n");
	scanner().print_jobs();

	scanner().get_datamodel();
	printf("[*] Grabbed datamodel jobs successfully.\n");

	scanner().get_script_context();
	printf("[*] Grabbed script context successfully.\n");

	rL = scanner().get_global_luastate();
	printf("[*] Console window will hide in a second.\n");

	rbx_print(1, "script execution * successfully loaded!");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllPipe, NULL, NULL, NULL);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		DllLoad();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}