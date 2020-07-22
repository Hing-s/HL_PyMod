#include "pymod/py_engine.h"

void PyInit();
void SetPythonPath();

bool PyClientCommand(edict_t *pEnt, const char *pcmd, const char *args);
