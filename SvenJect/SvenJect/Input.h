#pragma once

class CInput
{
public:
	int GetCurKey();
	void ResetCurKey();
	void ResetAsyncKeys();
	bool IsLeftMouseHold();
	bool IsRightMouseHold();
	POINT GetWndCursorPos();
	bool KeyPressed(INT iKey);
	bool IsLeftMouseClicked();
	bool IsRightMouseClicked();
	POINT GetGlobalCursorPos();
	bool IsMouseHover(HWND hWnd, RECT& rcOut);
	void DragWindow(HWND hWnd, int iWndSizeW, int iWndSizeH, int iScreenX, int iScreenY);
	bool IsMouseHover(HWND hWnd, int iHoverFromX, int iHoverToX, int iHoverFromY, int iHoverToY);

public:
	int iKeyPressed = -1;
	int iCurKeyPressed = -1;

	int iMousePosX = -1;
	int iMousePosY = -1;

	bool bIsLeftMouseHold = FALSE;
	bool bIsRightMouseHold = FALSE;

	bool bIsLeftMouseClicked = FALSE;
	bool bIsRightMouseClicked = FALSE;

	int iSavePosX = 0;
	int iSavePosY = 0;

	int iDrag = 0;
	int iDragHeight = 25;
	bool bSavePos = true;
};