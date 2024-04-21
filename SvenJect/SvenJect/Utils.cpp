#include "Include.h"

bool CUtils::IsCurUserAnAdmin()
{
	return IsUserAnAdmin();
}

int CUtils::GetTaskPanelPos()
{
	HWND hTaskPanelWnd = FindWindowA("Shell_TrayWnd", NULL);

	RECT rcTaskPos = g_Utils.GetCurWindowRect(hTaskPanelWnd);
	RECT rcTaskSize = g_Utils.GetCurClientRect(hTaskPanelWnd);

	if (rcTaskPos.left == 0 && rcTaskSize.right == g_Vars.iScreenSizeX
		&& rcTaskPos.top > 0 && rcTaskSize.bottom > 0)
		return 1;

	if (rcTaskPos.left == 0 && rcTaskSize.right == g_Vars.iScreenSizeX
		&& rcTaskPos.top == 0 && rcTaskSize.bottom > 0)
		return 0;

	if (rcTaskPos.left == 0 && rcTaskSize.right < g_Vars.iScreenSizeX
		&& rcTaskPos.top == 0 && rcTaskSize.bottom == g_Vars.iScreenSizeY)
		return 2;

	if (rcTaskPos.left > 0 && rcTaskSize.right < g_Vars.iScreenSizeX
		&& rcTaskPos.top == 0 && rcTaskSize.bottom == g_Vars.iScreenSizeY)
		return 3;
}

int CUtils::GetTaskPanelSize()
{
	RECT rcTaskPanel = g_Utils.GetCurClientRect(FindWindowA("Shell_TrayWnd", NULL));

	if (GetTaskPanelPos() == TASKBAR_POS_DOWN
		|| GetTaskPanelPos() == TASKBAR_POS_UP)
		return rcTaskPanel.bottom;

	if (GetTaskPanelPos() == TASKBAR_POS_LEFT
		|| GetTaskPanelPos() == TASKBAR_POS_RIGHT)
		return rcTaskPanel.right;
}

RECT CUtils::GetCurWindowRect(HWND hWnd)
{
	RECT rcWndPos = { };
	GetWindowRect(hWnd, &rcWndPos);

	return rcWndPos;
}

RECT CUtils::GetCurClientRect(HWND hWnd)
{
	RECT rcWndSize = { };
	GetClientRect(hWnd, &rcWndSize);

	return rcWndSize;
}

string CUtils::ToLowerCase(string strText)
{
	for (int i = 0; i < strText.length(); i++)
		strText[i] = tolower(strText[i]);

	return strText;
}

wstring CUtils::ToLowerCase(wstring wStrText)
{
	for (int i = 0; i < wStrText.length(); i++)
		wStrText[i] = towlower(wStrText[i]);

	return wStrText;
}

bool CUtils::IsPathExists(string strPath)
{
	WIN32_FIND_DATAA wnd32FindDataA;
	HANDLE hFile = FindFirstFileA(strPath.c_str(), &wnd32FindDataA);

	if (hFile && hFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return true;
	}

	return false;
}

HMODULE CUtils::GetBaseModuleHandle(wstring wStrModule, DWORD dwProcessId)
{
	HMODULE hModule = NULL;
	HANDLE hToolHelp32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);

	if (hToolHelp32 != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me32;
		me32.dwSize = sizeof(me32);

		if (Module32First(hToolHelp32, &me32))
		{
			do
			{
				if (wcsstr(g_Utils.ToLowerCase(me32.szModule).c_str(), g_Utils.ToLowerCase(wStrModule).c_str()))
				{
					hModule = me32.hModule;
					break;
				}
			} while (Module32Next(hToolHelp32, &me32));
		}
	}

	CloseHandle(hToolHelp32);
	return hModule;
}

wstring CUtils::GetRegValueString(HKEY hKey, wstring wStrSubKey, wstring wStrValue)
{
	wchar_t wChValue[500];

	DWORD dwType = NULL;
	DWORD dwValue = sizeof(wChValue);

	if (RegGetValue(hKey, wStrSubKey.c_str(), wStrValue.c_str(), RRF_RT_ANY, &dwType, wChValue, &dwValue) == 2)
		return L"";

	return wChValue;
}

bool CUtils::SetRegValue(HKEY hKey, wstring wStrSubKey, wstring wStrKeyName, wstring wStrValue)
{
	if (RegSetKeyValue(hKey, wStrSubKey.c_str(), wStrKeyName.c_str(), REG_SZ, wStrValue.c_str(), (wStrValue.length() * 2)) == 0)
		return true;

	return false;
}

HWND CUtils::CreateWnd(HINSTANCE hInstance, WNDPROC WndProc, DWORD dwStyle, DWORD dwExStyle, int iX, int iY, int iW, int iH, string strClassName, string strWndName, HWND hWndParent, COLORREF cBkColor)
{
	WNDCLASSEXA wndClassExA = { };

	wndClassExA.cbSize = sizeof(wndClassExA);
	wndClassExA.style = (CS_VREDRAW | CS_HREDRAW);
	wndClassExA.cbClsExtra = NULL;
	wndClassExA.cbWndExtra = NULL;
	wndClassExA.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClassExA.hIconSm = wndClassExA.hIcon;
	wndClassExA.hInstance = hInstance;
	wndClassExA.lpfnWndProc = WndProc;
	wndClassExA.hbrBackground = CreateSolidBrush(cBkColor);
	wndClassExA.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassExA.lpszClassName = strClassName.c_str();
	wndClassExA.lpszMenuName = strWndName.c_str();

	if (RegisterClassExA(&wndClassExA))
		return CreateWindowExA(dwExStyle, wndClassExA.lpszClassName, wndClassExA.lpszMenuName, dwStyle, iX, iY, iW, iH, hWndParent, NULL, hInstance, NULL);

	return NULL;
}