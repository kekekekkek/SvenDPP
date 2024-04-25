#pragma once

class CVars
{
public:
	wstring wStrToken = L"";
	wstring wStrChannelId = L"";

	HWND hSvenJectWnd = NULL;
	HINSTANCE hCurInstance = NULL;

	int iWndSizeW = 310;
	int iWndSizeH = 187;

	int iScreenSizeX = 0;
	int iScreenSizeY = 0;

	string strModules[6]
	{
		"dpp.dll",
		"libcrypto-1_1.dll",
		"libsodium.dll",
		"libssl-1_1.dll",
		"opus.dll",
		"zlib1.dll",
	};

	int iProc = 0;
	wstring wStrProc[2] =
	{
		L"svencoop.exe",
		L"svends.exe"
	};
};