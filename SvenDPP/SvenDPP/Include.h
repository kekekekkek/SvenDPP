#pragma once
#pragma warning(disable: 4251)

#include <thread>
#include <string>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <dpp/dpp.h>
#include <Windows.h>
#include <wininet.h>
#include <algorithm>

using namespace std;
using namespace dpp;

/*Не PlayerSay хук (hServer + 0x136360), но вызывается
в этой же функции которая указана в скобках. Что-то похожее
на strcpy и sprintf*/
typedef const char* (__cdecl* UnknownFuncFn)(const char* chStr1, const char* chStr2, int iSize);

//Функции с переменными аргументами (могу сломаться при вызове игрой с большим количеством аргументов)
typedef int(__cdecl* UnknownFuncFn2)(int a1, const char* a2, const char* chPlayerName);
typedef int(__cdecl* UnknownFuncFn3)(const char* chText, const char* chCvar, const char* chValue);

#include "detours.h"
#include "SDK/Structs.h"

#include "Vars.h"
#include "Utils.h"
#include "Console.h"
#include "ServerAPI.h"
#include "DiscordAPI.h"
#include "SteamWebAPI.h"

extern CVars g_Vars;
extern CUtils g_Utils;
extern CConsole g_Console;
extern CServerAPI g_ServerAPI;
extern CDiscordAPI g_DiscordAPI;
extern CSteamWebAPI g_SteamWebAPI;