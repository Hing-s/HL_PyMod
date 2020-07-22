/***
*
*  Copyright (c) 1999, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id 
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*  All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// util.cpp
//

#include "extdll.h"
#include "h_export.h"
#include "shake.h"

extern int gmsgTextMsg;
extern int gmsgSayText;
extern int gmsgShowMenu;

static unsigned short FixedUnsigned16( float value, float scale )
{
    int output;

    output = (int)( value * scale );
    if( output < 0 )
        output = 0;
    if( output > 0xFFFF )
        output = 0xFFFF;

    return (unsigned short)output;
}

static short FixedSigned16( float value, float scale )
{
    int output;

    output = (int)( value * scale );

    if( output > 32767 )
        output = 32767;

    if( output < -32768 )
        output = -32768;

    return (short)output;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  EHANDLE FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////////////

edict_t * EHANDLE::Get( void )
{ 
    if (m_pent)
    {
        if (m_pent->serialnumber == m_serialnumber) 
            return m_pent; 
        else
            return NULL;
    }
    return NULL; 
};

edict_t * EHANDLE::Set( edict_t *pent ) 
{ 
    m_pent = pent;  
    if (pent) 
        m_serialnumber = m_pent->serialnumber; 
    return pent; 
};


EHANDLE :: operator CBaseEntity *() 
{ 
    return (CBaseEntity *)GET_PRIVATE( Get( ) ); 
};


CBaseEntity * EHANDLE :: operator = (CBaseEntity *pEntity)
{
    if (pEntity)
    {
        m_pent = ENT( pEntity->pev );
        if (m_pent)
            m_serialnumber = m_pent->serialnumber;
    }
    else
    {
        m_pent = NULL;
        m_serialnumber = 0;
    }
    return pEntity;
}

EHANDLE :: operator int ()
{
    return Get() != NULL;
}

CBaseEntity * EHANDLE :: operator -> ()
{
    return (CBaseEntity *)GET_PRIVATE( Get( ) ); 
}




Vector UTIL_VecToAngles( const Vector &vec )
{
   float rgflVecOut[3];
   VEC_TO_ANGLES(vec, rgflVecOut);
   return Vector(rgflVecOut);
}

Vector UTIL_GetAimVector( edict_t *pent, float flSpeed )
{
    Vector tmp;
    GET_AIM_VECTOR( pent, flSpeed, tmp );
    UTIL_MakeVectors(tmp + pent->v.v_angle + pent->v.punchangle);
    return tmp;
}


Vector UTIL_GetRight( const Vector &vec )
{
    Vector angles = UTIL_VecToAngles( vec );
    MAKE_VECTORS( angles );
    return gpGlobals->v_right;
}


// Overloaded to add IGNORE_GLASS
void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
    TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}


void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
   TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), pentIgnore, ptr );
}


void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
    TRACE_HULL( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), hullNumber, pentIgnore, ptr );
}


void UTIL_MakeVectors( const Vector &vecAngles )
{
   MAKE_VECTORS( vecAngles );
}


CBaseEntity* UTIL_FindEntityByString( CBaseEntity *pStartEntity, const char *szKeyword, const char *szValue )
{
    edict_t *pentEntity;

    if (pStartEntity)
        pentEntity = pStartEntity->edict();
    else
        pentEntity = NULL;

    pentEntity = FIND_ENTITY_BY_STRING( pentEntity, szKeyword, szValue );

    if (!FNullEnt(pentEntity))
        return CBaseEntity::Instance(pentEntity);
    return NULL;
}


CBaseEntity* UTIL_FindEntityByClassname( CBaseEntity *pStartEntity, const char *szName )
{
    return UTIL_FindEntityByString( pStartEntity, "classname", szName );
}


CBaseEntity* UTIL_FindEntityInSphere( CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius )
{
    edict_t *pentEntity;

    if (pStartEntity)
        pentEntity = pStartEntity->edict();
    else
        pentEntity = NULL;

    pentEntity = FIND_ENTITY_IN_SPHERE( pentEntity, vecCenter, flRadius);

    if (!FNullEnt(pentEntity))
        return CBaseEntity::Instance(pentEntity);
    return NULL;
}

CBaseEntity *UTIL_EntCreate( const char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner )
{
    edict_t	*pent;
    CBaseEntity *pEntity;

    pent = CREATE_NAMED_ENTITY( MAKE_STRING( szName ) );
    if( FNullEnt( pent ) )
    {
        ALERT ( at_console, "NULL Ent in Create!\n" );
        return NULL;
    }
    pEntity = CBaseEntity::Instance( pent );
    pEntity->pev->owner = pentOwner;
    pEntity->pev->origin = vecOrigin;
    pEntity->pev->angles = vecAngles;
    DispatchSpawn( pEntity->edict() );
    return pEntity;
}


CBaseEntity *UTIL_PlayerByIndex( int playerIndex )
{
    CBaseEntity *pPlayer = NULL;

    if ( playerIndex > 0 && playerIndex <= gpGlobals->maxClients ) {
        edict_t *pPlayerEdict = INDEXENT( playerIndex );
        if ( pPlayerEdict && !pPlayerEdict->free ) 
            pPlayer = CBaseEntity::Instance( pPlayerEdict );
    }   
    return pPlayer;
}


int UTIL_PointContents( const Vector &vec )
{
   return POINT_CONTENTS(vec);
}


void UTIL_SetSize( entvars_t *pev, const Vector &vecMin, const Vector &vecMax )
{
   SET_SIZE( ENT(pev), vecMin, vecMax );
}


void UTIL_SetOrigin( entvars_t *pev, const Vector &vecOrigin )
{
   SET_ORIGIN(ENT(pev), vecOrigin );
}


void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
   if (gmsgTextMsg == 0)
      gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, client );

   WRITE_BYTE( msg_dest );
   WRITE_STRING( msg_name );

   if ( param1 )
      WRITE_STRING( param1 );
   if ( param2 )
      WRITE_STRING( param2 );
   if ( param3 )
      WRITE_STRING( param3 );
   if ( param4 )
      WRITE_STRING( param4 );

   MESSAGE_END();
}

void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
   if (gmsgTextMsg == 0)
      gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );

   MESSAGE_BEGIN( MSG_ALL, gmsgTextMsg );
      WRITE_BYTE( msg_dest );
      WRITE_STRING( msg_name );

      if ( param1 )
         WRITE_STRING( param1 );
      if ( param2 )
         WRITE_STRING( param2 );
      if ( param3 )
         WRITE_STRING( param3 );
      if ( param4 )
         WRITE_STRING( param4 );

   MESSAGE_END();
}

void UTIL_SayText( const char *pText, edict_t *pEdict )
{
   if (gmsgSayText == 0)
      gmsgSayText = REG_USER_MSG( "SayText", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, pEdict );

   WRITE_BYTE( ENTINDEX(pEdict) );
   WRITE_STRING( pText );

   MESSAGE_END();
}

bool valveTeamPlayMode = false;
char valveTeamList[16][32] = { "","","","","","","","","","","","","","","","" };
int  valveTeamNumber = 0;
int mod_id = 0;


// return team number 0 through 3 based what MOD uses for team numbers
int UTIL_GetTeam(edict_t *pEntity)
{
    const char *infobuffer;
    char teamName[32];
    char modelName[32];
    int i;

    switch( mod_id ) 
    {
    case DMC_DLL:
    case VALVE_DLL:
    case HUNGER_DLL:
    case GEARBOX_DLL:
        infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );

        if(false)
        {
            strcpy( modelName, (g_engfuncs.pfnInfoKeyValue( infobuffer, "model" )) );
            if ((strcmp(modelName, "ctf_barney") == 0) ||
                (strcmp(modelName, "cl_suit") == 0) ||
                (strcmp(modelName, "ctf_gina") == 0) ||
                (strcmp(modelName, "ctf_gordon") == 0) ||
                (strcmp(modelName, "otis") == 0) ||
                (strcmp(modelName, "ctf_scientist") == 0))
            {
                return 0;
            }
            else if ((strcmp(modelName, "beret") == 0) ||
                (strcmp(modelName, "drill") == 0) ||
                (strcmp(modelName, "grunt") == 0) ||
                (strcmp(modelName, "recruit") == 0) ||
                (strcmp(modelName, "shephard") == 0) ||
                (strcmp(modelName, "tower") == 0))
            {
                return 1;
            }
        }
        else
        {
            strcpy( teamName, (g_engfuncs.pfnInfoKeyValue( infobuffer, "team" )) );
            for (i=0; i<valveTeamNumber; i++) 
                if (stricmp( teamName, valveTeamList[i] ) == 0) return i;
        }
       //debugMsg( "ERROR: Team not found!\n" );
        return 0;
    case AG_DLL:
    //case FRONTLINE_DLL:
    case TFC_DLL:
        return pEntity->v.team - 1;  // teams are 1-4 based
    
    case CSTRIKE_DLL:
        infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
        strcpy(modelName, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));
        
        if ((strcmp( modelName, "terror"   ) == 0) ||  // Phoenix Connektion
            (strcmp( modelName, "arab"     ) == 0) ||    // Old L337 Krew
            (strcmp( modelName, "leet") == 0) ||    // L337 Krew
            (strcmp( modelName, "arctic"   ) == 0) ||  // Artic Avenger
            (strcmp( modelName, "guerilla" ) == 0))  // Gorilla Warfare
        {
            return 0;
        }
        else if ((strcmp( modelName, "urban" ) == 0) ||  // Seal Team 6
                 (strcmp( modelName, "gsg9"  ) == 0) ||   // German GSG-9
                 (strcmp( modelName, "sas"   ) == 0) ||    // UK SAS
                 (strcmp( modelName, "gign"  ) == 0) ||   // French GIGN
                 (strcmp( modelName, "vip"   ) == 0))      // VIP
        {
            return 1;
        }
        
        //debugMsg( "UTIL_GetTeam: Unknown model = " );  debugMsg( modelName );  debugMsg( "\n" );
        return -1;  // return -1 if team is unknown
    
    default:
        int team = pEntity->v.team;  // assume all others are 0-3 based
        
        if ((team < 0) || (team > 3)) {
           //debugMsg( "UTIL_GetTeam: Unknown team code = %i\n", team ); 
            team = -1;
        }           
        return team;
    } 
}


int UTIL_GetBotIndex( edict_t *pEdict )
{
    return -1;
}


int* UTIL_GetBotPointer( edict_t *pEdict )
{ 
    return 0;
}


void UTIL_ScreenShake( const Vector &center, float amplitude, float frequency, float duration, float radius )
{
    int     i;
    float       localAmplitude;
    ScreenShake shake;

    shake.duration = FixedUnsigned16( duration, 1 << 12 );      // 4.12 fixed
    shake.frequency = FixedUnsigned16( frequency, 1 << 8 ); // 8.8 fixed

    for( i = 1; i <= gpGlobals->maxClients; i++ )
    {
        CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );

        if( !pPlayer || !( pPlayer->pev->flags & FL_ONGROUND ) )    // Don't shake if not onground
            continue;

        localAmplitude = 0;

        if( radius <= 0 )
            localAmplitude = amplitude;
        else
        {
            Vector delta = center - pPlayer->pev->origin;
            float distance = delta.Length();

            // Had to get rid of this falloff - it didn't work well
            if( distance < radius )
                localAmplitude = amplitude;//radius - distance;
        }
        if( localAmplitude )
        {
            shake.amplitude = FixedUnsigned16( localAmplitude, 1 << 12 );       // 4.12 fixed

            MESSAGE_BEGIN( MSG_ONE, gmsgShake, NULL, pPlayer->edict() );        // use the magic #1 for "one client"
                WRITE_SHORT( shake.amplitude );             // shake amount
                WRITE_SHORT( shake.duration );              // shake lasts this long
                WRITE_SHORT( shake.frequency );             // shake noise frequency
            MESSAGE_END();
        }
    }
}

void UTIL_ScreenShakeAll( const Vector &center, float amplitude, float frequency, float duration )
{
    UTIL_ScreenShake( center, amplitude, frequency, duration, 0 );
}

void UTIL_ScreenFadeBuild( ScreenFade &fade, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
    fade.duration = FixedUnsigned16( fadeTime, 1 << 12 );       // 4.12 fixed
    fade.holdTime = FixedUnsigned16( fadeHold, 1 << 12 );       // 4.12 fixed
    fade.r = (int)color.x;
    fade.g = (int)color.y;
    fade.b = (int)color.z;
    fade.a = alpha;
    fade.fadeFlags = flags;
}

void UTIL_ScreenFadeWrite( const ScreenFade &fade, CBaseEntity *pEntity )
{
    if( !pEntity || !pEntity->IsNetClient() )
        return;

    ALERT(at_console, "%d\n", gmsgFade);
    MESSAGE_BEGIN( MSG_ONE, gmsgFade, NULL, pEntity->edict() );     // use the magic #1 for "one client"
        WRITE_SHORT( fade.duration );       // fade lasts this long
        WRITE_SHORT( fade.holdTime );       // fade lasts this long
        WRITE_SHORT( fade.fadeFlags );      // fade type (in / out)
        WRITE_BYTE( fade.r );               // fade red
        WRITE_BYTE( fade.g );               // fade green
        WRITE_BYTE( fade.b );               // fade blue
        WRITE_BYTE( fade.a );               // fade blue
    MESSAGE_END();
}

void UTIL_ScreenFadeAll( const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
    int i;
    ScreenFade fade;

    UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );

    for( i = 1; i <= gpGlobals->maxClients; i++ )
    {
        CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );

        UTIL_ScreenFadeWrite( fade, pPlayer );
    }
}

void UTIL_ScreenFade( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
    ScreenFade fade;

    UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );
    UTIL_ScreenFadeWrite( fade, pEntity );
}

bool UTIL_ButtonTriggers( edict_t *button, edict_t *target )
{
    const char *targetName = STRING( target->v.targetname );
    const char *buttonTarget = STRING( button->v.target );
    if (strcmp( buttonTarget, targetName )==0) return true;
    // multimanager in between?
    edict_t *bTarget = FIND_ENTITY_BY_TARGETNAME( 0, buttonTarget );
    if (!bTarget) return false;
    if (strcmp( STRING( bTarget->v.classname ), "multi_manager" )==0) {
        CMultiManager *mm = (CMultiManager*)GET_PRIVATE( bTarget );
        // check all multimanager targets:
        for ( int i=0; i<mm->m_cTargets; i++ ) {
            if ( FStrEq(targetName, STRING(mm->m_iTargetName[i])) ) return true;
        }
    } 
    return false;
}


void UTIL_SelectItem(edict_t *pEdict, char *item_name)
{
   FakeClientCommand(pEdict, item_name, NULL, NULL);
}


void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, const char *pText )
{
   if (gmsgShowMenu == 0)
      gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgShowMenu, NULL, pEdict );

   WRITE_SHORT( slots );
   WRITE_CHAR( displaytime );
   WRITE_BYTE( needmore );
   WRITE_STRING( pText );

   MESSAGE_END();
}

#ifdef DEBUG

FILE *UTIL_OpenDebugLog( void )
{
    char logfile[64];
    FILE *fp;

    sprintf( logfile,"%s/addons/parabot/log/debug.txt", mod_name );
    fp = fopen( logfile, "a" );
    return fp;
}


    edict_t *DBG_EntOfVars( const entvars_t *pev )
    {
        if (pev->pContainingEntity != NULL) return pev->pContainingEntity;
        ALERT(at_console, "entvars_t pContainingEntity is NULL, calling into engine");
        edict_t* pent = (*g_engfuncs.pfnFindEntityByVars)((entvars_t*)pev);
        if (pent == NULL) ALERT(at_console, "DAMN!  Even the engine couldn't FindEntityByVars!");
        ((entvars_t *)pev)->pContainingEntity = pent;
        return pent;
    }


    void DBG_AssertFunction( BOOL fExpr, const char *szExpr, const char *szFile, int szLine, const char *szMessage )
    {
        return;
        if (fExpr) return;
        char szOut[512];
        if (szMessage) sprintf( szOut, "ASSERT FAILED:\n %s \n(%s@%d)\n%s", szExpr, szFile, szLine, szMessage );
        else           sprintf( szOut, "ASSERT FAILED:\n %s \n(%s@%d)", szExpr, szFile, szLine );
        ALERT( at_console, szOut );
    }

#endif  //DEBUG




/*
int UTIL_IsMasterTriggered(string_t sMaster, CBaseEntity *pActivator)
{
    if (sMaster)
    {
        edict_t *pentTarget = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(sMaster));
    
        if ( !FNullEnt(pentTarget) )
        {
            CBaseEntity *pMaster = CBaseEntity::Instance(pentTarget);
            if ( pMaster && (pMaster->ObjectCaps() & FCAP_MASTER) )
                return pMaster->IsTriggered( pActivator );
        }

        ALERT(at_console, "Master was null or not a master!\n");
    }

    // if this isn't a master entity, just say yes.
    return 1;
}
*/
