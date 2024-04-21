#pragma once

class CVars
{
public:
	char chSaveMsg[256];
	bool bLockMsg = false;
	bool bCallOnce = false;

	bool bSendMsg = false;
	string strGetMsg = "";

	string strToken = "";
	string strChannelId = "";

	ULONG64 uChannelId = NULL;
};