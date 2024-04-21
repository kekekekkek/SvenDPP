#include "Include.h"

int CInput::GetCurKey()
{
	return iCurKeyPressed;
}

void CInput::ResetCurKey()
{
	iCurKeyPressed = -1;
}

void CInput::ResetAsyncKeys()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetAsyncKeyState(i))
			continue;
	}
}

bool CInput::IsLeftMouseHold()
{
	return bIsLeftMouseHold;
}

bool CInput::IsRightMouseHold()
{
	return bIsRightMouseHold;
}

POINT CInput::GetWndCursorPos()
{
	POINT pMousePos = { iMousePosX, iMousePosY };
	return pMousePos;
}

bool CInput::KeyPressed(INT iKey)
{
	if (iKeyPressed == iKey)
	{
		iKeyPressed = -1;
		return TRUE;
	}

	return FALSE;
}

bool CInput::IsLeftMouseClicked()
{
	if (bIsLeftMouseClicked)
	{
		bIsLeftMouseClicked = FALSE;
		return TRUE;
	}

	return FALSE;
}

bool CInput::IsRightMouseClicked()
{
	if (bIsRightMouseClicked)
	{
		bIsRightMouseClicked = FALSE;
		return TRUE;
	}

	return FALSE;
}

POINT CInput::GetGlobalCursorPos()
{
	POINT pCursorPos;
	GetCursorPos(&pCursorPos);

	return pCursorPos;
}

bool CInput::IsMouseHover(HWND hWnd, RECT& rcOut)
{
	RECT rcWndPos = g_Utils.GetCurWindowRect(hWnd);
	RECT rcWndSize = g_Utils.GetCurClientRect(hWnd);

	rcOut = rcWndSize;
	POINT pGlobalCursorPos = GetGlobalCursorPos();

	if ((pGlobalCursorPos.x >= rcWndPos.left) && (pGlobalCursorPos.x <= (rcWndPos.left + rcWndSize.right))
		&& (pGlobalCursorPos.y >= rcWndPos.top) && (pGlobalCursorPos.y <= (rcWndPos.top + rcWndSize.bottom)))
		return true;

	return false;
}

void CInput::DragWindow(HWND hWnd, int iWndSizeW, int iWndSizeH, int iScreenX, int iScreenY)
{
	RECT rcWndPos = g_Utils.GetCurWindowRect(hWnd);
	RECT rcWndSize = g_Utils.GetCurClientRect(hWnd);

	if (!IsLeftMouseHold()
		|| GetForegroundWindow() != hWnd)
		iDrag = 0;

	if (iDrag == 0)
	{
		if (IsMouseHover(hWnd, 0, rcWndSize.right, 0, iDragHeight) && IsLeftMouseHold())
			iDrag = 1;
		else if (!IsMouseHover(hWnd, 0, rcWndSize.right, 0, iDragHeight) && IsLeftMouseHold())
			iDrag = 2;
	}

	if (iDrag == 1)
	{
		POINT pWndCursorPos = GetWndCursorPos();
		POINT pGlobalCursorPos = GetGlobalCursorPos();

		if (bSavePos)
		{
			iSavePosX = pWndCursorPos.x;
			iSavePosY = pWndCursorPos.y;

			bSavePos = false;
		}

		SetWindowPos(hWnd, NULL, (pGlobalCursorPos.x - iSavePosX), (pGlobalCursorPos.y - iSavePosY), iWndSizeW, iWndSizeH, NULL);
	}
	else
	{
		if (g_Utils.GetTaskPanelPos() == TASKBAR_POS_DOWN)
		{
			if (rcWndPos.top < 0)
				rcWndPos.top = 0;

			if (rcWndPos.left < 0)
				rcWndPos.left = 0;

			if ((rcWndPos.left + iWndSizeW) > iScreenX)
				rcWndPos.left = (iScreenX - iWndSizeW);

			if ((rcWndPos.top + iWndSizeH + g_Utils.GetTaskPanelSize()) > iScreenY)
				rcWndPos.top = (iScreenY - iWndSizeH - g_Utils.GetTaskPanelSize());
		}
		else if (g_Utils.GetTaskPanelPos() == TASKBAR_POS_UP)
		{
			if (rcWndPos.top < g_Utils.GetTaskPanelSize())
				rcWndPos.top = g_Utils.GetTaskPanelSize();

			if (rcWndPos.left < 0)
				rcWndPos.left = 0;

			if ((rcWndPos.left + iWndSizeW) > iScreenX)
				rcWndPos.left = (iScreenX - iWndSizeW);

			if ((rcWndPos.top + iWndSizeH + g_Utils.GetTaskPanelSize()) > iScreenY)
				rcWndPos.top = (iScreenY - iWndSizeH);
		}
		else if (g_Utils.GetTaskPanelPos() == TASKBAR_POS_LEFT)
		{
			if (rcWndPos.top < 0)
				rcWndPos.top = 0;

			if (rcWndPos.left < g_Utils.GetTaskPanelSize())
				rcWndPos.left = g_Utils.GetTaskPanelSize();

			if ((rcWndPos.left + iWndSizeW) > iScreenX)
				rcWndPos.left = (iScreenX - iWndSizeW);

			if ((rcWndPos.top + iWndSizeH + g_Utils.GetTaskPanelSize()) > iScreenY)
				rcWndPos.top = (iScreenY - iWndSizeH);
		}
		else if (g_Utils.GetTaskPanelPos() == TASKBAR_POS_RIGHT)
		{
			if (rcWndPos.top < 0)
				rcWndPos.top = 0;

			if (rcWndPos.left < 0)
				rcWndPos.left = 0;

			if ((rcWndPos.left + iWndSizeW) > (iScreenX - g_Utils.GetTaskPanelSize()))
				rcWndPos.left = (iScreenX - iWndSizeW - g_Utils.GetTaskPanelSize());

			if ((rcWndPos.top + iWndSizeH + g_Utils.GetTaskPanelSize()) > iScreenY)
				rcWndPos.top = (iScreenY - iWndSizeH);
		}
		else
		{
			if (rcWndPos.top < 0)
				rcWndPos.top = 0;

			if (rcWndPos.left < 0)
				rcWndPos.left = 0;

			if ((rcWndPos.left + iWndSizeW) > iScreenX)
				rcWndPos.left = (iScreenX - iWndSizeW);

			if ((rcWndPos.top + iWndSizeH) > iScreenY)
				rcWndPos.top = (iScreenY - iWndSizeH);
		}

		SetWindowPos(hWnd, NULL, rcWndPos.left, rcWndPos.top, iWndSizeW, iWndSizeH, NULL);
		bSavePos = true;
	}
}

bool CInput::IsMouseHover(HWND hWnd, int iHoverFromX, int iHoverToX, int iHoverFromY, int iHoverToY)
{
	RECT rcWndPos = g_Utils.GetCurWindowRect(hWnd);
	POINT pGlobalCursorPos = GetGlobalCursorPos();

	if ((pGlobalCursorPos.x >= (rcWndPos.left + iHoverFromX)) && (pGlobalCursorPos.x <= (rcWndPos.left + (iHoverFromX + iHoverToX)))
		&& (pGlobalCursorPos.y >= (rcWndPos.top + iHoverFromY)) && (pGlobalCursorPos.y <= (rcWndPos.top + (iHoverFromY + iHoverToY))))
		return true;

	return false;
}