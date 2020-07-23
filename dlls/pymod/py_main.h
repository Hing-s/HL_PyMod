#include "pymod/py_engine.h"

void PyInit(enginefuncs_t *eng);
void SetPythonPath();

int REG_USER_MSG_ID(const char *msg, int size);
bool PyClientCommand(edict_t *pEnt, const char *pcmd, const char *args);

void LIB_MESSAGE_BEGIN(int dest, int type, const float *flpOrigin, edict_t *ent_owner);
void LIB_WRITE_BYTE(int iValue);
void LIB_WRITE_CHAR(int iValue);
void LIB_WRITE_SHORT(int iValue);
void LIB_WRITE_LONG(int iValue);
void LIB_WRITE_ANGLE(float flValue);
void LIB_WRITE_COORD(float flValue);
void LIB_WRITE_STRING(const char *value);
void LIB_MESSAGE_END();
