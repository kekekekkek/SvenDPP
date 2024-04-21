#include "Include.h"

string CDiscordAPI::GetToken()
{
	return strToken;
}

void CDiscordAPI::SetToken(string strValue)
{
	strToken = strValue;
}

void CDiscordAPI::SendMessageAsync(cluster* clBot, snowflake snflChanId, string strMsg)
{
	if (!GetToken().empty() && clBot)
		clBot->message_create_sync(message(snflChanId, strMsg));
}

void CDiscordAPI::SendMessageFromDiscord(ClientPrintAllFn fFunc, int iFlag, string strMsg)
{
	if (!strMsg.empty())
		fFunc(iFlag, strMsg.c_str(), "", "", "", "");
}

void CDiscordAPI::RunListening(cluster* clBot, function<void(const message_create_t&)> fFunc)
{
	if (clBot)
	{
		clBot->on_message_create(fFunc);
		clBot->start(dpp::st_wait);
	}
}