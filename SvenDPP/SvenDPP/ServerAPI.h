#pragma once

class CServerAPI
{
public:
	void Execute(string strCommand);
	void ClientPrintAll(string strMsg);
	string GetSteamIdByPlayerName(string strName);
	enginefuncs_s* Initialization(HMODULE hServer);
	void SetVariable(string strCVar, string strValue);
	void RegisterVariable(string strName, string strDefValue);

public:
	enginefuncs_s* pServerFuncs = nullptr;
};