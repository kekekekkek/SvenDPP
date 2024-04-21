#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <iostream>
#include <Windows.h>
#include <functional>
#include <shlobj_core.h>
#include <Psapi.h>
#include <TlHelp32.h>

using namespace std;

#include "Vars.h"
#include "Draw.h"
#include "Utils.h"
#include "Input.h"
#include "Console.h"
#include "resource.h"

extern CVars g_Vars;
extern CDraw g_Draw;
extern CUtils g_Utils;
extern CInput g_Input;
extern CConsole g_Console;