#pragma once

class CUtils
{
public:
	string GetProcName();
	bool IsNaN(string strValue);
	int ArgCount(string strCommand);
	int ArgCount(vector<string> strArgs);
	string ToLowerCase(string strText);
	UINT64 ToSteamID64(string strSteamId);
	string Parse(string strValue, string strText);
	vector<string> GetArguments(string strCommand);
	int IsWhiteSpace(string strText, int iStart, int iEnd);
	string SendGETRequest(string strServerName, string strGETRequest);
	string GetRegValueString(HKEY hKey, string strSubKey, string strValue);
	bool SetRegValue(HKEY hKey, string strSubKey, string strKeyName, string strValue);
};