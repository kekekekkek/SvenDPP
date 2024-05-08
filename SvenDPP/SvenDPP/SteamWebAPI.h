#pragma once

class CSteamWebAPI
{
public:
	string GetAPIKey();
	void SetAPIKey(string strValue);
	string GetSteamProfileCountry(string strSteamId);

private:
	string strAPIKey = "";
};