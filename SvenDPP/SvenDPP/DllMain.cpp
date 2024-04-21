#include "Include.h"

#define HUD_PRINTNOTIFY 1
#define HUD_PRINTCONSOLE 2
#define HUD_PRINTTALK 3
#define HUD_PRINTCENTER 4

CVars g_Vars;
CUtils g_Utils;
CConsole g_Console;
CDetours g_Detours;
CDiscordAPI g_DiscordAPI;

ClientPrintAllFn ClientPrintAll = NULL;
OutputDebugStringFn OrigOutputDebugString = NULL;

DWORD WINAPI AsyncMessage(LPVOID lpParam)
{
	structDiscord* pStructDiscord = (structDiscord*)lpParam;
	string strGetMsg = pStructDiscord->chMsg;

	if (strGetMsg[0] != '☻'
		&& strGetMsg[0] != '')
		g_Vars.bCallOnce = false;

	if (strGetMsg[0] == '☻'
		|| strGetMsg[0] == '')
		strGetMsg.erase(0, 1);

	if (!strlen(g_Vars.chSaveMsg))
	{
		ZeroMemory(g_Vars.chSaveMsg, sizeof(g_Vars.chSaveMsg));
		strncpy(g_Vars.chSaveMsg, strGetMsg.c_str(), sizeof(g_Vars.chSaveMsg));
	}

	if (strstr(g_Vars.chSaveMsg, strGetMsg.c_str()))
	{
		if (!g_Vars.bCallOnce)
		{
			for (int i = 0; i < strGetMsg.length(); i++)
			{
				if (strGetMsg[i] == ':')
				{
					strGetMsg.insert(i, "]`");
					strGetMsg.insert(0, "`[");

					break;
				}
			}

			if (strGetMsg[2] != '('
				&& strGetMsg[2] != '[')
			{
				if (g_Utils.ToLowerCase(strGetMsg).find("!send") == string::npos)
				{
					g_DiscordAPI.SendMessageAsync(pStructDiscord->clBot, pStructDiscord->uChanId, strGetMsg);
					g_Vars.bCallOnce = true;
				}
			}
		}
	}
	else
	{
		ZeroMemory(g_Vars.chSaveMsg, sizeof(g_Vars.chSaveMsg));
		g_Vars.bCallOnce = false;
	}

	return NULL;
}

void HandleCommands(const message_create_t& msgCreate_t)
{
	if (!msgCreate_t.msg.content.empty())
	{
		if (g_Vars.strGetMsg != msgCreate_t.msg.content)
		{
			g_Vars.strGetMsg = msgCreate_t.msg.content;

			if (ClientPrintAll)
			{
				if (g_Vars.strGetMsg[0] != '`'
					&& g_Vars.strGetMsg[0] != '['
					&& g_Vars.strGetMsg[0] != '(')
				{
					string strText = "";
					vector<string> strArgs = g_Utils.GetArguments(g_Vars.strGetMsg);

					if (g_Utils.ToLowerCase(strArgs[0]) == "!send")
					{
						for (int i = 1; i < g_Utils.ArgCount(strArgs); i++)
							strText += (strArgs[i] + " ");
					}

					if (!strText.empty())
					{
						string strMsg = ("(Discord) " + msgCreate_t.msg.author.username + ": " + strText);
						g_DiscordAPI.SendMessageFromDiscord(ClientPrintAll, HUD_PRINTTALK, strMsg);
					}
				}
			}
		}
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

void WINAPI OutputDebugStringHook(LPCSTR lpOutput)
{
	structDiscord stDiscord = { };

	if (g_Vars.uChannelId != NULL)
	{
		stDiscord.clBot = g_DiscordAPI.clBot;
		stDiscord.uChanId = g_Vars.uChannelId;

		strncpy(stDiscord.chMsg, lpOutput, sizeof(stDiscord.chMsg));

		void* pMemory = malloc(sizeof(stDiscord));
		memcpy(pMemory, &stDiscord, sizeof(stDiscord));

		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AsyncMessage, pMemory, NULL, NULL);
		CloseHandle(hThread);
	}

	return OrigOutputDebugString(lpOutput);
}

bool Initialization(HMODULE hModule)
{
	if (!hModule)
		return false;

	HMODULE hServer = GetModuleHandleA("server.dll");

	if (hServer)
	{
		g_Vars.strToken = g_Utils.GetRegValueString(HKEY_CURRENT_USER, "SOFTWARE\\SvenJector", "Token");
		g_Vars.strChannelId = g_Utils.GetRegValueString(HKEY_CURRENT_USER, "SOFTWARE\\SvenJector", "ChannelID");

		if (!g_Vars.strChannelId.empty())
			g_Vars.uChannelId = _atoi64(g_Vars.strChannelId.c_str());

		g_DiscordAPI.SetToken(g_Vars.strToken);
		g_DiscordAPI.clBot = new cluster(g_DiscordAPI.GetToken());

		g_DiscordAPI.SendMessageAsync(g_DiscordAPI.clBot, g_Vars.uChannelId, "`SvenDPP.dll` successfully injected!");

		ClientPrintAll = ((ClientPrintAllFn)((DWORD)hServer + 0x25CA80));
		OrigOutputDebugString = (OutputDebugStringFn)g_Detours.TrampHook32((char*)OutputDebugStringA, (char*)OutputDebugStringHook, 6);

		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ListenCommands, NULL, NULL, NULL);
		CloseHandle(hThread);

		SendMessageA(FindWindowA("SvenJectWndClass", "SvenJector"), WM_SETTEXT, NULL, (LPARAM)"Successfully injected!");
	}
	else
	{
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