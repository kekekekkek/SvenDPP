#include "Include.h"

string CUtils::GetProcName()
{
	char chProcName[500];
	string strProcName = "";

	GetModuleFileNameA(GetModuleHandleA(NULL), chProcName, sizeof(chProcName));
	strProcName = chProcName;

	return strProcName.erase(0, (strProcName.find_last_of("\\") + 1));
}

bool CUtils::IsNaN(string strValue)
{
	int iPointCount = 0;

	for (int i = 0; i < strValue.length(); i++)
	{
		if (i == 0 && strValue[i] == '-')
			continue;

		if (strValue[i] == '.')
		{
			iPointCount++;

			if (iPointCount < 2)
				continue;
		}

		if (!isdigit(strValue[i]))
			return true;
	}

	return false;
}

int CUtils::ArgCount(string strCommand)
{
	return GetArguments(strCommand).size();
}

int CUtils::ArgCount(vector<string> strArgs)
{
	return strArgs.size();
}

string CUtils::ToLowerCase(string strText)
{
	for (int i = 0; i < strText.length(); i++)
		strText[i] = tolower(strText[i]);

	return strText;
}

UINT64 CUtils::ToSteamID64(string strSteamId)
{
	if (strSteamId.find_last_of(":") != string::npos)
		return (((stoull(strSteamId.erase(0, (strSteamId.find_last_of(":") + 1))) << 1) | 0x0110000100000000ULL) + 1);

	return NULL;
}

string CUtils::Parse(string strValue, string strText)
{
	int iReadFrom = 0;
	string strOutput = "";

	if (strValue.empty()
		|| strText.empty())
		return strOutput;

	iReadFrom = strText.find(strValue);

	if (iReadFrom == string::npos)
		return strOutput;

	for (int i = (iReadFrom + strValue.length() + 3); i < strText.length(); i++)
	{
		if (strText[i] == 34)
			break;

		strOutput += strText[i];
	}

	return strOutput;
}

vector<string> CUtils::GetArguments(string strCommand)
{
	int iArg = 0;
	string strArg = "";
	vector<string> strCommands(0);

	strCommand.append(" ");

	for (int i = 0; i < strCommand.length(); i++)
	{
		if (strCommand[i] == ' '
			&& strCommand[i + 1] != ' ')
		{
			strCommands.push_back(strArg);

			strArg.clear();
			iArg++;

			continue;
		}

		strArg += strCommand[i];
	}

	return strCommands;
}

int CUtils::IsWhiteSpace(string strText, int iStart, int iEnd)
{
	if (iStart >= iEnd && strText[iEnd] != 34)
		return -1;

	for (int i = iStart; i < iEnd; i++)
	{
		if (strText[i] == 34)
			continue;

		if (strText[i] != 32)
			return 0;
	}

	return 1;
}

string CUtils::SendGETRequest(string strServerName, string strGETRequest)
{
	string strResult = "";

	HINTERNET hOpen = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;

	hOpen = InternetOpenA(NULL, NULL, NULL, NULL, NULL);

	if (hOpen)
	{
		hConnect = InternetConnectA(hOpen, strServerName.c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

		if (hConnect)
		{
			hRequest = HttpOpenRequestA(hConnect, "GET", strGETRequest.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, NULL);

			if (hRequest)
			{
				if (HttpSendRequestA(hRequest, NULL, NULL, NULL, NULL))
				{
					char chResult[4096];
					DWORD dwBytes = NULL;

					if (InternetReadFile(hRequest, chResult, sizeof(chResult), &dwBytes))
						strResult = chResult;
				}
			}
		}
	}

	InternetCloseHandle(hOpen);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);

	return strResult;
}

string CUtils::GetRegValueString(HKEY hKey, string strSubKey, string strValue)
{
	char chValue[500];

	DWORD dwType = NULL;
	DWORD dwValue = sizeof(chValue);

	if (RegGetValueA(hKey, strSubKey.c_str(), strValue.c_str(), RRF_RT_ANY, &dwType, chValue, &dwValue) == 2)
		return "";

	return chValue;
}

bool CUtils::SetRegValue(HKEY hKey, string strSubKey, string strKeyName, string strValue)
{
	if (RegSetKeyValueA(hKey, strSubKey.c_str(), strKeyName.c_str(), REG_SZ, strValue.c_str(), strValue.length()) == 0)
		return true;

	return false;
}