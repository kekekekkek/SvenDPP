#pragma once

#define EDIT_TYPE_DEFAULT 0
#define EDIT_TYPE_NUMBERSONLY 1

class CDraw
{
public:
	void ResetFocus();
	void ResetSysMenu();
	bool IsFocusFound();
	void ResetComboBox();
	bool IsComboBoxOpened();
	void EndDraw(HDC hWndDC);
	void BeginDraw(HDC hWndDC);
	void UpdateFrame(HWND hWnd);
	void DrawFocus(RECT rcRect);
	void NextFocus(int iElemNum);
	void DrawMainFrame(HWND hWnd);
	void PreviousFocus(int iElemNum);
	void SetIcon(HINSTANCE hInstance);
	void SetTitle(HWND hWnd, wstring wStrText);
	void UpdateFocus(int iCurKey, int iElemNum);
	void DrawRect(RECT rcRect, COLORREF cColor);
	void DrawTriangle(int iX, int iY, COLORREF cColor);
	void PaintBackground(HWND hWnd, COLORREF cBkColor);
	void DrawAbout(HWND hWnd, int iNum, function<void()> fFunc);
	void DrawClose(HWND hWnd, int iNum, function<void()> fFunc);
	void DrawRollup(HWND hWnd, int iNum, function<void()> fFunc);
	void DrawButton(HWND hWnd, RECT rcRect, wstring wStrText, int iNum, bool bLocked, function<void()> fFunc);
	void OutputText(wstring wStrText, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat);
	void OutputText(wstring wStrText, int iFontHeight, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat);
	void DrawEdit(HWND hWnd, RECT rcRect, int iSize, int iNum, wstring wStrText, wstring& wStrInput, int iMaxSize, int iType);
	void DrawBorder(int iX, int iY, int iW, int iH, int iSize, COLORREF cLeft, COLORREF cRight, COLORREF cUp, COLORREF cDown);
	void OutputText(wstring wStrText, int iFontHeight, RECT rcRect, COLORREF cBkColor, COLORREF cTextColor, UINT uFormat, string strFont);
	void DrawComboBox(HWND hWnd, RECT rcRect, int iWidth, int iHeight, int iNum, int& iCurElem, int iArraySize, wstring wStrText, wstring* wStrValues);

	bool bUpdateOnce = false;
	bool bIsKeyPressed = false;

	bool bComboBox[1] = { false };
	bool bDisabled[6]{ false, false, false, false, false, false };

private:
	bool bCall = false;
	bool bControl = false;

	bool bFocus[6]{ false, false, false, true, false, false };
	bool bSysMenu[3]{ false, false, false };

	HDC hMemDC = NULL;
	HBITMAP hMemBitmap = NULL;
};