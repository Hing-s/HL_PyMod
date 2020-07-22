#include "exportdef.h"
#include "dllapi.h"
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hl_classes.h"

#define HINSTANCE void *
#define CreateDirectory(p, n) mkdir(p, 0777)
#define GetProcAddress dlsym
#define LoadLibrary(x) dlopen(x, RTLD_NOW)
#define stricmp strcasecmp
#define _stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp

#define VALVE_DLL   1
#define TFC_DLL     2
#define CSTRIKE_DLL 3
#define GEARBOX_DLL 4
#define FRONTLINE_DLL   5
#define HOLYWARS_DLL    6
#define DMC_DLL     7
#define AG_DLL      8
#define HUNGER_DLL  9


BOOL ClientConnect( edict_t *pEntity, const char *pszName,
                    const char *pszAddress, char szRejectReason[ 128 ] );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );

void FakeClientCommand(edict_t *pBot, const char *arg1, const char *arg2, const char *arg3);

const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );

typedef int (*GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (*GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *); 

typedef void (*GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef void (*LINK_ENTITY_FUNC)(entvars_t *);
