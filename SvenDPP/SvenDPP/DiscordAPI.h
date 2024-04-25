#pragma once

struct structDiscord
{
	char chMsg[256];
	ULONG64 uChanId = -1;
	cluster* clBot = nullptr;
};

class CDiscordAPI
{
public:
	string GetToken();
	void SetToken(string strValue);
	void SendBotMessage(cluster* clBot, snowflake snflChanId, string strMsg);

	cluster* clBot = nullptr;

private:
	string strToken = "";
};