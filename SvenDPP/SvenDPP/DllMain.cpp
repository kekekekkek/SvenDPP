#include "Include.h"

CVars g_Vars;
CUtils g_Utils;
CConsole g_Console;
CDetours g_Detours;
CServerAPI g_ServerAPI;
CDiscordAPI g_DiscordAPI;

UnknownFuncFn OrigUnknownFunc = NULL;

DWORD WINAPI AsyncMessage(LPVOID lpParam)
{
	structDiscord* pStructDiscord = (structDiscord*)lpParam;
	g_DiscordAPI.SendBotMessage(pStructDiscord->clBot, pStructDiscord->uChanId, pStructDiscord->chMsg);

	return NULL;
}

void CreateAsyncMessage(const char* chMsg)
{
	structDiscord stDiscord = { };

	stDiscord.clBot = g_DiscordAPI.clBot;
	stDiscord.uChanId = g_Vars.uChannelId;

	strncpy(stDiscord.chMsg, chMsg, sizeof(stDiscord.chMsg));

	void* pMemory = malloc(sizeof(stDiscord));
	memcpy(pMemory, &stDiscord, sizeof(stDiscord));

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AsyncMessage, pMemory, NULL, NULL);
	CloseHandle(hThread);
}

const char* UnknownFuncHook(const char* chStr1, const char* chStr2, int iSize)
{
	if (chStr1[0] == 2)
	{
		string strMsg = "";
		string strName = "";

		for (int a = 1; a < strlen(chStr1); a++)
		{
			if (chStr1[a] == ':')
			{
				for (int b = (a + 2); b < strlen(chStr1); b++)
					strMsg += chStr1[b];

				break;
			}

			strName += chStr1[a];
		}

		if (g_Utils.IsWhiteSpace(strMsg, 0, strMsg.length()) == 0)
		{
			if (g_Vars.uChannelId != NULL)
				CreateAsyncMessage(("`" + strName + "`: " + strMsg).c_str());
		}
	}

	return OrigUnknownFunc(chStr1, chStr2, iSize);
}

void HandleCommands(const message_create_t& msgCreate_t)
{
	if (!msgCreate_t.msg.content.empty())
	{
		string strText = "";
		vector<string> strArgs = g_Utils.GetArguments(msgCreate_t.msg.content);

		if (g_Utils.ToLowerCase(strArgs[0]) == "!send")
		{
			for (int i = 1; i < g_Utils.ArgCount(strArgs); i++)
				strText += (strArgs[i] + " ");
		}

		if (!strText.empty())
			g_ServerAPI.ClientPrintAll(("(Discord) " + msgCreate_t.msg.author.username + ": " + strText));
	}
}

//Trash
void ListenCommands()
{
	/*Не смог заставить работать эту вещь нормально, поэтому приходится создавать второй
	экземпляр класса clBot (бота)*/

	cluster clBot(g_Vars.strToken, (i_default_intents | i_message_content));

	clBot.on_message_create(HandleCommands);
	clBot.start(st_wait);
}

bool Initialization(HMODULE hModule)
{
	if (!hModule)
		return false;

	HMODULE hServer = GetModuleHandleA("server.dll");

	g_Vars.strToken = g_Utils.GetRegValueString(HKEY_CURRENT_USER, "SOFTWARE\\SvenJector", "Token");
	g_Vars.strChannelId = g_Utils.GetRegValueString(HKEY_CURRENT_USER, "SOFTWARE\\SvenJector", "ChannelID");

	if (!g_Vars.strChannelId.empty())
		g_Vars.uChannelId = _atoi64(g_Vars.strChannelId.c_str());

	if (hServer 
		&& !g_Vars.strToken.empty()
		&& g_Vars.uChannelId != NULL)
	{
		g_ServerAPI.Initialization(hServer);
		OrigUnknownFunc = (UnknownFuncFn)g_Detours.TrampHook32((char*)((DWORD)hServer + 0x0D6EB0), (char*)UnknownFuncHook, 6);

		g_DiscordAPI.SetToken(g_Vars.strToken);
		g_DiscordAPI.clBot = new cluster(g_DiscordAPI.GetToken());

		g_DiscordAPI.SendBotMessage(g_DiscordAPI.clBot, g_Vars.uChannelId, "`SvenDPP.dll` successfully injected to `" + g_Utils.GetProcName() + "`");
		SendMessageA(FindWindowA("SvenJectWndClass", "SvenJector"), WM_SETTEXT, NULL, (LPARAM)"Successfully injected!");

		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ListenCommands, NULL, NULL, NULL);
		CloseHandle(hThread);
	}
	else
	{
		if (hServer)
		{
			if (g_Vars.strToken.empty()
				|| g_Vars.uChannelId == NULL)
				MessageBoxA(NULL, "Token or channel id is NULL! Try to fill in the required fields and reinject the module again.\n\nPress \"OK\" to unload the module.", "SvenDPP Error", (MB_OK | MB_ICONERROR));
		}
		else
			MessageBoxA(NULL, "\"server.dll\" was not found! Start the server, and then try to perform the injection again.", "SvenDPP Error", (MB_OK | MB_ICONERROR));

		FreeLibraryAndExitThread(hModule, NULL);
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	if (dwReasonForCall == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Initialization, hModule, NULL, NULL);

	return TRUE;
}