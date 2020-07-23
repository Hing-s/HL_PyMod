#include "pymod/py_main.h"

extern int msgs_count;
extern msg_t MSGS[197];

void SetPath() {
    PyObject *PATH = PyList_New(0);
    PyList_Append(PATH, PyUnicode_FromString("py/PySDK/"));
    PyList_Append(PATH, PyUnicode_FromString("py/PyLIBS/"));
    PySys_SetObject("path", PATH);
}

void PyInitCore() {
    PyImport_ImportModule("core_init");
}


void PyInit() {
    Py_Initialize();
    SetPath();
    PyInitEngine();
    PyInitCore();
}

bool PyClientCommand(edict_t *pEnt, const char *pcmd, const char *args) {
    PyObject *builtins = PyImport_ImportModule("builtins");
    PyObject *get_cmd = PyObject_GetAttrString(builtins, "GetCmdFunc");
    PyObject *handler = PyObject_CallObject(get_cmd, Py_BuildValue("(s)", pcmd));

    if(handler != Py_None) {
        PyObject *func = PyObject_GetAttrString(handler, "func");

        if(func) {
            PyObject *py_args = Py_BuildValue("(ii)ss", ENTINDEX(pEnt), pEnt->serialnumber, pcmd, args);
            PyObject_CallObject(func, py_args);
            Py_DECREF(py_args);
            Py_DECREF(func);
        }

        Py_DECREF(get_cmd);
        Py_DECREF(handler);

        return true;
    }

    return false;
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
