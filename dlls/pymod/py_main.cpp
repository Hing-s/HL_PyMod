#include "pymod/py_main.h"

extern int msgs_count;
extern msg_t MSGS[197];
PyObject *MessageBuffer = NULL;
int msg_type;

void (*ENG_MESSAGE_BEGIN)(int, int, const float *, edict_t *);
void (*ENG_WRITE_BYTE)(int);
void (*ENG_WRITE_CHAR)(int);
void (*ENG_WRITE_SHORT)(int);
void (*ENG_WRITE_LONG)(int);
void (*ENG_WRITE_ANGLE)(float);
void (*ENG_WRITE_COORD)(float);
void (*ENG_WRITE_STRING)(const char *);
void (*ENG_MESSAGE_END)();


void SetPath() {
    PyObject *PATH = PyList_New(0);
    PyList_Append(PATH, PyUnicode_FromString("py/PySDK/"));
    PyList_Append(PATH, PyUnicode_FromString("py/PyLIBS/"));
    PySys_SetObject("path", PATH);
}

void PyInitCore() {
    PyImport_ImportModule("core_init");
}

#define FROM(l, e) l = e
#define TO(l, e) e = l

void ReplaceEngineFuncs(enginefuncs_t *eng) {
    FROM(ENG_MESSAGE_BEGIN, eng->pfnMessageBegin);
    FROM(ENG_WRITE_BYTE, eng->pfnWriteByte);
    FROM(ENG_WRITE_CHAR, eng->pfnWriteChar);
    FROM(ENG_WRITE_LONG, eng->pfnWriteLong);
    FROM(ENG_WRITE_ANGLE, eng->pfnWriteAngle);
    FROM(ENG_WRITE_COORD, eng->pfnWriteCoord);
    FROM(ENG_WRITE_SHORT, eng->pfnWriteShort);
    FROM(ENG_WRITE_STRING, eng->pfnWriteString);
    FROM(ENG_MESSAGE_END, eng->pfnMessageEnd);

    TO(LIB_MESSAGE_BEGIN, eng->pfnMessageBegin);
    TO(LIB_WRITE_BYTE, eng->pfnWriteByte);
    TO(LIB_WRITE_CHAR, eng->pfnWriteChar);
    TO(LIB_WRITE_LONG, eng->pfnWriteLong);
    TO(LIB_WRITE_ANGLE, eng->pfnWriteAngle);
    TO(LIB_WRITE_COORD, eng->pfnWriteCoord);
    TO(LIB_WRITE_SHORT, eng->pfnWriteShort);
    TO(LIB_WRITE_STRING, eng->pfnWriteString);
    TO(LIB_MESSAGE_END, eng->pfnMessageEnd);
}


void PyInit(enginefuncs_t *eng) {
    Py_Initialize();
    SetPath();
    PyInitEngine();
    PyInitCore();
    ReplaceEngineFuncs(eng);
}

bool PyClientCommand(edict_t *pEnt, const char *pcmd, const char *args) {
    bool ignore = false;

    PyObject *builtins = PyImport_ImportModule("builtins");
    Py_XINCREF(builtins);

    PyObject *get_cmd = PyObject_GetAttrString(builtins, "GetCmdFunc");
    Py_XINCREF(get_cmd);
    PyObject *handler = PyObject_CallObject(get_cmd, Py_BuildValue("(s)", pcmd));

    if(handler != Py_None) {
        PyObject *func = PyObject_GetAttrString(handler, "func");

        if(func) {
            PyObject *py_args = Py_BuildValue("(ii)ss", ENTINDEX(pEnt), pEnt->serialnumber, pcmd, args);
            ignore = (PyObject_CallObject(func, py_args) == Py_True);

            Py_XDECREF(py_args);
            Py_XDECREF(func);
        }

        Py_XDECREF(get_cmd);
        Py_XDECREF(handler);
    }

    return ignore;
}

int REG_USER_MSG_ID(const char *msg, int size) {
    int id = REG_USER_MSG(msg, size);

    msg_t smsg;
    smsg.id = id;
    smsg.msg = msg;
    smsg.size = size;
    MSGS[msgs_count] = smsg;
    msgs_count++;
    return id;
}

void LIB_MESSAGE_BEGIN(int dest, int type, const float *flOrigin, edict_t *pEntOwner) {
    if(!MessageBuffer)
        MessageBuffer = PyList_New(0);

    msg_type = type;
    (*ENG_MESSAGE_BEGIN)(dest, type, flOrigin, pEntOwner);
}

void LIB_WRITE_BYTE(int iValue) {
    PyList_Append(MessageBuffer, PyLong_FromLong(iValue));

    ENG_WRITE_BYTE(iValue);
}

void LIB_WRITE_CHAR(int iValue) {
    PyList_Append(MessageBuffer, PyLong_FromLong(iValue));
    ENG_WRITE_CHAR(iValue);
}

void LIB_WRITE_SHORT(int iValue) {
    PyList_Append(MessageBuffer, PyLong_FromLong(iValue));
    ENG_WRITE_SHORT(iValue);
}

void LIB_WRITE_LONG(int iValue) {
    PyList_Append(MessageBuffer, PyLong_FromLong(iValue));
    ENG_WRITE_LONG(iValue);
}

void LIB_WRITE_ANGLE(float flValue) {
    PyList_Append(MessageBuffer, PyFloat_FromDouble((double)flValue));
    ENG_WRITE_ANGLE(flValue);
}

void LIB_WRITE_COORD(float flValue) {
    PyList_Append(MessageBuffer, PyFloat_FromDouble((double)flValue));
    ENG_WRITE_COORD(flValue);
}

void LIB_WRITE_STRING(const char *value) {
    PyList_Append(MessageBuffer, PyUnicode_FromString(value));
    ENG_WRITE_STRING(value);
}

void LIB_MESSAGE_END() {
    PyObject *builtins = PyImport_ImportModule("builtins");
    Py_XINCREF(builtins);

    PyObject *run_hadlers = PyObject_GetAttrString(builtins, "RunMsgHandlers");
    Py_XINCREF(run_hadlers);

    Py_XDECREF(PyObject_CallObject(run_hadlers, Py_BuildValue("iO", msg_type, MessageBuffer)));

    Py_XDECREF(MessageBuffer);
    MessageBuffer = PyList_New(0);

    ENG_MESSAGE_END();
}
