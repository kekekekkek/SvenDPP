#pragma once

class CUtils
{
public:
	string GetProcName();
	int ArgCount(string strCommand);
	int ArgCount(vector<string> strArgs);
	string ToLowerCase(string strText);
	vector<string> GetArguments(string strCommand);
	int IsWhiteSpace(string strText, int iStart, int iEnd);
	string GetRegValueString(HKEY hKey, string strSubKey, string strValue);
	bool SetRegValue(HKEY hKey, string strSubKey, string strKeyName, string strValue);
};