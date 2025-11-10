#include <winsock2.h>
#include <Windows.h>
#include <print>
#include "consoleHelper.hpp"
#include "MinHook.h"
#include <string>
#include "gldraw.h"
#include "gltext.h"
#include "geom.h"
#include "Entities.h"
#include "esp.h"
#include "aimbot.h"

static_assert(sizeof(int*) == 8, "Only x64 build is supported!");
#ifndef NDEBUG
#error "Only Release build is supported!"
#endif


GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

const char* example = "ESP Box";
const char* example2 = "im inside fam";

bool contextCreated = false;
HGLRC myContext;
HGLRC gameContext;

HWND GameHWND = FindWindow(0, "Cube 2: Sauerbraten");

esp ESP;

void Draw() {
	//Save the games context
	HGLRC oldContext = wglGetCurrentContext();
	HDC hdc = GetDC(GameHWND);

	//Create our own context if it hasn't been created yet
	if (contextCreated == false)
	{
		//Create new context
		myContext = wglCreateContext(hdc);

		//Make thread use our context
		wglMakeCurrent(hdc, myContext);

		//Setup our context
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Get resolution
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		glOrtho(0.0, m_viewport[2], m_viewport[3], 0.0, 1.0, -1.0);  //might want to make these your actual screen resolution
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1.0);

		contextCreated = true;
	}

	//Make thread use our context
	wglMakeCurrent(hdc, myContext);

	//Draw something (a rectangle here)
	 ESP.Draw(glFont, numOfPlayers, entityList, matrix);

	//Make thread to use games context again
	wglMakeCurrent(hdc, oldContext);
	ReleaseDC(GameHWND, hdc);
}

typedef int (WSAAPI* PTR_SWAPBUFFER)(
	SOCKET s,
	LPWSABUF lpBuffers,
	DWORD dwBufferCount,
	LPDWORD lpNumberOfBytesSent,
	DWORD dwFlags,
	const struct sockaddr* lpTo,
	int iToLen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

// Pointer for calling original WsaSendTo.
PTR_SWAPBUFFER fpWglSwapBuffer = NULL;

// Detour function which overrides WsaSendTo.
int WINAPI HookFunctionWglSwapBuffer(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr* lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{


	Draw();
	Aimbot();

	return fpWglSwapBuffer(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen, lpOverlapped, lpCompletionRoutine);
}



DWORD WINAPI MainThread(const LPVOID hDllModule);
void CleanupAndExit(LPVOID hDllModule);

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	[[maybe_unused]] LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, MainThread, hinstDLL, NULL, NULL);
	}
	return TRUE;
}

DWORD WINAPI MainThread(const LPVOID hDllModule) {

	console::ConsoleHelper::Attach();
	std::println("[+] Press + to apply hook.");
	std::println("[+] Press - to unhook.");
	std::println("[+] Press END to exit.\n");

	if (MH_Initialize() != MH_OK)
	{
		std::println("[-] Failed MH_Initialize");
		CleanupAndExit(hDllModule);
	}
	std::println("[+] MH_Initialize successful.");

	initializeOffsets();

	HMODULE hModuleOpenGl = GetModuleHandle("opengl32.dll");
	PTR_SWAPBUFFER pFunctionToHook = (PTR_SWAPBUFFER)GetProcAddress(hModuleOpenGl, "wglSwapBuffers");

	std::println("[+] opengl32.dll found loaded at {:016X}", (uint64_t)hModuleOpenGl);
	std::println("[+] WSASendTo found at {:016X}", (uint64_t)pFunctionToHook);

	MH_STATUS status;
	if ((status = MH_CreateHook(pFunctionToHook, &HookFunctionWglSwapBuffer, (LPVOID*)&fpWglSwapBuffer)) != MH_OK)
	{
		MessageBox(NULL, "Failed MH_CreateHook", "Error", MB_OK);

		const std::string statusStr = std::to_string(status);
		std::println("[-] Failed MH_CreateHook: {}", statusStr);
		std::println("[-] Exiting, you can close the console window.");

		CleanupAndExit(hDllModule);
	}

	std::println("[+] Hook created");

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 0x1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_ADD) & 0x1)
		{
			std::println("[+] Hooking...");
			MH_EnableHook(MH_ALL_HOOKS);
		}

		if (GetAsyncKeyState(VK_SUBTRACT) & 0x1)
		{
			std::println("[+] Unhooking...");
			MH_DisableHook(MH_ALL_HOOKS);
		}

		Sleep(50);
	}

	std::println("[+] Exiting, you can close the console window.");

	CleanupAndExit(hDllModule);
}

void CleanupAndExit(LPVOID hDllModule)
{
	if (!console::ConsoleHelper::Detach())
	{
		std::println("[-] Failed to free console!");
	}

	MH_Uninitialize();
	FreeLibraryAndExitThread((HMODULE)hDllModule, 0);
}