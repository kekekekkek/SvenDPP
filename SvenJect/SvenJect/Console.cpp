#include "Include.h"

bool CConsole::CreateConsole(bool bShowMsg)
{
	BOOL bResult = AllocConsole();

	if (bResult)
	{
		freopen("CONIN$", "w", stdin);
		freopen("CONOUT$", "w", stdout);

		if (bShowMsg)
			cout << "Console was allocated!\n";
	}

	return bResult;
}