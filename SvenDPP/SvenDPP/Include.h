#pragma once

#include <thread>
#include <string>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <dpp/dpp.h>
#include <Windows.h>
#include <algorithm>

using namespace std;
using namespace dpp;

/*Не PlayerSay хук (hServer + 0x136360), но вызывается
в этой же функции которая указана в скобках. Что-то похожее
на strcpy и sprintf*/

typedef const char* (__cdecl* UnknownFuncFn)(const char* chStr1, const char* chStr2, int iSize);
typedef std::thread StdThread;

#include "SDK/Structs.h"

#include "Vars.h"
#include "Utils.h"
#include "Console.h"
#include "Detours.h"
#include "ServerAPI.h"
#include "DiscordAPI.h"

extern CVars g_Vars;
extern CUtils g_Utils;
extern CConsole g_Console;
extern CDetours g_Detours;
extern CServerAPI g_ServerAPI;
extern CDiscordAPI g_DiscordAPI;