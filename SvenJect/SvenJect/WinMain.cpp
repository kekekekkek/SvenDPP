#include "Include.h"

CVars g_Vars;
CDraw g_Draw;
CUtils g_Utils;
CInput g_Input;
CConsole g_Console;

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void Initialization(HINSTANCE hInstance)
{
	HANDLE hMutex = CreateMutexA(NULL, NULL, "SvenJectMutex");
	g_Vars.hCurInstance = hInstance;

	g_Vars.iScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	g_Vars.iScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	g_Vars.hSvenJectWnd = g_Utils.CreateWnd(hInstance, &WndProc, WS_POPUP, NULL, ((g_Vars.iScreenSizeX / 2) - (g_Vars.iWndSizeW / 2)),
		((g_Vars.iScreenSizeY / 2) - (g_Vars.iWndSizeH / 2)), g_Vars.iWndSizeW, g_Vars.iWndSizeH, "SvenJectWndClass", "SvenJector", NULL, RGB(0, 0, 0));

	g_Vars.wStrToken = g_Utils.GetRegValueString(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"Token");
	g_Vars.wStrSteamAPIKey = g_Utils.GetRegValueString(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"SteamAPI");
	g_Vars.wStrChannelId = g_Utils.GetRegValueString(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"ChannelID");
	g_Vars.iProc = _wtoi(g_Utils.GetRegValueString(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"Process").c_str());

	if (FindWindowA("#32770", "SvenJector Error") != NULL)
		ExitProcess(NULL);

	if (hMutex != NULL)
	{
		if (WaitForSingleObject(hMutex, 0) > 0)
		{
			MessageBoxA(NULL, "Cannot run the program. Only one instance of the program can be started at a time.", "SvenJector Error", (MB_OK | MB_ICONERROR));
			SendMessageA(g_Vars.hSvenJectWnd, WM_QUIT, NULL, NULL);

			ReleaseMutex(hMutex);
		}
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SETTEXT)
	{
		g_Draw.bUpdateOnce = false;
		g_Draw.bDisabled[4] = true;

		MessageBoxA(hWnd, (char*)lParam, "SvenJector Info", (MB_OK | MB_ICONINFORMATION));
		return TRUE;
	}

	if (uMsg == WM_QUIT
		|| uMsg == WM_CLOSE
		|| uMsg == WM_DESTROY)
		PostQuitMessage(0x1);

	if (uMsg == WM_CHAR)
		g_Input.iCurKeyPressed = wParam;

	if (uMsg == WM_KEYDOWN)
		g_Input.iKeyPressed = wParam;

	if (uMsg == WM_LBUTTONDOWN)
	{
		g_Input.bIsLeftMouseClicked = TRUE;
		g_Input.bIsLeftMouseHold = TRUE;
	}

	if (uMsg == WM_RBUTTONDOWN)
	{
		g_Input.bIsRightMouseClicked = TRUE;
		g_Input.bIsRightMouseHold = TRUE;
	}

	if (uMsg == WM_LBUTTONUP)
		g_Input.bIsLeftMouseHold = FALSE;

	if (uMsg == WM_RBUTTONUP)
		g_Input.bIsRightMouseHold = FALSE;

	if (uMsg == WM_MOUSEMOVE)
	{
		g_Input.iMousePosX = LOWORD(lParam);
		g_Input.iMousePosY = HIWORD(lParam);
	}

	if (uMsg == WM_MOUSEWHEEL)
		(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? g_Input.iMouseWheel = 1 : g_Input.iMouseWheel = -1);

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommandLine, int iShowWnd)
{
	Initialization(hInstance);

	if (IsWindow(g_Vars.hSvenJectWnd))
	{
		ShowWindow(g_Vars.hSvenJectWnd, SHOW_OPENWINDOW);
		UpdateWindow(g_Vars.hSvenJectWnd);

		while (true)
		{
			MSG msMsg;
			if (PeekMessageA(&msMsg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msMsg);
				DispatchMessageA(&msMsg);

				if (msMsg.message == WM_QUIT)
					break;
			}

			g_Draw.DrawMainFrame(g_Vars.hSvenJectWnd);
			g_Input.DragWindow(g_Vars.hSvenJectWnd, g_Vars.iWndSizeW, g_Vars.iWndSizeH, g_Vars.iScreenSizeX, g_Vars.iScreenSizeY);
		}
	}
	else
		MessageBoxA(NULL, "Couldn't create a window! Try to restart the program.", "SvenJector Error", (MB_OK | MB_ICONERROR));

	return TRUE;
}