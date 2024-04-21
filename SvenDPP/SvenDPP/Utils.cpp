#include "Include.h"

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