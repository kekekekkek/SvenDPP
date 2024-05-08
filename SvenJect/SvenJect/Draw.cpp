#include "Include.h"

void Close()
{
	SendMessageA(g_Vars.hSvenJectWnd, WM_CLOSE, NULL, NULL);
}

void Rollup()
{
	if (IsWindowVisible(g_Vars.hSvenJectWnd))
		ShowWindow(g_Vars.hSvenJectWnd, SW_MINIMIZE);
}

void About()
{
	MessageBoxA(g_Vars.hSvenJectWnd, "SvenJector by kek\nSvenJector Version: 0.4\n\n----- Edit Guide -----\n\"RMB\" - View the content;\n\"Backspace\" - Delete one character;\n\"Delete\" - Delete the entire line;\n\"Ctrl + V\" - Insert text;\n\"Ctrl + C\" - Copy text.", "About", (MB_OK | MB_ICONINFORMATION));
}

void Save()
{
	int iResult = 0;

	if (g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"Token", g_Vars.wStrToken))
		iResult++;

	if (g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"SteamAPI", g_Vars.wStrSteamAPIKey))
		iResult++;

	if (g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"ChannelID", g_Vars.wStrChannelId))
		iResult++;

	if (g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"Process", to_wstring(g_Vars.iProc)))
		iResult++;

	if (iResult == 0)
		MessageBoxA(g_Vars.hSvenJectWnd, "Error of saving field values.", "SvenJector Error", (MB_OK | MB_ICONERROR));

	if (iResult < 4 && iResult != 0)
		MessageBoxA(g_Vars.hSvenJectWnd, "Error when saving one of the fields.", "SvenJector Error", (MB_OK | MB_ICONERROR));
}

void Load()
{
	HWND hSvenWnd = NULL;
	
	if (g_Vars.iProc == 0)
		hSvenWnd = FindWindowA("SDL_app", "Sven Co-op");

	if (g_Vars.iProc == 1)
		hSvenWnd = FindWindowA("Surface", NULL);

	if (IsWindow(hSvenWnd))
	{
		DWORD dwProcessId = NULL;
		GetWindowThreadProcessId(hSvenWnd, &dwProcessId);

		if (dwProcessId)
		{
			if (g_Utils.GetBaseModuleHandle(L"SvenDPP.dll", dwProcessId))
			{
				g_Draw.bUpdateOnce = false;
				g_Draw.bDisabled[4] = true;

				MessageBoxA(g_Vars.hSvenJectWnd, "The module is already injected.", "SvenJector Error", (MB_OK | MB_ICONERROR));
				return;
			}
			else
			{
				string strProcess = "";

				if (g_Vars.iProc == 0)
				{
					strProcess = "svends.exe";
					hSvenWnd = FindWindowA("Surface", NULL);
				}

				if (g_Vars.iProc == 1)
				{
					strProcess = "svencoop.exe";
					hSvenWnd = FindWindowA("SDL_app", "Sven Co-op");
				}

				DWORD dwProcessId = NULL;
				GetWindowThreadProcessId(hSvenWnd, &dwProcessId);

				if (dwProcessId && g_Utils.GetBaseModuleHandle(L"SvenDPP.dll", dwProcessId))
				{
					g_Draw.bUpdateOnce = false;
					g_Draw.bDisabled[4] = true;

					MessageBoxA(g_Vars.hSvenJectWnd, ("The module is already injected in \"" + strProcess + "\".").c_str(), "SvenJector Error", (MB_OK | MB_ICONERROR));
					return;
				}
				else
				{
					if (g_Vars.iProc == 0)
						hSvenWnd = FindWindowA("SDL_app", "Sven Co-op");

					if (g_Vars.iProc == 1)
						hSvenWnd = FindWindowA("Surface", NULL);
				}
			}

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwProcessId);

			if (hProcess)
			{
				string strProcess = "";
				char chProcess[MAX_PATH];

				GetModuleFileNameExA(hProcess, NULL, chProcess, sizeof(chProcess));
				strProcess = chProcess;

				if (strProcess.find_last_of("\\") != string::npos)
				{
					vector<string> strGetModules(0);
					strProcess.erase(strProcess.find_last_of("\\"), (strProcess.length() - 1));

					if (strstr(chProcess, "Sven Co-op"))
					{
						for (int i = 0; i < ARRAYSIZE(g_Vars.strModules); i++)
						{
							if (!g_Utils.IsPathExists((strProcess + "\\" + g_Vars.strModules[i])))
								strGetModules.push_back(g_Vars.strModules[i]);
						}

						if (strGetModules.size() > 0)
						{
							string strMsg = "The following modules could not be found:\n\n";

							for (int i = 0; i < strGetModules.size(); i++)
							{
								strMsg += (to_string((i + 1)) + ") \"" + strGetModules[i] + "\"");

								if ((i + 1) >= strGetModules.size())
									strMsg += ".\n";
								else
									strMsg += ";\n";
							}

							strMsg += ("\nMake sure that all the necessary modules are located in the folder:\n\n\"" + strProcess + "\"");
							MessageBoxA(g_Vars.hSvenJectWnd, strMsg.c_str(), "SvenJector Error", (MB_OK | MB_ICONERROR));
						}
						else
						{
							GetModuleFileNameA(GetModuleHandleA(NULL), chProcess, sizeof(chProcess));
							strProcess = chProcess;

							if (strProcess.find_last_of("\\") != string::npos)
								strProcess.erase(strProcess.find_last_of("\\"), (strProcess.length() - 1));

							if (g_Utils.IsPathExists(strProcess + "\\SvenDPP.dll"))
							{
								string strModule = (strProcess + "\\SvenDPP.dll");
								const char* cchModule = strModule.c_str();

								void* pString = VirtualAllocEx(hProcess, NULL, (strlen(cchModule) + 1), (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);

								if (WriteProcessMemory(hProcess, pString, cchModule, (strlen(cchModule) + 1), NULL))
								{
									HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, ((LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA")), pString, NULL, NULL);

									if (hThread)
									{
										g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"Token", g_Vars.wStrToken);
										g_Utils.SetRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\SvenJector", L"ChannelID", g_Vars.wStrChannelId);

										CloseHandle(hThread);
									}
									else
										MessageBoxA(g_Vars.hSvenJectWnd, "Failed to create a remote thread.", "SvenJector Error", (MB_OK | MB_ICONERROR));
								}
								else
									MessageBoxA(g_Vars.hSvenJectWnd, "Error writing to process memory!", "SvenJector Error", (MB_OK | MB_ICONERROR));
							}
							else
								MessageBoxA(g_Vars.hSvenJectWnd, "The module \"SvenDPP.dll\" was not found!", "SvenJector Error", (MB_OK | MB_ICONERROR));
						}
					}
					else
						MessageBoxA(g_Vars.hSvenJectWnd, ("The " + string(g_Vars.iProc == 0 ? "game" : "sven dedicated server") + " process could not be found! Try to start the " + string(g_Vars.iProc == 0 ? "game" : "sven dedicated server") + " and try to load the module again.").c_str(), "SvenJector Info", (MB_OK | MB_ICONINFORMATION));
				}

				CloseHandle(hProcess);
			}
			else
				MessageBoxA(g_Vars.hSvenJectWnd, ("The " + string(g_Vars.iProc == 0 ? "game" : "sven dedicated server") + " process could not be opened! Try restart the program with administrator rights.").c_str(), "SvenJector Error", (MB_OK | MB_ICONERROR));
		}
	}
	else
		MessageBoxA(g_Vars.hSvenJectWnd, ("The " + string(g_Vars.iProc == 0 ? "game" : "sven dedicated server") + " process could not be found! Try to start the " + string(g_Vars.iProc == 0 ? "game" : "sven dedicated server") + " and try to load the module again.").c_str(), "SvenJector Info", (MB_OK | MB_ICONINFORMATION));
}

void CDraw::ResetFocus()
{
	for (int i = 0; i < ARRAYSIZE(bFocus); i++)
		bFocus[i] = false;
}

void CDraw::ResetSysMenu()
{
	for (int i = 0; i < ARRAYSIZE(bSysMenu); i++)
		bSysMenu[i] = false;
}

bool CDraw::IsFocusFound()
{
	for (int i = 0; i < ARRAYSIZE(bFocus); i++)
	{
		if (bFocus[i])
			return true;
	}

	return false;
}

void CDraw::ResetComboBox()
{
	for (int i = 0; i < ARRAYSIZE(bComboBox); i++)
		bComboBox[i] = false;
}

bool CDraw::IsComboBoxOpened()
{
	for (int i = 0; i < ARRAYSIZE(bComboBox); i++)
	{
		if (bComboBox[i])
			return true;
	}
}

void CDraw::EndDraw(HDC hWndDC)
{
	HWND hWnd = WindowFromDC(hWndDC);

	if (IsWindow(hWnd) &&
		hMemDC && hMemBitmap)
	{
		RECT rcWndRect = g_Utils.GetCurClientRect(hWnd);
		BitBlt(hWndDC, 0, 0, rcWndRect.right, rcWndRect.bottom, hMemDC, 0, 0, SRCCOPY);

		DeleteDC(hMemDC);
		DeleteObject(hMemBitmap);

		ReleaseDC(hWnd, hWndDC);
	}
}

void CDraw::BeginDraw(HDC hWndDC)
{
	HWND hWnd = WindowFromDC(hWndDC);

	if (IsWindow(hWnd))
	{
		RECT rcWndRect = g_Utils.GetCurClientRect(hWnd);

		hMemDC = CreateCompatibleDC(hWndDC);
		hMemBitmap = CreateCompatibleBitmap(hWndDC, rcWndRect.right, rcWndRect.bottom);

		SelectObject(hMemDC, hMemBitmap);
	}
}

void CDraw::UpdateFrame(HWND hWnd)
{
	RECT rcOut = { };

	if (!bUpdateOnce)
	{
		for (int i = 0; i < ARRAYSIZE(bDisabled); i++)
		{
			if (bFocus[i] && bDisabled[i])
				NextFocus(i);
		}

		if (g_Vars.iProc >= ARRAYSIZE(g_Vars.wStrProc))
			g_Vars.iProc = (ARRAYSIZE(g_Vars.wStrProc) - 1);

		if (g_Vars.iProc <= 0)
			g_Vars.iProc = 0;

		bUpdateOnce = true;
	}

	if (g_Input.GetCurKey() == VK_ESCAPE)
	{
		if (IsComboBoxOpened())
			ResetComboBox();
		else
		{
			Close();
			g_Input.ResetCurKey();
		}
	}

	if (GetForegroundWindow() != hWnd)
		ResetComboBox();

	if (GetForegroundWindow() != hWnd
		|| !g_Input.IsMouseHover(hWnd, rcOut))
	{
		g_Input.iDrag = 0;
		g_Input.bIsLeftMouseHold = FALSE;
	}

	if (!IsFocusFound())
	{
		if (g_Input.GetCurKey() == VK_TAB)
			bFocus[(ARRAYSIZE(bFocus) - 1)] = true;
	}
}

void CDraw::DrawFocus(RECT rcRect)
{
	COLORREF cSaveTextColor = GetTextColor(hMemDC);

	SetTextColor(hMemDC, NULL);
	DrawFocusRect(hMemDC, &rcRect);

	SetTextColor(hMemDC, cSaveTextColor);
}

void CDraw::NextFocus(int iElemNum)
{
	int iNextItem = (iElemNum + 1);

	ResetFocus();
	ResetComboBox();

	if (bDisabled[iNextItem])
		NextFocus(iNextItem);

	if (ARRAYSIZE(bFocus) > iNextItem)
		bFocus[iNextItem] = true;
	else
		bFocus[0] = true;
}

void CDraw::DrawMainFrame(HWND hWnd)
{
	HDC hWndDC = GetDC(hWnd);

	BeginDraw(hWndDC);
	UpdateFrame(hWnd);

	PaintBackground(hWnd, RGB(50, 55, 70));
	SetTitle(hWnd, L"SvenJector for SvenDPP");
	SetIcon(g_Vars.hCurInstance);

	DrawClose(hWnd, 0, &Close);
	DrawRollup(hWnd, 1, &Rollup);
	DrawAbout(hWnd, 2, &About);

	DrawButton(hWnd, RECT{ 127, 185, 50, 24 }, L"Load", 4, bDisabled[4], &Load);
	DrawButton(hWnd, RECT{ 182, 185, 50, 24 }, L"Save", 5, bDisabled[5], &Save);
	DrawButton(hWnd, RECT{ 237, 185, 57, 24 }, L"Cancel", 6, bDisabled[6], &Close);

	DrawEdit(hWnd, RECT{ 10, 85, 65, 23 }, 210, 1, L"Token", g_Vars.wStrToken, 125, EDIT_TYPE_DEFAULT);
	DrawEdit(hWnd, RECT{ 10, 119, 65, 23 }, 210, 2, L"Channel ID", g_Vars.wStrChannelId, 50, EDIT_TYPE_NUMBERSONLY);

	DrawEdit(hWnd, RECT{ 10, 153, 65, 23 }, 210, 3, L"Steam API", g_Vars.wStrSteamAPIKey, 125, EDIT_TYPE_DEFAULT);
	DrawComboBox(hWnd, RECT{ 10, 51, 65, 23 }, 210, 43, 0, g_Vars.iProc, ARRAYSIZE(g_Vars.wStrProc), L"Process", g_Vars.wStrProc);

	DrawBorder(0, 0, (g_Vars.iWndSizeW - 1), (g_Vars.iWndSizeH - 1), 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
	EndDraw(hWndDC);
}

void CDraw::PreviousFocus(int iElemNum)
{
	int iPerviousItem = (iElemNum - 1);

	ResetFocus();
	ResetComboBox();

	if (bDisabled[iPerviousItem])
		PreviousFocus(iPerviousItem);

	if (iPerviousItem >= 0)
		bFocus[iPerviousItem] = true;
	else
		bFocus[(ARRAYSIZE(bFocus) - 1)] = true;
}

void CDraw::SetIcon(HINSTANCE hInstance)
{
	BITMAP bBitmap = { };
	HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	if (hBitmap)
	{
		GetObject(hBitmap, sizeof(BITMAP), &bBitmap);

		HDC hInterDC = CreateCompatibleDC(hMemDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hInterDC, hBitmap);

		BitBlt(hMemDC, 7, 10, bBitmap.bmWidth, bBitmap.bmHeight, hInterDC, 0, 0, SRCCOPY);
		SelectObject(hInterDC, hOldBitmap);

		DeleteDC(hInterDC);
		DeleteObject(hBitmap);
	}
}

void CDraw::SetTitle(HWND hWnd, wstring wStrText)
{
	OutputText(wStrText, 16, RECT{ 27, 9, 500, 25 }, RGB(50, 55, 70),
		(GetForegroundWindow() == hWnd ? RGB(215, 230, 250) : RGB(125, 130, 150)), (DT_SINGLELINE | DT_LEFT));
}

void CDraw::UpdateFocus(int iCurKey, int iElemNum)
{
	if (iCurKey == VK_TAB
		&& !GetAsyncKeyState(VK_SHIFT))
		NextFocus(iElemNum);

	if (iCurKey == VK_TAB
		&& GetAsyncKeyState(VK_SHIFT))
		PreviousFocus(iElemNum);
}

void CDraw::DrawRect(RECT rcRect, COLORREF cColor)
{
	if (hMemDC)
	{
		rcRect.right += rcRect.left;
		rcRect.bottom += rcRect.top;

		HBRUSH hBrush = CreateSolidBrush(cColor);
		FillRect(hMemDC, &rcRect, hBrush);

		DeleteObject(hBrush);
	}
}

void CDraw::PaintBackground(HWND hWnd, COLORREF cBkColor)
{
	RECT rcWndRect = g_Utils.GetCurClientRect(hWnd);

	if (IsWindow(hWnd))
		DrawRect(rcWndRect, cBkColor);
}

void CDraw::DrawTriangle(int iX, int iY, COLORREF cColor)
{
	DrawRect(RECT{ 25 + iX, 25 + iY, 8, 1 }, cColor);
	DrawRect(RECT{ 26 + iX, 26 + iY, 6, 1 }, cColor);
	DrawRect(RECT{ 27 + iX, 27 + iY, 4, 1 }, cColor);
	DrawRect(RECT{ 28 + iX, 28 + iY, 2, 1 }, cColor);
}

void CDraw::DrawAbout(HWND hWnd, int iNum, function<void()> fFunc)
{
	RECT rcRect = { 0, 11, (g_Vars.iWndSizeW - 55), 25 };

	if (g_Input.IsMouseHover(hWnd, (g_Vars.iWndSizeW - 65), 16, 8, 16))
	{
		g_Input.iDrag = -1;

		if (g_Input.IsLeftMouseHold())
		{
			rcRect.top += 1;
			rcRect.right += 1;

			OutputText(L"?", 12, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));
			DrawBorder((g_Vars.iWndSizeW - 66), 8, 17, 16, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

			ResetFocus();
			ResetSysMenu();
			ResetComboBox();

			bSysMenu[iNum] = true;

			if (bSysMenu[iNum])
				bCall = true;
		}
		else
		{
			if (bSysMenu[iNum])
			{
				if (bCall)
				{
					fFunc();
					bCall = false;
				}
			}

			OutputText(L"?", 12, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

			if (GetForegroundWindow() == hWnd)
				DrawBorder((g_Vars.iWndSizeW - 66), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		}
	}
	else
	{
		if (bSysMenu[iNum])
		{
			if (g_Input.iDrag == 0)
				bCall = false;
		}

		if (g_Input.iDrag != 1
			&& g_Input.iDrag != 2)
		{
			if (g_Input.iDrag == -1)
			{
				if (!g_Input.IsLeftMouseHold())
					g_Input.iDrag = 0;
			}
		}

		OutputText(L"?", 12, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

		if (GetForegroundWindow() == hWnd)
			DrawBorder((g_Vars.iWndSizeW - 66), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		else
			DrawBorder((g_Vars.iWndSizeW - 66), 8, 17, 16, 1, RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70));
	}
}

void CDraw::DrawClose(HWND hWnd, int iNum, function<void()> fFunc)
{
	RECT rcRect = { 0, 9, (g_Vars.iWndSizeW - 8), 25 };

	if (g_Input.IsMouseHover(hWnd, (g_Vars.iWndSizeW - 23), 16, 8, 16))
	{
		g_Input.iDrag = -1;

		if (g_Input.IsLeftMouseHold())
		{
			rcRect.top += 1;
			rcRect.right += 1;

			OutputText(L"❌", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));
			DrawBorder((g_Vars.iWndSizeW - 24), 8, 17, 16, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

			ResetFocus();
			ResetSysMenu();
			ResetComboBox();

			bSysMenu[iNum] = true;

			if (bSysMenu[iNum])
				bCall = true;
		}
		else
		{
			if (bSysMenu[iNum])
			{
				if (bCall)
				{
					fFunc();
					bCall = false;
				}
			}

			OutputText(L"❌", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

			if (GetForegroundWindow() == hWnd)
				DrawBorder((g_Vars.iWndSizeW - 24), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		}
	}
	else
	{
		if (bSysMenu[iNum])
		{
			if (g_Input.iDrag == 0)
				bCall = false;
		}

		if (g_Input.iDrag != 1
			&& g_Input.iDrag != 2)
		{
			if (g_Input.iDrag == -1)
			{
				if (!g_Input.IsLeftMouseHold())
					g_Input.iDrag = 0;
			}
		}

		OutputText(L"❌", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

		if (GetForegroundWindow() == hWnd)
			DrawBorder((g_Vars.iWndSizeW - 24), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		else
			DrawBorder((g_Vars.iWndSizeW - 24), 8, 17, 16, 1, RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70));
	}
}

void CDraw::DrawRollup(HWND hWnd, int iNum, function<void()> fFunc)
{
	RECT rcRect = { 0, 11, (g_Vars.iWndSizeW - 33), 25 };

	if (g_Input.IsMouseHover(hWnd, (g_Vars.iWndSizeW - 44), 16, 8, 16))
	{
		g_Input.iDrag = -1;

		if (g_Input.IsLeftMouseHold())
		{
			rcRect.top += 1;
			rcRect.right += 1;

			OutputText(L"━", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));
			DrawBorder((g_Vars.iWndSizeW - 45), 8, 17, 16, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

			ResetFocus();
			ResetSysMenu();
			ResetComboBox();

			bSysMenu[iNum] = true;

			if (bSysMenu[iNum])
				bCall = true;
		}
		else
		{
			if (bSysMenu[iNum])
			{
				if (bCall)
				{
					fFunc();
					bCall = false;
				}
			}

			OutputText(L"━", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

			if (GetForegroundWindow() == hWnd)
				DrawBorder((g_Vars.iWndSizeW - 45), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		}
	}
	else
	{
		if (bSysMenu[iNum])
		{
			if (g_Input.iDrag == 0)
				bCall = false;
		}

		if (g_Input.iDrag != 1
			&& g_Input.iDrag != 2)
		{
			if (g_Input.iDrag == -1)
			{
				if (!g_Input.IsLeftMouseHold())
					g_Input.iDrag = 0;
			}
		}

		OutputText(L"━", 13, rcRect, RGB(50, 55, 70), RGB(125, 130, 150), (DT_SINGLELINE | DT_RIGHT));

		if (GetForegroundWindow() == hWnd)
			DrawBorder((g_Vars.iWndSizeW - 45), 8, 17, 16, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
		else
			DrawBorder((g_Vars.iWndSizeW - 45), 8, 17, 16, 1, RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70), RGB(50, 55, 70));
	}
}

void CDraw::DrawButton(HWND hWnd, RECT rcRect, wstring wStrText, int iNum, bool bLocked, function<void()> fFunc)
{
	if (bLocked)
	{
		DrawBorder(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));

		rcRect.top += 5;
		rcRect.left += 6;

		OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(100, 100, 100), (DT_SINGLELINE | DT_LEFT), "Tahoma");
	}

	if (!bLocked)
	{
		if (!GetAsyncKeyState(VK_SPACE))
		{
			if (bFocus[iNum] && GetForegroundWindow() == hWnd)
			{
				UpdateFocus(g_Input.GetCurKey(), iNum);

				if (g_Input.GetCurKey() == VK_RETURN)
					fFunc();

				g_Input.ResetCurKey();
			}
			else
				g_Input.ResetAsyncKeys();
		}

		if (g_Input.IsMouseHover(hWnd, rcRect.left, rcRect.right, rcRect.top, rcRect.bottom))
		{
			if (g_Input.IsLeftMouseHold()
				|| GetAsyncKeyState(VK_SPACE))
			{
				DrawBorder(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

				rcRect.top += 7;
				rcRect.left += 8;

				OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");

				ResetFocus();
				ResetComboBox();

				rcRect.top -= 7;
				rcRect.left -= 8;

				DrawFocus(RECT{ (rcRect.left + 3), (rcRect.top + 3), (rcRect.left + rcRect.right), (rcRect.top + rcRect.bottom) });

				bFocus[iNum] = true;
				bCall = true;
			}
			else
			{
				if (GetForegroundWindow() == hWnd
					&& !GetAsyncKeyState(VK_SPACE))
				{
					if (bCall)
					{
						fFunc();
						bCall = false;
					}
				}

				if (bFocus[iNum])
				{
					DrawBorder(rcRect.left, rcRect.top, (rcRect.right + 1), (rcRect.bottom + 1), 1, NULL, NULL, NULL, NULL);
					DrawBorder((rcRect.left + 1), (rcRect.top + 1), (rcRect.right - 1), (rcRect.bottom - 1), 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
				}
				else
					DrawBorder(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));

				rcRect.top += 5;
				rcRect.left += 6;

				OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");
			}
		}
		else
		{
			if (bFocus[iNum])
			{
				if (GetForegroundWindow() == hWnd
					&& GetAsyncKeyState(VK_SPACE))
				{
					DrawBorder(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

					rcRect.top += 7;
					rcRect.left += 8;

					OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");

					ResetFocus();
					ResetComboBox();

					rcRect.top -= 7;
					rcRect.left -= 8;

					DrawFocus(RECT{ (rcRect.left + 3), (rcRect.top + 3), (rcRect.left + rcRect.right), (rcRect.top + rcRect.bottom) });

					bFocus[iNum] = true;
					bCall = true;
				}
				else
				{
					if (bCall)
					{
						fFunc();
						bCall = false;
					}

					DrawBorder(rcRect.left, rcRect.top, (rcRect.right + 1), (rcRect.bottom + 1), 1, NULL, NULL, NULL, NULL);
					DrawBorder((rcRect.left + 1), (rcRect.top + 1), (rcRect.right - 1), (rcRect.bottom - 1), 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
				}
			}
			else
				DrawBorder(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));

			rcRect.top += 5;
			rcRect.left += 6;

			if (bFocus[iNum])
			{
				if (GetForegroundWindow() == hWnd
					&& !GetAsyncKeyState(VK_SPACE))
					OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");
				else
				{
					if (GetForegroundWindow() != hWnd)
						OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");
				}
			}
			else
				OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT), "Tahoma");
		}
	}
}

void CDraw::OutputText(wstring wStrText, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat)
{
	/*Можно использовать функцию GetCharABCWidths для получения размера символа,
	но я решил опустить этот момент*/

	rcRect.right += rcRect.left;
	rcRect.bottom += rcRect.top;

	SetBkColor(hMemDC, cBkColor);
	SetTextColor(hMemDC, cTextColor);

	SelectObject(hMemDC, GetStockObject(DEFAULT_GUI_FONT));
	DrawTextW(hMemDC, wStrText.c_str(), -1, &rcRect, uFormat);
}

void CDraw::OutputText(wstring wStrText, int iFontHeight, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat)
{
	HFONT hFont = CreateFontA(iFontHeight, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Marlett");
	SelectObject(hMemDC, hFont);

	rcRect.right += rcRect.left;
	rcRect.bottom += rcRect.top;

	SetBkColor(hMemDC, cBkColor);
	SetTextColor(hMemDC, cTextColor);

	DrawTextW(hMemDC, wStrText.c_str(), -1, &rcRect, uFormat);
	DeleteObject(hFont);
}

void CDraw::DrawEdit(HWND hWnd, RECT rcRect, int iSize, int iNum, wstring wStrText, wstring& wStrInput, int iMaxSize, int iType)
{
	if (!IsComboBoxOpened())
	{
		if (g_Input.IsMouseHover(hWnd, rcRect.left, (rcRect.right + iSize + 10), (rcRect.top - 9), (rcRect.bottom + 3)))
		{
			if (g_Input.IsRightMouseHold())
			{
				ResetFocus();
				ResetComboBox();

				bFocus[iNum] = true;

				if (!wStrInput.empty())
					MessageBoxW(g_Vars.hSvenJectWnd, wStrInput.c_str(), (L"SvenJector (" + wStrText + L")").c_str(), (MB_OK | MB_ICONINFORMATION));

				g_Input.bIsRightMouseHold = FALSE;
			}

			if (g_Input.IsLeftMouseHold()
				|| g_Input.IsRightMouseHold())
			{
				ResetFocus();
				ResetComboBox();

				bFocus[iNum] = true;
			}
		}

		if (iType == 1)
		{
			for (int i = 0; i < wStrInput.length(); i++)
			{
				if (!isdigit(wStrInput[i]))
				{
					wStrInput.clear();
					break;
				}
			}
		}

		if (bFocus[iNum] && GetForegroundWindow() == hWnd)
		{
			UpdateFocus(g_Input.GetCurKey(), iNum);

			if (GetAsyncKeyState(VK_DELETE))
				wStrInput.clear();

			if ((!GetAsyncKeyState('V') || !GetAsyncKeyState('C'))
				&& !GetAsyncKeyState(VK_CONTROL))
			{
				bControl = false;

				if (wStrInput.length() > iMaxSize)
					wStrInput.erase(iMaxSize, wStrInput.length());

				if (g_Input.GetCurKey() > 0
					&& g_Input.GetCurKey() <= 127)
				{
					if (g_Input.GetCurKey() == VK_BACK)
					{
						if (!wStrInput.empty())
							wStrInput.erase((wStrInput.length() - 1), 1);
					}
					else
					{
						if (wStrInput.length() < iMaxSize)
						{
							if (iType == 1)
							{
								if (isdigit(g_Input.iCurKeyPressed))
									wStrInput += g_Input.iCurKeyPressed;
							}
							else
							{
								if (g_Input.GetCurKey() != VK_TAB)
									wStrInput += g_Input.iCurKeyPressed;
							}
						}
					}

					g_Input.ResetCurKey();
				}
			}
			else
			{
				if (!bControl)
				{
					if (GetAsyncKeyState('V')
						&& GetAsyncKeyState(VK_CONTROL))
					{
						if (OpenClipboard(hWnd))
						{
							HANDLE hText = GetClipboardData(CF_UNICODETEXT);

							if (hText != NULL && hText != INVALID_HANDLE_VALUE)
								wStrInput.insert(wStrInput.length(), (wchar_t*)hText);

							CloseClipboard();
						}

						bControl = true;
					}

					if (GetAsyncKeyState('C')
						&& GetAsyncKeyState(VK_CONTROL))
					{
						if (OpenClipboard(hWnd))
						{
							HANDLE hText = GetClipboardData(CF_UNICODETEXT);
							void* pMemory = malloc(((wStrInput.length() + 1) * 2));

							wcscpy_s((wchar_t*)pMemory, (wStrInput.length() + 1), wStrInput.c_str());
							SetClipboardData(CF_UNICODETEXT, pMemory);

							CloseClipboard();
						}

						bControl = true;
					}
				}

				g_Input.ResetCurKey();
			}
		}
	}

	OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), (bFocus[iNum] && GetForegroundWindow() == hWnd ? RGB(180, 220, 255) : RGB(192, 192, 192)), (DT_SINGLELINE | DT_RIGHT));
	DrawBorder((rcRect.left + 75), (rcRect.top - 4), iSize, rcRect.bottom, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

	rcRect.left += 79;
	rcRect.right += (iSize - 75);

	OutputText(wStrInput, 15, rcRect, RGB(50, 55, 70), RGB(192, 192, 192), (DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS));
}

void CDraw::DrawBorder(int iX, int iY, int iW, int iH, int iSize, COLORREF cLeft, COLORREF cRight, COLORREF cUp, COLORREF cDown)
{
	DrawRect(RECT{ iX, iY, iW, iSize }, cUp);
	DrawRect(RECT{ iX, iY, iSize, iH }, cLeft);
	DrawRect(RECT{ iX, iH + iY, iW, iSize }, cDown);
	DrawRect(RECT{ iW + iX, iY, iSize, (iH + 1) }, cRight);
}

void CDraw::OutputText(wstring wStrText, int iFontHeight, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat, string strFont)
{
	HFONT hFont = CreateFontA(iFontHeight, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, strFont.c_str());
	SelectObject(hMemDC, hFont);

	rcRect.right += rcRect.left;
	rcRect.bottom += rcRect.top;

	SetBkColor(hMemDC, cBkColor);
	SetTextColor(hMemDC, cTextColor);

	DrawTextW(hMemDC, wStrText.c_str(), -1, &rcRect, uFormat);
	DeleteObject(hFont);
}

void CDraw::DrawComboBox(HWND hWnd, RECT rcRect, int iWidth, int iHeight, int iNum, int& iCurElem, int iArraySize, wstring wStrText, wstring* wStrValues)
{
	if (bFocus[iNum] && bComboBox[iNum])
	{
		RECT rcComboBox = { (rcRect.left + 75), (rcRect.top + (rcRect.bottom - 2)), 0, iHeight };
		DrawRect(RECT{ rcComboBox.left, rcComboBox.top, iWidth, rcComboBox.bottom }, RGB(50, 55, 70));

		rcRect.left += 82;
		rcRect.right += (iWidth - 75);
		rcRect.top += (rcRect.bottom + 2);

		for (int i = 0; i < iArraySize; i++)
		{
			rcRect.top += (20 * i);

			if (g_Input.IsMouseHover(hWnd, rcRect.left - 6, (iWidth - 1), (rcRect.top - 2), (rcRect.bottom - 5)))
			{
				RECT rcOffset = { };
				rcOffset.top = (rcRect.top - 1);

				DrawRect(RECT{ rcComboBox.left, rcOffset.top, iWidth, 18 }, RGB(175, 190, 210));
				OutputText(wStrValues[i], 15, rcRect, RGB(175, 190, 210), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS));

				if (g_Input.IsLeftMouseHold()
					|| g_Input.IsRightMouseHold()
					|| g_Input.KeyPressed(VK_SPACE)
					|| g_Input.KeyPressed(VK_RETURN))
					iCurElem = i;
			}
			else
				OutputText(wStrValues[i], 15, rcRect, RGB(50, 55, 70), RGB(215, 230, 250), (DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS));

			rcRect.top -= (20 * i);
		}

		rcRect.left -= 82;
		rcRect.right -= (iWidth - 75);
		rcRect.top -= (rcRect.bottom + 2);

		DrawBorder(rcComboBox.left, rcComboBox.top, iWidth, rcComboBox.bottom, 1, RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105));
	}

	OutputText(wStrText, 15, rcRect, RGB(50, 55, 70), (bFocus[iNum] && GetForegroundWindow() == hWnd ? RGB(180, 220, 255) : RGB(192, 192, 192)), (DT_SINGLELINE | DT_RIGHT));
	DrawBorder((rcRect.left + 75), (rcRect.top - 4), iWidth, rcRect.bottom, 1, RGB(75, 80, 105), RGB(120, 125, 150), RGB(75, 80, 105), RGB(120, 125, 150));

	rcRect.left += 79;
	rcRect.right += (iWidth - 75);

	(bFocus[iNum] && !bComboBox[iNum] && GetForegroundWindow() == hWnd ? OutputText(wStrValues[iCurElem], 15, rcRect, RGB(175, 190, 210), RGB(255, 255, 255), (DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS)) :
		OutputText(wStrValues[iCurElem], 15, rcRect, RGB(50, 55, 70), RGB(192, 192, 192), (DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS)));

	if (bFocus[iNum] && GetForegroundWindow() == hWnd)
	{
		UpdateFocus(g_Input.GetCurKey(), iNum);

		if (g_Input.KeyPressed(VK_UP)
			|| g_Input.KeyPressed(VK_LEFT))
		{
			iCurElem--;

			if (iCurElem <= 0)
				iCurElem = 0;

			ResetComboBox();
		}

		if (g_Input.KeyPressed(VK_DOWN)
			|| g_Input.KeyPressed(VK_RIGHT))
		{
			iCurElem++;

			if (iCurElem >= iArraySize)
				iCurElem = (iArraySize - 1);

			ResetComboBox();
		}

		if (g_Input.KeyPressed(VK_SPACE)
			|| g_Input.KeyPressed(VK_RETURN))
			bComboBox[iNum] = !bComboBox[iNum];

		g_Input.ResetCurKey();
	}

	if (g_Input.IsMouseHover(hWnd, (rcRect.left - iWidth), (rcRect.right + 10), (rcRect.top - 9), (rcRect.bottom + 3)))
	{
		if (g_Input.GetMouseWheelDirection() != 0)
		{
			if (g_Input.GetMouseWheelDirection() == MOUSEWHEEL_DIR_UP)
				g_Input.iKeyPressed = VK_UP;

			if (g_Input.GetMouseWheelDirection() == MOUSEWHEEL_DIR_DOWN)
				g_Input.iKeyPressed = VK_DOWN;

			g_Input.ResetMouseWheelDirection();
		}

		if (g_Input.IsLeftMouseHold()
			|| g_Input.IsRightMouseHold())
		{
			ResetFocus();
			ResetComboBox();

			bFocus[iNum] = true;
		}
	}

	if (g_Input.IsMouseHover(hWnd, rcRect.left, (rcRect.right + iWidth + 10), (rcRect.top - 9), (rcRect.bottom + 3)))
	{
		DrawTriangle((rcRect.right + iWidth - 156), (rcRect.top - 19), RGB(255, 255, 255));

		if (g_Input.IsLeftMouseHold()
			|| g_Input.IsRightMouseHold())
		{
			ResetFocus();
			ResetComboBox();

			bFocus[iNum] = true;
			bComboBox[iNum] = true;
		}

		if (g_Input.GetMouseWheelDirection() != 0)
		{
			if (g_Input.GetMouseWheelDirection() == MOUSEWHEEL_DIR_UP)
				g_Input.iKeyPressed = VK_UP;

			if (g_Input.GetMouseWheelDirection() == MOUSEWHEEL_DIR_DOWN)
				g_Input.iKeyPressed = VK_DOWN;

			g_Input.ResetMouseWheelDirection();
		}
	}
	else
	{
		DrawTriangle((rcRect.right + iWidth - 156), (rcRect.top - 19), (!bComboBox[iNum] ? RGB(215, 230, 250) : RGB(255, 255, 255)));

		if (g_Input.IsLeftMouseHold()
			|| g_Input.IsRightMouseHold())
			bComboBox[iNum] = false;
	}
}