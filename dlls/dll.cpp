// Based on:
//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// dll.cpp
//


#include "extdll.h"
#include "enginecallback.h"
#include "dllapi.h"
#include "meta_api.h"
#include "h_export.h"
#include "cbase.h"
#include "shake.h"
#include "../Python/Include/Python.h"

extern bool PyClientCommand(edict_t *, const char *, const char *);
extern int REG_USER_MSG_ID(const char *, int);
#undef REG_USER_MSG
#define REG_USER_MSG(a, b) REG_USER_MSG_ID(a, b);

extern int debug_engine;
extern bool speechSynthesis;
//extern ChatList chatGotKilled, chatKilledPlayer, chatGotWeapon, chatReplyUnknown;
extern GETENTITYAPI other_GetEntityAPI;
extern GETNEWDLLFUNCTIONS other_GetNewDLLFunctions;
static char g_argv[256];

//#include "hl_game.h"
//extern HL_Game game;

int numberOfClients = 0;
edict_t *playerEnt = 0;
edict_t *clients[32];
DLL_FUNCTIONS other_gFunctionTable;
DLL_GLOBAL const Vector g_vecZero = Vector(0,0,0);
int isFakeClientCommand = 0;
int fake_arg_count;
float bot_check_time = 10.0;    // will be set to correct value when client connects
int min_bots = -1;                  
bool g_GameRules = FALSE;       
int welcome_index = -1; 
int wpSpriteTexture, wpBeamTexture, wpSprite2Texture;
int g_hldm_mod = 0;
bool gearbox_ctf = false;
char ag_gamemode[8] = {};
static FILE *fp;

int giPrecacheGrunt = 0;
int gmsgShake = 0;
int gmsgFade = 0;
int gmsgSelAmmo = 0;
int gmsgFlashlight = 0;
int gmsgFlashBattery = 0;
int gmsgResetHUD = 0;
int gmsgInitHUD = 0;
int gmsgShowGameTitle = 0;
int gmsgCurWeapon = 0;
int gmsgHealth = 0;
int gmsgDamage = 0;
int gmsgBattery = 0;
int gmsgTrain = 0;
int gmsgLogo = 0;
int gmsgWeaponList = 0;
int gmsgAmmoX = 0;
int gmsgHudText = 0;
int gmsgDeathMsg = 0;
int gmsgScoreInfo = 0;
int gmsgTeamInfo = 0;
int gmsgTeamScore = 0;
int gmsgGameMode = 0;
int gmsgMOTD = 0;
int gmsgServerName = 0;
int gmsgAmmoPickup = 0;
int gmsgWeapPickup = 0;
int gmsgItemPickup = 0;
int gmsgHideWeapon = 0;
int gmsgSetCurWeap = 0;
int gmsgSayText = 0;
int gmsgTextMsg = 0;
int gmsgSetFOV = 0;
int gmsgShowMenu = 0;
int gmsgGeigerRange = 0;
int gmsgTeamNames = 0;
int gmsgBhopcap = 0;

int gmsgStatusText = 0;
int gmsgStatusValue = 0;

void LinkUserMessages( void )
{
    // Already taken care of?
    if( gmsgSelAmmo )
    {
        return;
    }

    gmsgSelAmmo = REG_USER_MSG( "SelAmmo", sizeof(SelAmmo) );
    gmsgCurWeapon = REG_USER_MSG( "CurWeapon", 3 );
    gmsgGeigerRange = REG_USER_MSG( "Geiger", 1 );
    gmsgFlashlight = REG_USER_MSG( "Flashlight", 2 );
    gmsgFlashBattery = REG_USER_MSG( "FlashBat", 1 );
    gmsgHealth = REG_USER_MSG( "Health", 1 );
    gmsgDamage = REG_USER_MSG( "Damage", 12 );
    gmsgBattery = REG_USER_MSG( "Battery", 2);
    gmsgTrain = REG_USER_MSG( "Train", 1 );
    //gmsgHudText = REG_USER_MSG( "HudTextPro", -1 );
    gmsgHudText = REG_USER_MSG( "HudText", -1 ); // we don't use the message but 3rd party addons may!
    gmsgSayText = REG_USER_MSG( "SayText", -1 );
    gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );
    gmsgWeaponList = REG_USER_MSG( "WeaponList", -1 );
    gmsgResetHUD = REG_USER_MSG( "ResetHUD", 1 );		// called every respawn
    gmsgInitHUD = REG_USER_MSG( "InitHUD", 0 );		// called every time a new player joins the server
    gmsgShowGameTitle = REG_USER_MSG( "GameTitle", 1 );
    gmsgDeathMsg = REG_USER_MSG( "DeathMsg", -1 );
    gmsgScoreInfo = REG_USER_MSG( "ScoreInfo", 9 );
    gmsgTeamInfo = REG_USER_MSG( "TeamInfo", -1 );  // sets the name of a player's team
    gmsgTeamScore = REG_USER_MSG( "TeamScore", -1 );  // sets the score of a team on the scoreboard
    gmsgGameMode = REG_USER_MSG( "GameMode", 1 );
    gmsgMOTD = REG_USER_MSG( "MOTD", -1 );
    gmsgServerName = REG_USER_MSG( "ServerName", -1 );
    gmsgAmmoPickup = REG_USER_MSG( "AmmoPickup", 2 );
    gmsgWeapPickup = REG_USER_MSG( "WeapPickup", 1 );
    gmsgItemPickup = REG_USER_MSG( "ItemPickup", -1 );
    gmsgHideWeapon = REG_USER_MSG( "HideWeapon", 1 );
    gmsgSetFOV = REG_USER_MSG( "SetFOV", 1 );
    gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );
    gmsgShake = REG_USER_MSG( "ScreenShake", sizeof(ScreenShake) );
    gmsgFade = REG_USER_MSG( "ScreenFade", sizeof(ScreenFade) );
    gmsgAmmoX = REG_USER_MSG( "AmmoX", 2 );
    gmsgTeamNames = REG_USER_MSG( "TeamNames", -1 );
    gmsgBhopcap = REG_USER_MSG( "Bhopcap", 1 );

    gmsgStatusText = REG_USER_MSG( "StatusText", -1 );
    gmsgStatusValue = REG_USER_MSG( "StatusValue", 3 );
}

void ClientCommand (edict_t *pEntity)
{
    const char *pcmd = Cmd_Argv(0);
    const char *args = Cmd_Args();

    if(PyClientCommand(pEntity, pcmd, args))
        return;

    (*other_gFunctionTable.pfnClientCommand)(pEntity);
}

void GameDLLInit( void )
{
    (*other_gFunctionTable.pfnGameInit)();
    LinkUserMessages();
}

int DispatchSpawn( edict_t *pent )
{
   if (gpGlobals->deathmatch)
   {
      char *pClassname = (char *)STRING(pent->v.classname);
#ifdef _DEBUG
      if (debug_engine) {
         fp = UTIL_OpenDebugLog();
         fprintf(fp, "%f: DispatchSpawn: %s\n",worldTime(), pClassname );
         if (pent->v.model != 0)
            fprintf(fp, " model=%s\n",STRING(pent->v.model));
         fclose(fp);
      }
#endif
      if (strcmp(pClassname, "worldspawn") == 0)
      {
         // do level initialization stuff here...
/*
          if (speechSynthesis) {
              // precache samples
                int i;
                for (i=0; i<chatGotKilled.size(); i++ ) 
                    PRECACHE_SOUND( chatGotKilled[i].text );
                for (i=0; i<chatKilledPlayer.size(); i++ ) 
                    PRECACHE_SOUND( chatKilledPlayer[i].text );
                for (i=0; i<chatGotWeapon.size(); i++ ) 
                    PRECACHE_SOUND( chatGotWeapon[i].text );
                for (i=0; i<chatReplyUnknown.size(); i++ ) 
                    PRECACHE_SOUND( chatReplyUnknown[i].text );
          }
*/
        gearbox_ctf = false;

           PRECACHE_SOUND("weapons/xbow_hit1.wav");      // waypoint add
           PRECACHE_SOUND("weapons/mine_activate.wav");  // waypoint delete
           PRECACHE_SOUND("common/wpn_hudoff.wav");      // path add/delete start
           PRECACHE_SOUND("common/wpn_hudon.wav");       // path add/delete done
           PRECACHE_SOUND("common/wpn_moveselect.wav");  // path add/delete cancel
           PRECACHE_SOUND("common/wpn_denyselect.wav");  // path add/delete error
           wpBeamTexture = PRECACHE_MODEL( "sprites/lgtning.spr");
           wpSpriteTexture = PRECACHE_MODEL( "sprites/hotglow.spr");
           wpSprite2Texture = PRECACHE_MODEL( "sprites/laserdot.spr");

         g_GameRules = TRUE;
      }
//    else if (strcmp(pClassname, "env_sound") == 0) debugMsg( "DISPATCH env_sound\n" );
//    else if (strcmp(pClassname, "env_shake") == 0) debugMsg( "DISPATCH env_shake\n" );
//    else if (strcmp(pClassname, "env_explosion") == 0) debugMsg( "DISPATCH env_explosion\n" );
   }
    return (*other_gFunctionTable.pfnSpawn)(pent);
}

void DispatchThink( edict_t *pent )
{
    const char *classname = STRING(pent->v.classname);

    if(classname[0] == '@')
    {
        PyObject *builtins = PyImport_ImportModule("builtins");

        if(builtins) {
            Py_XINCREF(builtins);

            PyObject *RunDispatch = PyObject_GetAttrString(builtins, "RunDispatch");
            PyObject *args = Py_BuildValue("s((ii))", "think", ENTINDEX(pent), pent->serialnumber);
            Py_XDECREF(PyObject_CallObject(RunDispatch, args));

            Py_XINCREF(RunDispatch);
            Py_XDECREF(args);
        }

        return;
    }

   (*other_gFunctionTable.pfnThink)(pent);
}

void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
    const char *classname = STRING(pentUsed->v.classname);

    if(classname[0] == '@')
    {

        PyObject *builtins = PyImport_ImportModule("builtins");

        if(builtins) {
            Py_XINCREF(builtins);

            PyObject *RunDispatch = PyObject_GetAttrString(builtins, "RunDispatch");
            PyObject *args = Py_BuildValue("s((ii)(ii))", "use", ENTINDEX(pentUsed), pentUsed->serialnumber, ENTINDEX(pentOther), pentOther->serialnumber);
            Py_XDECREF(PyObject_CallObject(RunDispatch, args));

            Py_XINCREF(RunDispatch);
            Py_XDECREF(args);
        }

        return;
    }

   (*other_gFunctionTable.pfnUse)(pentUsed, pentOther);
}

void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
   const char *classname = STRING(pentTouched->v.classname);

   if(classname[0] == '@')
   {
       PyObject *builtins = PyImport_ImportModule("builtins");

       if(builtins) {
           Py_XINCREF(builtins);

           PyObject *RunDispatch = PyObject_GetAttrString(builtins, "RunDispatch");
           PyObject *args = Py_BuildValue("s((ii)(ii))", "touch", ENTINDEX(pentTouched), pentTouched->serialnumber, ENTINDEX(pentOther), pentOther->serialnumber);
           Py_XDECREF(PyObject_CallObject(RunDispatch, args));

           Py_XINCREF(RunDispatch);
           Py_XDECREF(args);
       }

       return;
   }

   (*other_gFunctionTable.pfnTouch)(pentTouched, pentOther);
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
    const char *classname = STRING(pentBlocked->v.classname);

    if(classname[0] == '@')
    {
        PyObject *builtins = PyImport_ImportModule("builtins");

        if(builtins) {
            Py_XINCREF(builtins);

            PyObject *RunDispatch = PyObject_GetAttrString(builtins, "RunDispatch");
            PyObject *args = Py_BuildValue("s((ii)(ii))", "block", ENTINDEX(pentBlocked), pentBlocked->serialnumber, ENTINDEX(pentOther), pentOther->serialnumber);
            Py_XDECREF(PyObject_CallObject(RunDispatch, args));

            Py_XINCREF(RunDispatch);
            Py_XDECREF(args);
        }

        return;
    }

   (*other_gFunctionTable.pfnBlocked)(pentBlocked, pentOther);
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
    (*other_gFunctionTable.pfnKeyValue)(pentKeyvalue, pkvd);
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnSave)(pent, pSaveData);
}

int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
   return (*other_gFunctionTable.pfnRestore)(pent, pSaveData, globalEntity);
}

void DispatchObjectCollsionBox( edict_t *pent )
{
    const char *classname = STRING(pent->v.classname);

    if(classname[0] == '@')
    {
        PyObject *builtins = PyImport_ImportModule("builtins");

        if(builtins) {
            Py_XINCREF(builtins);

            PyObject *RunDispatch = PyObject_GetAttrString(builtins, "RunDispatch");
            PyObject *args = Py_BuildValue("s((ii))", "collbox", ENTINDEX(pent), pent->serialnumber);
            PyObject_CallObject(RunDispatch, args);

            Py_XINCREF(RunDispatch);
            Py_XDECREF(args);
        }

        return;
    }

   (*other_gFunctionTable.pfnSetAbsBox)(pent);
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   (*other_gFunctionTable.pfnSaveWriteFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   (*other_gFunctionTable.pfnSaveReadFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnSaveGlobalState)(pSaveData);
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnRestoreGlobalState)(pSaveData);
}

void ResetGlobalState( void )
{
   (*other_gFunctionTable.pfnResetGlobalState)();
}

///////////////////////////////////////////////////////////////////////////////////
//
//  CLIENT HANDLING
//
///////////////////////////////////////////////////////////////////////////////////
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{ 
    bool connected;
#ifdef _DEBUG
    char buffer[256];
    sprintf( buffer, "%.f: ClientConnect: %s (%s)", worldTime(), STRING(pEntity->v.netname), pszName );
    debugFile( buffer );
#endif
    if (gpGlobals->deathmatch)
    {
#ifdef _DEBUG
        if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "ClientConnect: pent=%p name=%s\n",pEntity,pszName); fclose(fp); }
#endif
        // check if this client is the listen server client
        if (strcmp(pszAddress, "loopback") == 0)
        {
            // save the edict of the listen server client...
            playerEnt = pEntity;
        }
        // check if this is NOT a bot joining the server...
        if (strcmp(pszAddress, "127.0.0.1") != 0) {
            // don't try to add bots for 10 seconds, give client time to get added
            if (bot_check_time < gpGlobals->time + 10.0) bot_check_time = gpGlobals->time + 10.0;
        }
    }

    BOOL success = (*other_gFunctionTable.pfnClientConnect)(pEntity, pszName, pszAddress, szRejectReason);

    return success;
}


void ClientDisconnect( edict_t *pEntity )
{
    (*other_gFunctionTable.pfnClientDisconnect)(pEntity);
}

void ClientKill( edict_t *pEntity )
{
#ifdef _DEBUG
   if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "ClientKill: %p\n",pEntity); fclose(fp); }
#endif
   (*other_gFunctionTable.pfnClientKill)(pEntity);
}

void ClientPutInServer( edict_t *pEntity )
{
    (*other_gFunctionTable.pfnClientPutInServer)(pEntity);
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
#ifdef _DEBUG
   if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "ClientUserInfoChanged: pEntity=%p infobuffer=%s\n", pEntity, infobuffer); fclose(fp); }
#endif
   (*other_gFunctionTable.pfnClientUserInfoChanged)(pEntity, infobuffer);
}


void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
   (*other_gFunctionTable.pfnServerActivate)(pEdictList, edictCount, clientMax);
}

void ServerDeactivate( void )
{
#ifdef _DEBUG
    if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "ServerDeactivate\n"); fclose(fp); }
#endif
    (*other_gFunctionTable.pfnServerDeactivate)();
}


void PlayerPreThink( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnPlayerPreThink)(pEntity);
}


void PlayerPostThink( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnPlayerPostThink)(pEntity);
}


void ParmsNewLevel( void )
{
   (*other_gFunctionTable.pfnParmsNewLevel)();
}


void ParmsChangeLevel( void )
{
   (*other_gFunctionTable.pfnParmsChangeLevel)();
    LinkUserMessages();
}


const char *GetGameDescription( void )
{
   return (*other_gFunctionTable.pfnGetGameDescription)();
}


void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
#ifdef _DEBUG
   if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "PlayerCustomization: %p\n",pEntity); fclose(fp); }
#endif
   (*other_gFunctionTable.pfnPlayerCustomization)(pEntity, pCust);
}


void SpectatorConnect( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorConnect)(pEntity);
}


void SpectatorDisconnect( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorDisconnect)(pEntity);
}


void SpectatorThink( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorThink)(pEntity);
}


void Sys_Error( const char *error_string )
{
#ifdef _DEBUG
    if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "Sys_Error: %s\n", error_string); fclose(fp); }
#endif
    (*other_gFunctionTable.pfnSys_Error)(error_string);
}

void PM_Move ( struct playermove_s *ppmove, int server )
{
    (*other_gFunctionTable.pfnPM_Move)(ppmove, server);
}

void PM_Init ( struct playermove_s *ppmove )
{
   (*other_gFunctionTable.pfnPM_Init)(ppmove);
}


char PM_FindTextureType( char *name )
{
   return (*other_gFunctionTable.pfnPM_FindTextureType)(name);
}


void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
   (*other_gFunctionTable.pfnSetupVisibility)(pViewEntity, pClient, pvs, pas);
}


void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
   (*other_gFunctionTable.pfnUpdateClientData)(ent, sendweapons, cd);
}


int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
   return (*other_gFunctionTable.pfnAddToFullPack)(state, e, ent, host, hostflags, player, pSet);
}


void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
   (*other_gFunctionTable.pfnCreateBaseline)(player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs);
}


void RegisterEncoders( void )
{
   (*other_gFunctionTable.pfnRegisterEncoders)();
}


int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
   return (*other_gFunctionTable.pfnGetWeaponData)(player, info);
}


void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
#ifdef _DEBUG
    /*if (debug_engine) { 
        fp = UTIL_OpenDebugLog(); 
        fprintf(fp, "CmdStart: ed=%p, lms=%i, msec=%i, bts=%i, imp=%i, wps=%i\n",
            player, cmd->lerp_msec, cmd->msec, cmd->buttons, cmd->impulse, cmd->weaponselect ); 
        fclose(fp); 
    }*/
#endif
/*  short   lerp_msec;      // Interpolation time on client
    byte    msec;           // Duration in ms of command
    vec3_t  viewangles;     // Command view angles.

// intended velocities
    float   forwardmove;    // Forward velocity.
    float   sidemove;       // Sideways velocity.
    float   upmove;         // Upward velocity.
    byte    lightlevel;     // Light level at spot where we are standing.
    unsigned short  buttons;  // Attack buttons
    byte    impulse;          // Impulse command issued.
    byte    weaponselect;   // Current weapon id
*/
   (*other_gFunctionTable.pfnCmdStart)(player, cmd, random_seed);
}


void CmdEnd ( const edict_t *player )
{
#ifdef _DEBUG
    /*if (debug_engine) { 
        fp = UTIL_OpenDebugLog(); 
        fprintf(fp, "CmdEnd: ed=%p\n", player ); 
        fclose(fp); 
    }*/
#endif
    //debugMsg( "CmdEnd\n" );
   (*other_gFunctionTable.pfnCmdEnd)(player);
}


int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
   return (*other_gFunctionTable.pfnConnectionlessPacket)(net_from, args, response_buffer, response_buffer_size);
}


int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
   return (*other_gFunctionTable.pfnGetHullBounds)(hullnumber, mins, maxs);
}


void CreateInstancedBaselines( void )
{
   (*other_gFunctionTable.pfnCreateInstancedBaselines)();
}


int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
#ifdef _DEBUG
   if (debug_engine) { fp = UTIL_OpenDebugLog(); fprintf(fp, "InconsistentFile: %p filename=%s\n",player,filename); fclose(fp); }
#endif
   return (*other_gFunctionTable.pfnInconsistentFile)(player, filename, disconnect_message);
}


int AllowLagCompensation( void )
{
   return (*other_gFunctionTable.pfnAllowLagCompensation)();
}

void StartFrame()
{
    (*other_gFunctionTable.pfnStartFrame)();
}


DLL_FUNCTIONS gFunctionTable =
{
   GameDLLInit,               //pfnGameInit
   DispatchSpawn,             //pfnSpawn
   DispatchThink,             //pfnThink
   DispatchUse,               //pfnUse
   DispatchTouch,             //pfnTouch
   DispatchBlocked,           //pfnBlocked
   DispatchKeyValue,          //pfnKeyValue
   DispatchSave,              //pfnSave
   DispatchRestore,           //pfnRestore
   DispatchObjectCollsionBox, //pfnAbsBox

   SaveWriteFields,           //pfnSaveWriteFields
   SaveReadFields,            //pfnSaveReadFields

   SaveGlobalState,           //pfnSaveGlobalState
   RestoreGlobalState,        //pfnRestoreGlobalState
   ResetGlobalState,          //pfnResetGlobalState

   ClientConnect,             //pfnClientConnect
   ClientDisconnect,          //pfnClientDisconnect
   ClientKill,                //pfnClientKill
   ClientPutInServer,         //pfnClientPutInServer
   ClientCommand,             //pfnClientCommand
   ClientUserInfoChanged,     //pfnClientUserInfoChanged
   ServerActivate,            //pfnServerActivate
   ServerDeactivate,          //pfnServerDeactivate

   PlayerPreThink,            //pfnPlayerPreThink
   PlayerPostThink,           //pfnPlayerPostThink

   StartFrame,                //pfnStartFrame
   ParmsNewLevel,             //pfnParmsNewLevel
   ParmsChangeLevel,          //pfnParmsChangeLevel

   GetGameDescription,        //pfnGetGameDescription    Returns string describing current .dll game.
   PlayerCustomization,       //pfnPlayerCustomization   Notifies .dll of new customization for player.

   SpectatorConnect,          //pfnSpectatorConnect      Called when spectator joins server
   SpectatorDisconnect,       //pfnSpectatorDisconnect   Called when spectator leaves the server
   SpectatorThink,            //pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)

   Sys_Error,                 //pfnSys_Error          Called when engine has encountered an error

   PM_Move,                   //pfnPM_Move
   PM_Init,                   //pfnPM_Init            Server version of player movement initialization
   PM_FindTextureType,        //pfnPM_FindTextureType

   SetupVisibility,           //pfnSetupVisibility        Set up PVS and PAS for networking for this client
   UpdateClientData,          //pfnUpdateClientData       Set up data sent only to specific client
   AddToFullPack,             //pfnAddToFullPack
   CreateBaseline,            //pfnCreateBaseline        Tweak entity baseline for network encoding, allows setup of player baselines, too.
   RegisterEncoders,          //pfnRegisterEncoders      Callbacks for network encoding
   GetWeaponData,             //pfnGetWeaponData
   CmdStart,                  //pfnCmdStart
   CmdEnd,                    //pfnCmdEnd
   ConnectionlessPacket,      //pfnConnectionlessPacket
   GetHullBounds,             //pfnGetHullBounds
   CreateInstancedBaselines,  //pfnCreateInstancedBaselines
   InconsistentFile,          //pfnInconsistentFile
   AllowLagCompensation,      //pfnAllowLagCompensation
};

extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{

    // check if engine's pointer is valid and version is correct...
    if ( !pFunctionTable || interfaceVersion != INTERFACE_VERSION )
        return FALSE;

    // pass engine callback function table to engine...
    memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );

    // pass other DLLs engine callbacks to function table...
    if (!(*other_GetEntityAPI)(&other_gFunctionTable, INTERFACE_VERSION))
    {
        return FALSE;  // error initializing function table!!!
    }

   return TRUE;
}

extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion ) 
{ 
    // pass other DLLs engine callbacks to function table... 
    if (!other_GetNewDLLFunctions) return FALSE;
    if (!(*other_GetNewDLLFunctions)(pFunctionTable, interfaceVersion)) 
    { 
        return FALSE;  // error initializing function table!!! 
    } 

    return TRUE; 
}

void FakeClientCommand(edict_t *pBot, const char *arg1, const char *arg2, const char *arg3)
{
   int length;

   isFakeClientCommand = 1;
   memset( g_argv, 0, sizeof(g_argv) );

   if ((arg1 == NULL) || (*arg1 == 0)) return;

   if ((arg2 == NULL) || (*arg2 == 0))
   {
      length = sprintf(&g_argv[0], "%s", arg1);
      fake_arg_count = 1;
   }
   else if ((arg3 == NULL) || (*arg3 == 0))
   {
      length = sprintf(&g_argv[0], "%s %s", arg1, arg2);
      fake_arg_count = 2;
   }
   else
   {
      length = sprintf(&g_argv[0], "%s %s %s", arg1, arg2, arg3);
      fake_arg_count = 3;
   }

   g_argv[length] = 0;  // null terminate just in case

   strcpy(&g_argv[64], arg1);

   if (arg2)
      strcpy(&g_argv[128], arg2);

   if (arg3)
      strcpy(&g_argv[192], arg3);

    // allow the MOD DLL to execute the ClientCommand...
   ClientCommand(pBot);

   isFakeClientCommand = 0;
}


const char *Cmd_Args( void )
{
   if (isFakeClientCommand)
      return &g_argv[0];

   else
    return (*g_engfuncs.pfnCmd_Args)();
}

const char *Cmd_Argv( int argc )
{
   if (isFakeClientCommand)
   {
      if (argc == 0)
        return &g_argv[64];

      else if (argc == 1)
        return &g_argv[128];
      else if (argc == 2)
        return &g_argv[192];
      else
        return "???";
   }
   else
       return (*g_engfuncs.pfnCmd_Argv)(argc);
}


int Cmd_Argc( void )
{
   if (isFakeClientCommand)
        return fake_arg_count;
   else
    return (*g_engfuncs.pfnCmd_Argc)();
}
