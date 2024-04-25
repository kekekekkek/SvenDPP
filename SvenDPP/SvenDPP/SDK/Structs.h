#pragma once

// edict
#define	MAX_ENT_LEAFS 48
#define MAX_PLAYERS 32

//Hud
#define HUD_PRINTNOTIFY 1
#define HUD_PRINTCONSOLE 2
#define HUD_PRINTTALK 3
#define HUD_PRINTCENTER 4

//Network Message Dest
#define MSG_BROADCAST 0
#define MSG_ONE 1
#define MSG_ALL 2
#define MSG_INIT 3
#define MSG_PVS 4
#define MSG_PAS 5
#define MSG_PVS_R 6
#define MSG_PAS_R 7
#define MSG_ONE_UNRELIABLE 8
#define MSG_SPEC 9

//Network Message Type
#define SVC_TEMPENTITY 23
#define SVC_INTERMISSION 30
#define SVC_STUFFTEXT 9
#define SelAmmo 64
#define CurWeapon 65
#define Geiger 66
#define Flashlight 67
#define FlashBat 68
#define Health 69
#define Damage 70
#define Battery 71
#define Train 72
#define HudText 73
#define SayText 74
#define TextMsg 75
#define WeaponList 76
#define CustWeapon 77
#define ResetHUD 78
#define InitHUD 79
#define CdAudio 80
#define GameTitle 81
#define DeathMsg 82
#define ScoreInfo 83
#define TeamInfo 84
#define TeamScore 85
#define GameMode 86
#define MOTD 87
#define AmmoPickup 88
#define WeapPickup 89
#define ItemPickup 90
#define HideHUD 91
#define SetFOV 92
#define ShowMenu 93
#define ScreenShake 94
#define ScreenFade 95
#define AmmoX 96
#define Gib 97
#define Spectator 98
#define TE_CUSTOM 99
#define Speaksent 100
#define TimeEnd 101
#define MapList 102
#define CbElec 103
#define EndVote 104
#define VoteMenu 105
#define NextMap 106
#define StartSound 107
#define SoundList 108
#define ToxicCloud 109
#define ShkFlash 110
#define CreateBlood 111
#define GargSplash 112
#define SporeTrail 113
#define TracerDecal 114
#define SRDetonate 115
#define SRPrimed 116
#define SRPrimedOff 117
#define RampSprite 118
#define ShieldRic 119
#define Playlist 120
#define VGUIMenu 121
#define ServerName 122
#define TeamNames 123
#define ServerVer 124
#define ServerBuild 125
#define WeatherFX 126
#define CameraMouse 127
#define Fog 128
#define PrtlUpdt 129
#define ASScriptName 130
#define PrintKB 131
#define InvAdd 132
#define InvRemove 133
#define Concuss 134
#define ViewMode 135
#define Flamethwr 136
#define ClassicMode 137
#define WeaponSpr 138
#define ToggleElem 139
#define CustSpr 140
#define NumDisplay 141
#define UpdateNum 142
#define TimeDisplay 144
#define UpdateTime 144
#define VModelPos 145

typedef struct link_s
{
	struct link_s* prev, * next;
} link_t;

typedef struct edict_s
{
	bool	free;
	int			serialnumber;
	link_t		area;				// linked to a division node or leaf

	int			headnode;			// -1 to use normal leaf check
	int			num_leafs;
	short		leafnums[MAX_ENT_LEAFS];

	float		freetime;			// sv.time when the object was freed

	void* pvPrivateData;		// Alloced and freed by engine, used by DLLs

	struct entvars_s* vars;					// C exported fields from progs

	// other fields from progs come immediately after
} edict_t;

struct vec3_t
{
	float x;
	float y;
	float z;
};

// entvars_s
struct entvars_s
{
	string	classname;
	string	globalname;

	vec3_t		origin;
	vec3_t		oldorigin;
	vec3_t		velocity;
	vec3_t		basevelocity;
	vec3_t      clbasevelocity;  // Base velocity that was passed in to server physics so 
	//  client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
	vec3_t		movedir;

	vec3_t		angles;			// Model angles
	vec3_t		avelocity;		// angle velocity (degrees per second)
	vec3_t		punchangle;		// auto-decaying view angle adjustment
	vec3_t		v_angle;		// Viewing angle (player only)

	// For parametric entities
	vec3_t		endpos;
	vec3_t		startpos;
	float		impacttime;
	float		starttime;

	int			fixangle;		// 0:nothing, 1:force view angles, 2:add avelocity
	float		idealpitch;
	float		pitch_speed;
	float		ideal_yaw;
	float		yaw_speed;

	int			modelindex;
	string	model;

	int			viewmodel;		// player's viewmodel
	int			weaponmodel;	// what other players see

	vec3_t		absmin;		// BB max translated to world coord
	vec3_t		absmax;		// BB max translated to world coord
	vec3_t		mins;		// local BB min
	vec3_t		maxs;		// local BB max
	vec3_t		size;		// maxs - mins

	float		ltime;
	float		nextthink;

	int			movetype;
	int			solid;

	int			skin;
	int			body;			// sub-model selection for studiomodels
	int 		effects;

	float		gravity;		// % of "normal" gravity
	float		friction;		// inverse elasticity of MOVETYPE_BOUNCE

	int			light_level;

	int			sequence;		// animation sequence
	int			gaitsequence;	// movement animation sequence for player (0 for none)
	float		frame;			// % playback position in animation sequences (0..255)
	float		animtime;		// world time when frame was set
	float		framerate;		// animation playback rate (-8x to 8x)
	BYTE		controller[4];	// bone controller setting (0..255)
	BYTE		blending[2];	// blending amount between sub-sequences (0..255)

	float		scale;			// sprite rendering scale (0..255)

	int			rendermode;
	float		renderamt;
	vec3_t		rendercolor;
	int			renderfx;

	float		health;
	float		frags;
	int			weapons;  // bit mask for available weapons
	float		takedamage;

	int			deadflag;
	vec3_t		view_ofs;	// eye position

	int			button;
	int			impulse;

	edict_t* chain;			// Entity pointer when linked into a linked list
	edict_t* dmg_inflictor;
	edict_t* enemy;
	edict_t* aiment;		// entity pointer when MOVETYPE_FOLLOW
	edict_t* owner;
	edict_t* groundentity;

	int			spawnflags;
	int			flags;

	int			colormap;		// lowbyte topcolor, highbyte bottomcolor
	int			team;

	float		max_health;
	float		teleport_time;
	float		armortype;
	float		armorvalue;
	int			waterlevel;
	int			watertype;

	string	target;
	string	targetname;
	string	netname;
	string	message;

	float		dmg_take;
	float		dmg_save;
	float		dmg;
	float		dmgtime;

	string	noise;
	string	noise1;
	string	noise2;
	string	noise3;

	float		speed;
	float		air_finished;
	float		pain_finished;
	float		radsuit_finished;

	edict_t* pContainingEntity;

	int			playerclass;
	float		maxspeed;

	float		fov;
	int			weaponanim;

	int			pushmsec;

	int			bInDuck;
	int			flTimeStepSound;
	int			flSwimTime;
	int			flDuckTime;
	int			iStepLeft;
	float		flFallVelocity;

	int			gamestate;

	int			oldbuttons;

	int			groupinfo;

	// For mods
	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	vec3_t		vuser1;
	vec3_t		vuser2;
	vec3_t		vuser3;
	vec3_t		vuser4;
	edict_t* euser1;
	edict_t* euser2;
	edict_t* euser3;
	edict_t* euser4;
};

#define	FCVAR_ARCHIVE		(1<<0)	// set to cause it to be saved to vars.rc
#define	FCVAR_USERINFO		(1<<1)	// changes the client's info string
#define	FCVAR_SERVER		(1<<2)	// notifies players when changed
#define FCVAR_EXTDLL		(1<<3)	// defined by external DLL
#define FCVAR_CLIENTDLL     (1<<4)  // defined by the client dll
#define FCVAR_PROTECTED     (1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY        (1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_PRINTABLEONLY (1<<7)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED		(1<<8)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NOEXTRAWHITEPACE	(1<<9)  // strip trailing/leading white space from this cvar

typedef struct cvar_s
{
	const char* name;
	const char* string;
	int		flags;
	float	value;
	struct cvar_s* next;
} cvar_t;

// Engine hands this to DLLs for functionality callbacks
typedef struct enginefuncs_s
{
	int			(*pfnPrecacheModel)			(const char* s);
	int			(*pfnPrecacheSound)			(const char* s);
	void		(*pfnSetModel)				(edict_t* e, const char* m);
	int			(*pfnModelIndex)			(const char* m);
	int			(*pfnModelFrames)			(int modelIndex);
	void		(*pfnSetSize)				(edict_t* e, const float* rgflMin, const float* rgflMax);
	void		(*pfnChangeLevel)			(const char* s1, const char* s2);
	void		(*pfnGetSpawnParms)			(edict_t* ent);
	void		(*pfnSaveSpawnParms)		(edict_t* ent);
	float		(*pfnVecToYaw)				(const float* rgflVector);
	void		(*pfnVecToAngles)			(const float* rgflVectorIn, float* rgflVectorOut);
	void		(*pfnMoveToOrigin)			(edict_t* ent, const float* pflGoal, float dist, int iMoveType);
	void		(*pfnChangeYaw)				(edict_t* ent);
	void		(*pfnChangePitch)			(edict_t* ent);
	edict_t* (*pfnFindEntityByString)	(edict_t* pEdictStartSearchAfter, const char* pszField, const char* pszValue);
	int			(*pfnGetEntityIllum)		(edict_t* pEnt);
	edict_t* (*pfnFindEntityInSphere)	(edict_t* pEdictStartSearchAfter, const float* org, float rad);
	edict_t* (*pfnFindClientInPVS)		(edict_t* pEdict);
	edict_t* (*pfnEntitiesInPVS)			(edict_t* pplayer);
	void		(*pfnMakeVectors)			(const float* rgflVector);
	void		(*pfnAngleVectors)			(const float* rgflVector, float* forward, float* right, float* up);
	edict_t* (*pfnCreateEntity)			(void);
	void		(*pfnRemoveEntity)			(edict_t* e);
	edict_t* (*pfnCreateNamedEntity)		(int className);
	void		(*pfnMakeStatic)			(edict_t* ent);
	int			(*pfnEntIsOnFloor)			(edict_t* e);
	int			(*pfnDropToFloor)			(edict_t* e);
	int			(*pfnWalkMove)				(edict_t* ent, float yaw, float dist, int iMode);
	void		(*pfnSetOrigin)				(edict_t* e, const float* rgflOrigin);
	void		(*pfnEmitSound)				(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
	void		(*pfnEmitAmbientSound)		(edict_t* entity, float* pos, const char* samp, float vol, float attenuation, int fFlags, int pitch);
	void		(*pfnTraceLine)				(const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, void* ptr);
	void		(*pfnTraceToss)				(edict_t* pent, edict_t* pentToIgnore, void* ptr);
	int			(*pfnTraceMonsterHull)		(edict_t* pEdict, const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, void* ptr);
	void		(*pfnTraceHull)				(const float* v1, const float* v2, int fNoMonsters, int hullNumber, edict_t* pentToSkip, void* ptr);
	void		(*pfnTraceModel)			(const float* v1, const float* v2, int hullNumber, edict_t* pent, void* ptr);
	const char* (*pfnTraceTexture)			(edict_t* pTextureEntity, const float* v1, const float* v2);
	void		(*pfnTraceSphere)			(const float* v1, const float* v2, int fNoMonsters, float radius, edict_t* pentToSkip, void* ptr);
	void		(*pfnGetAimVector)			(edict_t* ent, float speed, float* rgflReturn);
	void		(*pfnServerCommand)			(const char* str);
	void		(*pfnServerExecute)			(void);
	void		(*pfnClientCommand)			(edict_t* pEdict, const char* szFmt, ...);
	void		(*pfnParticleEffect)		(const float* org, const float* dir, float color, float count);
	void		(*pfnLightStyle)			(int style, const char* val);
	int			(*pfnDecalIndex)			(const char* name);
	int			(*pfnPointContents)			(const float* rgflVector);
	void		(*pfnMessageBegin)			(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
	void		(*pfnMessageEnd)			(void);
	void		(*pfnWriteByte)				(int iValue);
	void		(*pfnWriteChar)				(int iValue);
	void		(*pfnWriteShort)			(int iValue);
	void		(*pfnWriteLong)				(int iValue);
	void		(*pfnWriteAngle)			(float flValue);
	void		(*pfnWriteCoord)			(float flValue);
	void		(*pfnWriteString)			(const char* sz);
	void		(*pfnWriteEntity)			(int iValue);
	void		(*pfnCVarRegister)			(cvar_t* pCvar);
	float		(*pfnCVarGetFloat)			(const char* szVarName);
	const char* (*pfnCVarGetString)			(const char* szVarName);
	void		(*pfnCVarSetFloat)			(const char* szVarName, float flValue);
	void		(*pfnCVarSetString)			(const char* szVarName, const char* szValue);
	void		(*pfnAlertMessage)			(int atype, const char* szFmt, ...);
	void		(*pfnEngineFprintf)			(void* pfile, const char* szFmt, ...);
	void* (*pfnPvAllocEntPrivateData)	(edict_t* pEdict, UINT32 cb);
	void* (*pfnPvEntPrivateData)		(edict_t* pEdict);
	void		(*pfnFreeEntPrivateData)	(edict_t* pEdict);
	const char* (*pfnSzFromIndex)			(int iString);
	int			(*pfnAllocString)			(const char* szValue);
	struct entvars_s* (*pfnGetVarsOfEnt)			(edict_t* pEdict);
	edict_t* (*pfnPEntityOfEntOffset)	(int iEntOffset);
	int			(*pfnEntOffsetOfPEntity)	(const edict_t* pEdict);
	int			(*pfnIndexOfEdict)			(const edict_t* pEdict);
	edict_t* (*pfnPEntityOfEntIndex)		(int iEntIndex);
	edict_t* (*pfnFindEntityByVars)		(struct entvars_s* pvars);
	void* (*pfnGetModelPtr)			(edict_t* pEdict);
	int			(*pfnRegUserMsg)			(const char* pszName, int iSize);
	void		(*pfnAnimationAutomove)		(const edict_t* pEdict, float flTime);
	void		(*pfnGetBonePosition)		(const edict_t* pEdict, int iBone, float* rgflOrigin, float* rgflAngles);
	UINT32(*pfnFunctionFromName)	(const char* pName);
	const char* (*pfnNameForFunction)		(UINT32 function);
	void		(*pfnClientPrintf)			(edict_t* pEdict, int ptype, const char* szMsg); // JOHN: engine callbacks so game DLL can print messages to individual clients
	void		(*pfnServerPrint)			(const char* szMsg);
	const char* (*pfnCmd_Args)				(void);		// these 3 added 
	const char* (*pfnCmd_Argv)				(int argc);	// so game DLL can easily 
	int			(*pfnCmd_Argc)				(void);		// access client 'cmd' strings
	void		(*pfnGetAttachment)			(const edict_t* pEdict, int iAttachment, float* rgflOrigin, float* rgflAngles);
	void		(*pfnCRC32_Init)			(UINT32* pulCRC);
	void        (*pfnCRC32_ProcessBuffer)   (UINT32* pulCRC, void* p, int len);
	void		(*pfnCRC32_ProcessByte)     (UINT32* pulCRC, unsigned char ch);
	UINT32(*pfnCRC32_Final)			(UINT32 pulCRC);
	UINT32(*pfnRandomLong)			(UINT32  lLow, UINT32  lHigh);
	float		(*pfnRandomFloat)			(float flLow, float flHigh);
	void		(*pfnSetView)				(const edict_t* pClient, const edict_t* pViewent);
	float		(*pfnTime)					(void);
	void		(*pfnCrosshairAngle)		(const edict_t* pClient, float pitch, float yaw);
	BYTE* (*pfnLoadFileForMe)         (const char* filename, int* pLength);
	void        (*pfnFreeFile)              (void* buffer);
	void        (*pfnEndSection)            (const char* pszSectionName); // trigger_endsection
	int 		(*pfnCompareFileTime)       (char* filename1, char* filename2, int* iCompare);
	void        (*pfnGetGameDir)            (char* szGetGameDir);
	void		(*pfnCvar_RegisterVariable) (cvar_t* variable);
	void        (*pfnFadeClientVolume)      (const edict_t* pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	void        (*pfnSetClientMaxspeed)     (edict_t* pEdict, float fNewMaxspeed);
	edict_t* (*pfnCreateFakeClient)		(const char* netname);	// returns NULL if fake client can't be created
	void		(*pfnRunPlayerMove)			(edict_t* fakeclient, const float* viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, BYTE impulse, BYTE msec);
	int			(*pfnNumberOfEntities)		(void);
	char* (*pfnGetInfoKeyBuffer)		(edict_t* e);	// passing in NULL gets the serverinfo
	char* (*pfnInfoKeyValue)			(char* infobuffer, const char* key);
	void		(*pfnSetKeyValue)			(char* infobuffer, const char* key, const char* value);
	void		(*pfnSetClientKeyValue)		(int clientIndex, char* infobuffer, const char* key, const char* value);
	int			(*pfnIsMapValid)			(const char* filename);
	void		(*pfnStaticDecal)			(const float* origin, int decalIndex, int entityIndex, int modelIndex);
	int			(*pfnPrecacheGeneric)		(const char* s);
	int			(*pfnGetPlayerUserId)		(edict_t* e); // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	void		(*pfnBuildSoundMsg)			(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
	int			(*pfnIsDedicatedServer)		(void);// is this a dedicated server?
	cvar_t* (*pfnCVarGetPointer)		(const char* szVarName);
	unsigned int (*pfnGetPlayerWONId)		(edict_t* e); // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

	// YWB 8/1/99 TFF Physics additions
	void		(*pfnInfo_RemoveKey)		(char* s, const char* key);
	const char* (*pfnGetPhysicsKeyValue)	(const edict_t* pClient, const char* key);
	void		(*pfnSetPhysicsKeyValue)	(const edict_t* pClient, const char* key, const char* value);
	const char* (*pfnGetPhysicsInfoString)	(const edict_t* pClient);
	unsigned short (*pfnPrecacheEvent)		(int type, const char* psz);
	void		(*pfnPlaybackEvent)			(int flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);

	unsigned char* (*pfnSetFatPVS)			(float* org);
	unsigned char* (*pfnSetFatPAS)			(float* org);

	int			(*pfnCheckVisibility)		(edict_t* entity, unsigned char* pset);

	void		(*pfnDeltaSetField)			(struct delta_s* pFields, const char* fieldname);
	void		(*pfnDeltaUnsetField)		(struct delta_s* pFields, const char* fieldname);
	void		(*pfnDeltaAddEncoder)		(const char* name, void (*conditionalencode)(struct delta_s* pFields, const unsigned char* from, const unsigned char* to));
	int			(*pfnGetCurrentPlayer)		(void);
	int			(*pfnCanSkipPlayer)			(const edict_t* player);
	int			(*pfnDeltaFindField)		(struct delta_s* pFields, const char* fieldname);
	void		(*pfnDeltaSetFieldByIndex)	(struct delta_s* pFields, int fieldNumber);
	void		(*pfnDeltaUnsetFieldByIndex)(struct delta_s* pFields, int fieldNumber);

	void		(*pfnSetGroupMask)			(int mask, int op);

	int			(*pfnCreateInstancedBaseline) (int classname, struct entity_state_s* baseline);
	void		(*pfnCvar_DirectSet)		(struct cvar_s* var, const char* value);

	// Forces the client and server to be running with the same version of the specified file
	//  ( e.g., a player model ).
	// Calling this has no effect in single player
	void		(*pfnForceUnmodified)		(int type, float* mins, float* maxs, const char* filename);

	void		(*pfnGetPlayerStats)		(const edict_t* pClient, int* ping, int* packet_loss);

	void		(*pfnAddServerCommand)		(const char* cmd_name, void (*function) (void));

	// For voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	bool(*pfnVoice_GetClientListening)(int iReceiver, int iSender);
	bool(*pfnVoice_SetClientListening)(int iReceiver, int iSender, bool bListen);

	const char* (*pfnGetPlayerAuthId)		(edict_t* e);

	// PSV: Added for CZ training map
//	const char *(*pfnKeyNameForBinding)					( const char* pBinding );

	void* (*pfnSequenceGet)				(const char* fileName, const char* entryName);
	void* (*pfnSequencePickSentence)		(const char* groupName, int pickMethod, int* picked);

	// LH: Give access to filesize via filesystem
	int			(*pfnGetFileSize)						(const char* filename);

	unsigned int (*pfnGetApproxWavePlayLen)				(const char* filepath);
	// MDC: Added for CZ career-mode
	int			(*pfnIsCareerMatch)						(void);

	// BGC: return the number of characters of the localized string referenced by using "label"
	int			(*pfnGetLocalizedStringLength)			(const char* label);

	// BGC: added to facilitate persistent storage of tutor message decay values for
	// different career game profiles.  Also needs to persist regardless of mp.dll being
	// destroyed and recreated.
	void		(*pfnRegisterTutorMessageShown)			(int mid);
	int			(*pfnGetTimesTutorMessageShown)			(int mid);
	void		(*pfnProcessTutorMessageDecayBuffer)	(int* buffer, int bufferLength);
	void		(*pfnConstructTutorMessageDecayBuffer)	(int* buffer, int bufferLength);
	void		(*pfnResetTutorMessageDecayData)		(void);

	// Added 2005/08/11 (no SDK update):
	void(*pfnQueryClientCvarValue)		(const edict_t* player, const char* cvarName);

	// Added 2005/11/21 (no SDK update):
	void(*pfnQueryClientCvarValue2)		(const edict_t* player, const char* cvarName, int requestID);

	// Added 2009/06/19 (no SDK update):
	int(*pfnEngCheckParm)				(const char* pchCmdLineToken, char** ppnext);
} enginefuncs_t;