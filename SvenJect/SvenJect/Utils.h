#pragma once

#define TASKBAR_POS_DOWN 1
#define TASKBAR_POS_UP 0
#define TASKBAR_POS_LEFT 2
#define TASKBAR_POS_RIGHT 3

class CUtils
{
public:
	int GetTaskPanelPos();
	int GetTaskPanelSize();
	bool IsCurUserAnAdmin();
	RECT GetCurWindowRect(HWND hWnd);
	RECT GetCurClientRect(HWND hWnd);
	bool IsPathExists(string strPath);
	string ToLowerCase(string strText);
	wstring ToLowerCase(wstring wStrText);
	HMODULE GetBaseModuleHandle(wstring wStrModule, DWORD dwProcessId);
	wstring GetRegValueString(HKEY hKey, wstring wStrSubKey, wstring wStrValue);
	bool SetRegValue(HKEY hKey, wstring wStrSubKey, wstring wStrKeyName, wstring wStrValue);
	HWND CreateWnd(HINSTANCE hInstance, WNDPROC WndProc, DWORD dwStyle, DWORD dwExStyle, int iX, int iY, int iW, int iH, string strClassName, string strWndName, HWND hWndParent, COLORREF cBkColor);
};