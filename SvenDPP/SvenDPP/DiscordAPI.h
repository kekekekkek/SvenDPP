#pragma once

struct structDiscord
{
	cluster* clBot = nullptr;
	ULONG64 uChanId = -1;
	char chMsg[256];
};

struct structListening
{
	cluster* clBot = nullptr;
	function<void(const message_create_t&)> fCallback;
};

class CDiscordAPI
{
public:
	string GetToken();
	void SetToken(string strValue);
	void SendMessageAsync(cluster* clBot, snowflake snflChanId, string strMsg);
	void SendMessageFromDiscord(ClientPrintAllFn fFunc, int iFlag, string strMsg);
	void RunListening(cluster* clBot, function<void(const message_create_t&)> fFunc);

	cluster* clBot = nullptr;

private:
	string strToken = "";
};