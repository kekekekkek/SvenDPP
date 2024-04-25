#include "Include.h"

string CUtils::GetProcName()
{
	char chProcName[500];
	string strProcName = "";

	GetModuleFileNameA(GetModuleHandleA(NULL), chProcName, sizeof(chProcName));
	strProcName = chProcName;

	return strProcName.erase(0, (strProcName.find_last_of("\\") + 1));
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