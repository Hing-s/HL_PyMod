#include "pymod/py_engine.h"

void PyInit();
void SetPythonPath();

int REG_USER_MSG_ID(const char *msg, int size);
bool PyClientCommand(edict_t *pEnt, const char *pcmd, const char *args);
