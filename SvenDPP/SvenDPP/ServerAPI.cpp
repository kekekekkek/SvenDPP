#include "Include.h"

void CServerAPI::ClientPrintAll(string strMsg)
{
	pServerFuncs->pfnMessageBegin(MSG_ALL, SayText, NULL, NULL);
	pServerFuncs->pfnWriteString(strMsg.insert(0, " ").c_str());
	pServerFuncs->pfnMessageEnd();
}

enginefuncs_s* CServerAPI::Initialization(HMODULE hServer)
{
	pServerFuncs = (enginefuncs_t*)((DWORD)hServer + 0x563C80);
	return pServerFuncs;
}

void CServerAPI::RegisterVariable(string strName, string strDefValue)
{
	cvar_t* pCvar = new cvar_t();

	pCvar->flags = FCVAR_SERVER;
	pCvar->name = strName.c_str();
	pCvar->string = strDefValue.c_str();

	g_ServerAPI.pServerFuncs->pfnCVarRegister(pCvar);
}