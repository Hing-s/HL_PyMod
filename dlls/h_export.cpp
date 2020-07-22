#include "pymod/py_main.h"

void *h_Library = NULL;

enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals; 
GETENTITYAPI other_GetEntityAPI;
GIVEFNPTRSTODLL other_GiveFnptrsToDll;
GETNEWDLLFUNCTIONS other_GetNewDLLFunctions;

extern "C" void DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{   
    memcpy( &g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t) );
    gpGlobals = pGlobals;

    h_Library = LoadLibrary( "valve/dlls/hl.so" );
    
    if(h_Library)
    {
        PyInit();

        other_GetEntityAPI = (GETENTITYAPI)GetProcAddress( h_Library, "GetEntityAPI" );
        other_GiveFnptrsToDll = (GIVEFNPTRSTODLL)GetProcAddress(h_Library, "GiveFnptrsToDll"); 
        other_GetNewDLLFunctions = (GETNEWDLLFUNCTIONS)GetProcAddress(h_Library, "GetNewDLLFunctions");

        (*other_GiveFnptrsToDll)(&g_engfuncs, pGlobals);
    }
}
