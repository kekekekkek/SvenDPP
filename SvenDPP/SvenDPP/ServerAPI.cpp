#include "Include.h"

void CServerAPI::Execute(string strCommand)
{
	pServerFuncs->pfnServerCommand(strCommand.c_str());
	pServerFuncs->pfnServerExecute();
}

void CServerAPI::ClientPrintAll(string strMsg)
{
	pServerFuncs->pfnMessageBegin(MSG_ALL, SayText, NULL, NULL);
	pServerFuncs->pfnWriteString(strMsg.insert(0, " ").c_str());
	pServerFuncs->pfnMessageEnd();
}

string CServerAPI::GetSteamIdByPlayerName(string strName)
{
	edict_t* pEdict = g_ServerAPI.pServerFuncs->pfnFindEntityByString(NULL, "netname", strName.c_str());

	if (pEdict != NULL)
		return g_ServerAPI.pServerFuncs->pfnGetPlayerAuthId(pEdict);

	return "";
}

enginefuncs_s* CServerAPI::Initialization(HMODULE hServer)
{
	pServerFuncs = (enginefuncs_t*)((DWORD)hServer + 0x563C80);
	return pServerFuncs;
}

void CServerAPI::SetVariable(string strCVar, string strValue)
{
	if (g_Utils.IsNaN(strValue))
		g_ServerAPI.pServerFuncs->pfnCVarSetString(strCVar.c_str(), strValue.c_str());
	else
		g_ServerAPI.pServerFuncs->pfnCVarSetFloat(strCVar.c_str(), atof(strValue.c_str()));
}

void CServerAPI::RegisterVariable(string strName, string strDefValue)
{
	cvar_t* pCvar = new cvar_t();

	pCvar->flags = FCVAR_SERVER;
	pCvar->name = strName.c_str();
	pCvar->string = strDefValue.c_str();

	g_ServerAPI.pServerFuncs->pfnCVarRegister(pCvar);
}