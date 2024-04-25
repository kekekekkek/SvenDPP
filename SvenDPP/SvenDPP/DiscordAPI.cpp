#include "Include.h"

string CDiscordAPI::GetToken()
{
	return strToken;
}

void CDiscordAPI::SetToken(string strValue)
{
	strToken = strValue;
}

void CDiscordAPI::SendBotMessage(cluster* clBot, snowflake snflChanId, string strMsg)
{
	if (!GetToken().empty() && clBot)
		clBot->message_create_sync(message(snflChanId, strMsg));
}