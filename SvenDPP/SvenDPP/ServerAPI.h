#pragma once

class CServerAPI
{
public:
	void ClientPrintAll(string strMsg);
	enginefuncs_s* Initialization(HMODULE hServer);
	void RegisterVariable(string strName, string strDefValue);

	enginefuncs_s* pServerFuncs = nullptr;
};