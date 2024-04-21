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

typedef std::thread StdThread;
typedef void (WINAPI* OutputDebugStringFn)(LPCSTR lpOutput);
typedef int (__cdecl* ClientPrintAllFn)(int iDest, const char* chMsg, const char* chLine1, const char* chLine2, const char* chLine3, const char* chLine4);

#include "Vars.h"
#include "Utils.h"
#include "Console.h"
#include "Detours.h"
#include "DiscordAPI.h"

extern CVars g_Vars;
extern CUtils g_Utils;
extern CConsole g_Console;
extern CDetours g_Detours;
extern CDiscordAPI g_DiscordAPI;