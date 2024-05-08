#include "Include.h"

string CSteamWebAPI::GetAPIKey()
{
	return strAPIKey;
}

void CSteamWebAPI::SetAPIKey(string strValue)
{
	strAPIKey = strValue;
}

string CSteamWebAPI::GetSteamProfileCountry(string strSteamId)
{
	if (!strSteamId.empty())
	{
		UINT64 uSteamId64 = g_Utils.ToSteamID64(strSteamId);

		if (uSteamId64 > 0)
		{
			string strCountryCode = g_Utils.Parse("loccountrycode", g_Utils.SendGETRequest("api.steampowered.com", ("/ISteamUser/GetPlayerSummaries/v0002/?key=" + strAPIKey + "&steamids=" + to_string(uSteamId64))));

			if (!strCountryCode.empty())
				return strCountryCode;
		}
	}

	return "";
}